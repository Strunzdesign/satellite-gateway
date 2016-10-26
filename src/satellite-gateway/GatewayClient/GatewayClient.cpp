/**
 * \file      GatewayClient.cpp
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

#include "GatewayClient.h"
#include "ConfigServerHandlerCollection.h"
#include "HdlcdClientHandlerCollection.h"
#include "GatewayFrameData.h"
#include <assert.h>

GatewayClient::GatewayClient(boost::asio::io_service& a_IOService, boost::asio::ip::tcp::resolver::iterator a_EndpointIterator, std::shared_ptr<ConfigServerHandlerCollection>
                             a_ConfigServerHandlerCollection, std::shared_ptr<HdlcdClientHandlerCollection> a_HdlcdClientHandlerCollection):
                             m_IOService(a_IOService), m_ConfigServerHandlerCollection(a_ConfigServerHandlerCollection), m_HdlcdClientHandlerCollection(a_HdlcdClientHandlerCollection),
                             m_TcpSocket(a_IOService), m_bClosed(false) {
    // Checks
    assert(m_ConfigServerHandlerCollection);
    assert(m_HdlcdClientHandlerCollection);

    // Connect the TCP socket
    boost::asio::async_connect(m_TcpSocket, a_EndpointIterator, [this, &a_IOService](boost::system::error_code a_ErrorCode, boost::asio::ip::tcp::resolver::iterator) {
        if (a_ErrorCode == boost::asio::error::operation_aborted) return;
        if (a_ErrorCode) {
            std::cerr << "Connection to the master gateway was not established, error = " << a_ErrorCode << std::endl;
            Close();
        } else {
            std::cerr << "Connection to the master gateway was established" << std::endl;
            
            // Configure the frame end point
            m_FrameEndpoint = std::make_shared<FrameEndpoint>(a_IOService, m_TcpSocket, 0x00); // 0x00: unset the filter mask as there is no preceeding type byte
            m_FrameEndpoint->RegisterFrameFactory(GATEWAY_FRAME_DATA, []()->std::shared_ptr<Frame>{ return GatewayFrameData::CreateDeserializedFrame (); });
            m_FrameEndpoint->SetOnFrameCallback  ([this](std::shared_ptr<Frame> a_Frame)->bool{ return OnFrame(a_Frame); });
            m_FrameEndpoint->SetOnClosedCallback ([this](){ OnClosed(); });
            m_FrameEndpoint->Start();
        } // else
    }); // async_connect
}

GatewayClient::~GatewayClient() {
    Close();
}

void GatewayClient::Shutdown() {
    m_FrameEndpoint->Shutdown();
}

void GatewayClient::Close() {
    if (m_bClosed == false) {
        m_bClosed = true;
        if (m_FrameEndpoint) {
            m_FrameEndpoint->Close();
        } // if

        if (m_OnClosedCallback) {
            m_OnClosedCallback();
        } // if
    } // if
    
    // Drop all shared pointers
    m_ConfigServerHandlerCollection.reset();
    m_HdlcdClientHandlerCollection.reset();
}

void GatewayClient::SendPacket(uint16_t a_SerialPortNbr, const std::vector<unsigned char> &a_Payload) {
    // Deliver the packet to the gateway client entity
    m_FrameEndpoint->SendFrame(GatewayFrameData::Create(a_SerialPortNbr, a_Payload));
}

void GatewayClient::SetOnClosedCallback(std::function<void()> a_OnClosedCallback) {
    m_OnClosedCallback = a_OnClosedCallback;
}

bool GatewayClient::OnFrame(std::shared_ptr<Frame> a_Frame) {
    // Dispatch
    assert(a_Frame);
    auto l_GatewayFrameData = std::dynamic_pointer_cast<GatewayFrameData>(a_Frame);
    assert(l_GatewayFrameData);
    m_HdlcdClientHandlerCollection->SendPacket(l_GatewayFrameData->GetSerialPortNbr(), l_GatewayFrameData->GetPayload());
    return true;
}

void GatewayClient::OnClosed() {
    Close();
}
