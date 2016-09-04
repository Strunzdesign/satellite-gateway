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
#include <assert.h>

GatewayClientHandler::GatewayClientHandler(boost::asio::io_service& a_IOService, std::shared_ptr<ConfigServerHandlerCollection> a_ConfigServerHandlerCollection,
                                           std::shared_ptr<HdlcdClientHandlerCollection> a_HdlcdClientHandlerCollection):
                                           m_IOService(a_IOService), m_ConfigServerHandlerCollection(a_ConfigServerHandlerCollection), m_HdlcdClientHandlerCollection(a_HdlcdClientHandlerCollection) {
    // Checks
    assert(m_ConfigServerHandlerCollection);
    assert(m_HdlcdClientHandlerCollection);
}

void GatewayClientHandler::Reset() {
    // Drop all shared pointers
    m_ConfigServerHandlerCollection.reset();
    m_HdlcdClientHandlerCollection.reset();
}

void GatewayClientHandler::CleanAll() {
    // If a gateway client already exists it must be destoyed
    if (m_GatewayClient) {
        m_GatewayClient->Close();
    } // if
}

void GatewayClientHandler::Connect(uint32_t a_ReferenceNbr) {
    // If a gateway client already exists, it must be destoyed first
    if (m_GatewayClient) {
        m_GatewayClient->Close();
    } // if
    
    // Create a new gateway client entity
    m_GatewayClient = std::make_shared<GatewayClient>(m_IOService, m_ConfigServerHandlerCollection, m_HdlcdClientHandlerCollection, a_ReferenceNbr);
}

void GatewayClientHandler::Disconnect(uint32_t a_ReferenceNbr) {
    // Drop the current client entity
    if (m_GatewayClient) {
        if (m_GatewayClient->GetReferenceNbr() == a_ReferenceNbr) {
            m_GatewayClient->Close();
            m_GatewayClient.reset();
        } else {
            // Error: the reference number does not match!
            m_ConfigServerHandlerCollection->GatewayClientError(a_ReferenceNbr, 0x00);
        } // else
    } else {
        // Error: there was no gateway client identity
        m_ConfigServerHandlerCollection->GatewayClientError(a_ReferenceNbr, 0x00);
    } // else
}

void GatewayClientHandler::SendPacket(uint16_t a_SerialPortNbr, const std::vector<unsigned char> &a_Buffer) {
    // Deliver the packet to the gateway client entity
    if (m_GatewayClient) {
        m_GatewayClient->SendPacket(a_SerialPortNbr, a_Buffer);
    } // if
}
