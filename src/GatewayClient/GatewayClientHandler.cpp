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
#include "../ConfigServer/ConfigServerHandlerCollection.h"
#include "../HdlcdClient/HdlcdClientHandlerCollection.h"
#include "GatewayFrames/GatewayFrameData.h"
#include <assert.h>

GatewayClientHandler::GatewayClientHandler(boost::asio::io_service& a_IOService, std::shared_ptr<ConfigServerHandlerCollection> a_ConfigServerHandlerCollection,
                                           std::shared_ptr<HdlcdClientHandlerCollection> a_HdlcdClientHandlerCollection, uint16_t a_ReferenceNbr,
                                           std::string a_RemoteAddress, uint16_t a_RemotePortNbr):
                                           m_IOService(a_IOService), m_ConfigServerHandlerCollection(a_ConfigServerHandlerCollection),
                                           m_HdlcdClientHandlerCollection(a_HdlcdClientHandlerCollection), m_ReferenceNbr(a_ReferenceNbr),
                                           m_RemoteAddress(a_RemoteAddress), m_RemotePortNbr(a_RemotePortNbr) {
    // Checks
    assert(m_ConfigServerHandlerCollection);
    assert(m_HdlcdClientHandlerCollection);
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

void GatewayClientHandler::GatewayFrameReceived(const std::shared_ptr<GatewayFrame> &a_GatewayFrame) {
    // Dispatch
    assert(a_GatewayFrame);
    switch (a_GatewayFrame->GetGatewayFrameType()) {
        case GATEWAY_FRAME_DATA: {
            auto l_GatewayFrameData = std::dynamic_pointer_cast<GatewayFrameData>(a_GatewayFrame);
            m_HdlcdClientHandlerCollection->SendPacket(l_GatewayFrameData->GetSerialPortNbr(), l_GatewayFrameData->GetPayload());
            break;
        }
        case GATEWAY_FRAME_UNKNOWN:
        default:
            // Ignore
            break;
    } // switch
}

void GatewayClientHandler::SendPacket(uint16_t a_SerialPortNbr, const std::vector<unsigned char> &a_Payload) {
    // Deliver the packet to the gateway client entity
    if (m_GatewayClient) {
        m_GatewayClient->SendGatewayFrame(GatewayFrameData::Create(a_SerialPortNbr, a_Payload));
    } // if
}
