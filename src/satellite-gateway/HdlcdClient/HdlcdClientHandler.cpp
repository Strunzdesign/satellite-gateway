/**
 * \file      HdlcdClientHandler.cpp
 * \brief     
 * \author    Florian Evers, florian-evers@gmx.de
 * \copyright GNU Public License version 3.
 *
 * The HDLC Deamon implements the HDLC protocol to easily talk to devices connected via serial communications.
 * Copyright (C) 2016  Florian Evers, florian-evers@gmx.de
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "HdlcdClientHandler.h"
#include "ConfigServerHandlerCollection.h"
#include "GatewayClientHandlerCollection.h"
#include "HdlcdClient.h"
#include <assert.h>

HdlcdClientHandler::HdlcdClientHandler(boost::asio::io_service& a_IOService, std::shared_ptr<ConfigServerHandlerCollection> a_ConfigServerHandlerCollection,
                                       std::shared_ptr<GatewayClientHandlerCollection> a_GatewayClientHandlerCollection, const std::string& a_RemoteAddress,
                                       uint16_t a_TcpPortNbr, uint16_t a_SerialPortNbr): m_IOService(a_IOService), m_ConfigServerHandlerCollection(a_ConfigServerHandlerCollection),
                                       m_GatewayClientHandlerCollection(a_GatewayClientHandlerCollection), m_RemoteAddress(a_RemoteAddress), m_TcpPortNbr(a_TcpPortNbr),
                                       m_SerialPortNbr(a_SerialPortNbr), m_Resolver(a_IOService), m_ConnectionRetryTimer(a_IOService) {
    // Checks
    assert(m_ConfigServerHandlerCollection);
    assert(m_GatewayClientHandlerCollection);
                                           
    // Trigger activity
    m_bSuspendSerialPort = false;
    ResolveDestination();
}

void HdlcdClientHandler::Close() {
    // Drop all shared pointers
    m_ConfigServerHandlerCollection.reset();
    m_GatewayClientHandlerCollection.reset();
}

void HdlcdClientHandler::Suspend() {
    // Send a control packet to the HDLC daemon to suspend the serial device
    m_bSuspendSerialPort = true;
    if (m_HdlcdClient) {
        // No problem if sent multiple times
        m_HdlcdClient->Send(HdlcdPacketCtrl::CreatePortStatusRequest(true));
    } // if
}

void HdlcdClientHandler::Resume() {
    // Send a control packet to the HDLC daemon to resume the serial device
    m_bSuspendSerialPort = false;
    if (m_HdlcdClient) {
        // No problem if sent multiple times
        m_HdlcdClient->Send(HdlcdPacketCtrl::CreatePortStatusRequest(false));
    } // if
}

void HdlcdClientHandler::SendPacket(const std::vector<unsigned char> &a_Payload) {
    // Send a data packet to the HDLC daemon to deliver the payload via an HDLC I-frame
    if ((m_HdlcdClient) && (!m_bSuspendSerialPort)) {
        // Deliver
        m_HdlcdClient->Send(HdlcdPacketData::CreatePacket(a_Payload, true));
    } else {
        // Drop silently
    } // else
}

void HdlcdClientHandler::ResolveDestination() {
    std::stringstream l_OStream;
    l_OStream << m_TcpPortNbr;
    m_Resolver.async_resolve({m_RemoteAddress, l_OStream.str()}, [this](const boost::system::error_code& a_ErrorCode, boost::asio::ip::tcp::resolver::iterator a_EndpointIterator) {
        if (a_ErrorCode) {
            std::cout << "Failed to resolve host name: " << m_RemoteAddress << std::endl;
            m_ConnectionRetryTimer.expires_from_now(boost::posix_time::seconds(2));
            m_ConnectionRetryTimer.async_wait([this](const boost::system::error_code& a_ErrorCode) {
                if (!a_ErrorCode) {
                    // Reestablish the connection to the HDLC Daemon
                    ResolveDestination();
                } // if
            }); // async_wait
        } else {
            // Start the HDLCd access client. On any error, restart after a short delay
            std::stringstream l_OStream;
            l_OStream << "/dev/ttyUSB" << m_SerialPortNbr;
            m_HdlcdClient = std::make_shared<HdlcdClient>(m_IOService, l_OStream.str(), 0x01);
            m_HdlcdClient->SetOnClosedCallback([this]() {
                m_ConnectionRetryTimer.expires_from_now(boost::posix_time::seconds(2));
                m_ConnectionRetryTimer.async_wait([this](const boost::system::error_code& a_ErrorCode) {
                    if (!a_ErrorCode) {
                        // Reestablish the connection to the HDLC Daemon
                        ResolveDestination();
                    } // if
                }); // async_wait
            }); // SetOnClosedCallback
            
            m_HdlcdClient->SetOnDataCallback([this](const HdlcdPacketData& a_PacketData) {
                // Deliver the payload
                m_GatewayClientHandlerCollection->SendPacket(m_SerialPortNbr, a_PacketData.GetData());
            }); // SetOnDataCallback
            
            m_HdlcdClient->SetOnCtrlCallback([this](const HdlcdPacketCtrl& a_PacketCtrl) {
                if (a_PacketCtrl.GetPacketType() == HdlcdPacketCtrl::CTRL_TYPE_PORT_STATUS) {
                    // Update the state of the serial port
                    m_ConfigServerHandlerCollection->HdlcdClientNewStatus(m_SerialPortNbr, (!a_PacketCtrl.GetIsLockedBySelf() || !a_PacketCtrl.GetIsLockedByOthers()),
                                                                        a_PacketCtrl.GetIsAlive());
                } // if
            }); // SetOnCtrlCallback
            
            // Connect
            m_HdlcdClient->AsyncConnect(a_EndpointIterator, [this](bool a_bSuccess) {
                if (a_bSuccess) {
                    if (m_bSuspendSerialPort) {
                        // Immediately send a serial port suspend request message to the HDLC daemon
                        m_HdlcdClient->Send(HdlcdPacketCtrl::CreatePortStatusRequest(true));
                    } // if
                } else {
                    std::cout << "Failed to connect to the HDLC Daemon!" << std::endl;
                    m_ConnectionRetryTimer.expires_from_now(boost::posix_time::seconds(2));
                    m_ConnectionRetryTimer.async_wait([this](const boost::system::error_code& a_ErrorCode) {
                        if (!a_ErrorCode) {
                            // Reestablish the connection to the HDLC Daemon
                            ResolveDestination();
                        } // if
                    }); // async_wait
                } // else
            }); // AsyncConnect
        } // else
    }); // async_resolve
}
