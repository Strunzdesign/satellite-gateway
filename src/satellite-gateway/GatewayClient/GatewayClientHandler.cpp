/**
 * \file      GatewayClientHandler.cpp
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

#include "GatewayClientHandler.h"
#include "GatewayClient.h"
#include "ConfigServerHandlerCollection.h"
#include "HdlcdClientHandlerCollection.h"
#include "GatewayFrameData.h"
#include <assert.h>

GatewayClientHandler::GatewayClientHandler(boost::asio::io_service& a_IOService, std::shared_ptr<ConfigServerHandlerCollection> a_ConfigServerHandlerCollection,
                                           std::shared_ptr<HdlcdClientHandlerCollection> a_HdlcdClientHandlerCollection, uint16_t a_ReferenceNbr,
                                           std::string a_RemoteAddress, uint16_t a_RemotePortNbr):
                                           m_IOService(a_IOService), m_ConfigServerHandlerCollection(a_ConfigServerHandlerCollection),
                                           m_HdlcdClientHandlerCollection(a_HdlcdClientHandlerCollection), m_RemoteAddress(a_RemoteAddress),
                                           m_RemotePortNbr(a_RemotePortNbr), m_ReferenceNbr(a_ReferenceNbr), m_Resolver(a_IOService), m_ConnectionRetryTimer(a_IOService) {
    // Checks
    assert(m_ConfigServerHandlerCollection);
    assert(m_HdlcdClientHandlerCollection);
    
    // Trigger activity
    ResolveDestination();
}

void GatewayClientHandler::Close() {
    // Drop all shared pointers
    if (m_GatewayClient) {
        m_GatewayClient->Close();
        m_GatewayClient.reset();
    } // if

    m_ConfigServerHandlerCollection.reset();
    m_HdlcdClientHandlerCollection.reset();
}

void GatewayClientHandler::SendPacket(uint16_t a_SerialPortNbr, const std::vector<unsigned char> &a_Buffer) {
    if (m_GatewayClient) {
        m_GatewayClient->SendPacket(a_SerialPortNbr, a_Buffer);
    } // if
}

void GatewayClientHandler::ResolveDestination() {
    std::stringstream l_OStream;
    l_OStream << m_RemotePortNbr;
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
            // Start the HDLCd access client
            m_GatewayClient = std::make_shared<GatewayClient>(m_IOService, a_EndpointIterator, m_ConfigServerHandlerCollection, m_HdlcdClientHandlerCollection, m_ReferenceNbr);
            
            // On any error, restart after a short delay
            m_GatewayClient->SetOnClosedCallback([this]() {
                m_ConnectionRetryTimer.expires_from_now(boost::posix_time::seconds(2));
                m_ConnectionRetryTimer.async_wait([this](const boost::system::error_code& a_ErrorCode) {
                    if (!a_ErrorCode) {
                        // Reestablish the connection to the HDLC Daemon
                        ResolveDestination();
                    } // if
                }); // async_wait
            }); // SetOnClosedCallback
        } // else
    }); // async_resolve
}
