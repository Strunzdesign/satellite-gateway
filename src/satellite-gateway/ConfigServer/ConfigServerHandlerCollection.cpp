/**
 * \file      ConfigServerHandlerCollection.cpp
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

#include "ConfigServerHandlerCollection.h"
#include "GatewayClientHandlerCollection.h"
#include "HdlcdClientHandlerCollection.h"
#include "ConfigServerHandler.h"
#include <assert.h>
using boost::asio::ip::tcp;

ConfigServerHandlerCollection::ConfigServerHandlerCollection(boost::asio::io_service& a_IOService, uint16_t a_TcpPortNbr): m_IOService(a_IOService), m_TcpAcceptor(a_IOService, tcp::endpoint(tcp::v4(), a_TcpPortNbr)), m_TcpSocket(a_IOService) {
}

void ConfigServerHandlerCollection::Initialize(std::shared_ptr<GatewayClientHandlerCollection> a_GatewayClientHandlerCollection,
                                               std::shared_ptr<HdlcdClientHandlerCollection>   a_HdlcdClientHandlerCollection) {
    assert(a_GatewayClientHandlerCollection);
    assert(a_HdlcdClientHandlerCollection);
    m_GatewayClientHandlerCollection = a_GatewayClientHandlerCollection;
    m_HdlcdClientHandlerCollection   = a_HdlcdClientHandlerCollection;
    
    // Start accepting incoming TCP connections
    DoAccept();
}

void ConfigServerHandlerCollection::SystemShutdown() {
    // Stop accepting subsequent TCP connections
    m_TcpAcceptor.close();

    // Drop all shared pointers
    if (m_ConfigServerHandler) {
        m_ConfigServerHandler->Close();
        m_ConfigServerHandler.reset();
    } // if

    m_GatewayClientHandlerCollection.reset();
    m_HdlcdClientHandlerCollection.reset();
}

void ConfigServerHandlerCollection::RegisterConfigServerHandler(std::shared_ptr<ConfigServerHandler> a_ConfigServerHandler) {
    // This "collection" does only allow one single incoming control connection. If one already existed, it is stopped and discarded
    assert(a_ConfigServerHandler);
    if (m_ConfigServerHandler) {
        m_ConfigServerHandler->Close();
    } // if
    
    m_ConfigServerHandler = a_ConfigServerHandler;
}

void ConfigServerHandlerCollection::DeregisterConfigServerHandler(std::shared_ptr<ConfigServerHandler> a_ConfigServerHandler) {
    // One control connection terminated. If this is the current one, drop it.
    assert(a_ConfigServerHandler);
    if (m_ConfigServerHandler == a_ConfigServerHandler) {
        // Closing is not required anymore (already in DTOR)
        m_ConfigServerHandler.reset();
    } // if
}

void ConfigServerHandlerCollection::DoAccept() {
    m_TcpAcceptor.async_accept(m_TcpSocket, [this](boost::system::error_code a_ErrorCode) {
        if (!a_ErrorCode) {
            // Create config server handler and start it. It registers and deregisters itself
            auto l_ConfigServerHandler = std::make_shared<ConfigServerHandler>(m_IOService, m_TcpSocket, m_GatewayClientHandlerCollection, m_HdlcdClientHandlerCollection);
            l_ConfigServerHandler->Start(shared_from_this());
        } // if

        // Wait for subsequent TCP connections
        DoAccept();
    });
}

void ConfigServerHandlerCollection::GatewayClientCreated(uint16_t a_ReferenceNbr) {
    // There is only one config server handler entity
    if (m_ConfigServerHandler) {
        m_ConfigServerHandler->GatewayClientCreated(a_ReferenceNbr);
    } // if
}

void ConfigServerHandlerCollection::GatewayClientDestroyed(uint16_t a_ReferenceNbr) {
    // There is only one config server handler entity
    if (m_ConfigServerHandler) {
        m_ConfigServerHandler->GatewayClientDestroyed(a_ReferenceNbr);
    } // if
}

void ConfigServerHandlerCollection::GatewayClientConnected(uint16_t a_ReferenceNbr) {
    // There is only one config server handler entity
    if (m_ConfigServerHandler) {
        m_ConfigServerHandler->GatewayClientConnected(a_ReferenceNbr);
    } // if
}

void ConfigServerHandlerCollection::GatewayClientDisconnected(uint16_t a_ReferenceNbr) {
    // There is only one config server handler entity
    if (m_ConfigServerHandler) {
        m_ConfigServerHandler->GatewayClientDisconnected(a_ReferenceNbr);
    } // if
}

void ConfigServerHandlerCollection::GatewayClientError(uint16_t a_ReferenceNbr, uint16_t a_ErrorCode) {
    // There is only one config server handler entity
    if (m_ConfigServerHandler) {
        m_ConfigServerHandler->GatewayClientError(a_ReferenceNbr, a_ErrorCode);
    } // if
}

void ConfigServerHandlerCollection::HdlcdClientCreated(uint16_t a_SerialPortNbr) {
    // There is only one config server handler entity
    if (m_ConfigServerHandler) {
        m_ConfigServerHandler->HdlcdClientCreated(a_SerialPortNbr);
    } // if
}

void ConfigServerHandlerCollection::HdlcdClientDestroyed(uint16_t a_SerialPortNbr) {
    // There is only one config server handler entity
    if (m_ConfigServerHandler) {
        m_ConfigServerHandler->HdlcdClientDestroyed(a_SerialPortNbr);
    } // if
}

void ConfigServerHandlerCollection::HdlcdClientNewStatus(uint16_t a_SerialPortNbr, bool a_bIsResumed, bool a_bIsAlive) {
    // There is only one config server handler entity
    if (m_ConfigServerHandler) {
        m_ConfigServerHandler->HdlcdClientNewStatus(a_SerialPortNbr, a_bIsResumed, a_bIsAlive);
    } // if
}

void ConfigServerHandlerCollection::HdlcdClientError(uint16_t a_SerialPortNbr, uint16_t a_ErrorCode) {
    // There is only one config server handler entity
    if (m_ConfigServerHandler) {
        m_ConfigServerHandler->HdlcdClientError(a_SerialPortNbr, a_ErrorCode);
    } // if
}
