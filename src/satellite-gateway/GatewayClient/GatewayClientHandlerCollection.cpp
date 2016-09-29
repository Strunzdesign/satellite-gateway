/**
 * \file      GatewayClientHandlerCollection.cpp
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

#include "GatewayClientHandlerCollection.h"
#include "ConfigServerHandlerCollection.h"
#include "HdlcdClientHandlerCollection.h"
#include "GatewayClientHandler.h"
#include "GatewayClientErrorCodes.h"
#include <assert.h>

GatewayClientHandlerCollection::GatewayClientHandlerCollection(boost::asio::io_service& a_IOService): m_IOService(a_IOService) {
}

void GatewayClientHandlerCollection::Initialize(std::shared_ptr<ConfigServerHandlerCollection> a_ConfigServerHandlerCollection,
                                                std::shared_ptr<HdlcdClientHandlerCollection>  a_HdlcdClientHandlerCollection) {
    assert(a_ConfigServerHandlerCollection);
    assert(a_HdlcdClientHandlerCollection);
    m_ConfigServerHandlerCollection = a_ConfigServerHandlerCollection;
    m_HdlcdClientHandlerCollection  = a_HdlcdClientHandlerCollection;
}

void GatewayClientHandlerCollection::SystemShutdown() {
    // Silently remove the previous gateway client
    if (m_GatewayClientHandler) {
        m_GatewayClientHandler->Close();
        m_GatewayClientHandler.reset();
    } // if
    
    // Drop all shared pointers
    m_ConfigServerHandlerCollection.reset();
    m_HdlcdClientHandlerCollection.reset();
}

void GatewayClientHandlerCollection::CleanAll() {
    // Silently remove the previous gateway client
    if (m_GatewayClientHandler) {
        m_GatewayClientHandler->Close();
        m_GatewayClientHandler.reset();
    } // if
}

void GatewayClientHandlerCollection::CreateClient(uint16_t a_ReferenceNbr, std::string a_RemoteAddress, uint16_t a_RemotePortNbr) {
    if (m_GatewayClientHandler) {
        // There was already a client entity! Replace it.
        m_ConfigServerHandlerCollection->GatewayClientError(a_ReferenceNbr, GATEWAY_CLIENT_ALREADY_EXISTED);
        m_ConfigServerHandlerCollection->GatewayClientDestroyed(m_GatewayClientHandler->GetReferenceNbr());
        m_GatewayClientHandler->Close();
        m_GatewayClientHandler.reset();
    } // if
    
    // Create new entity
    m_GatewayClientHandler = std::make_shared<GatewayClientHandler>(m_IOService, m_ConfigServerHandlerCollection, m_HdlcdClientHandlerCollection,
                                                                    a_ReferenceNbr, a_RemoteAddress, a_RemotePortNbr);
    m_ConfigServerHandlerCollection->GatewayClientCreated(a_ReferenceNbr);
}

void GatewayClientHandlerCollection::DestroyClient(uint16_t a_ReferenceNbr) {
    if (m_GatewayClientHandler) {
        if (m_GatewayClientHandler->GetReferenceNbr() == a_ReferenceNbr) {
            m_ConfigServerHandlerCollection->GatewayClientDestroyed(a_ReferenceNbr);
            m_GatewayClientHandler->Close();
            m_GatewayClientHandler.reset();
        } else {
            // The reference number did not match!
            m_ConfigServerHandlerCollection->GatewayClientError(a_ReferenceNbr, GATEWAY_CLIENT_NO_MATCH);
        } // else
    } else {
        // There was no client entity!
        m_ConfigServerHandlerCollection->GatewayClientError(a_ReferenceNbr, GATEWAY_CLIENT_NOT_EXISTED);
    } // else
}

void GatewayClientHandlerCollection::SendPacket(uint16_t a_SerialPortNbr, const std::vector<unsigned char> &a_Buffer) {
    if (m_GatewayClientHandler) {
        m_GatewayClientHandler->SendPacket(a_SerialPortNbr, a_Buffer);
    } // if
}
