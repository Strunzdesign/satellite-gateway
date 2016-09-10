/**
 * \file      ConfigServerHandler.cpp
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

#include "ConfigServerHandler.h"
#include "GatewayClientHandlerCollection.h"
#include "ConfigServerHandlerCollection.h"
#include "ConfigServer.h"
#include <assert.h>

ConfigServerHandler::ConfigServerHandler(boost::asio::io_service& a_IOService, boost::asio::ip::tcp::tcp::socket& a_TcpSocket, std::shared_ptr<GatewayClientHandlerCollection> a_GatewayClientHandlerCollection, std::shared_ptr<HdlcdClientHandlerCollection> a_HdlcdClientHandlerCollection): m_IOService(a_IOService), m_GatewayClientHandlerCollection(a_GatewayClientHandlerCollection), m_HdlcdClientHandlerCollection(a_HdlcdClientHandlerCollection) {
    // Checks
    assert(m_GatewayClientHandlerCollection);
    assert(m_HdlcdClientHandlerCollection);
    m_Registered = false;
    m_ConfigServer = std::make_shared<ConfigServer>(m_IOService, a_TcpSocket);
}

ConfigServerHandler::~ConfigServerHandler() {
    Close();
}

void ConfigServerHandler::Start(std::shared_ptr<ConfigServerHandlerCollection> a_ConfigServerHandlerCollection) {
    // Checks
    assert(a_ConfigServerHandlerCollection);
    assert(m_Registered == false);
    
    // Register to the collection
    m_Registered = true;
    m_ConfigServerHandlerCollection = a_ConfigServerHandlerCollection;
    m_ConfigServerHandlerCollection->RegisterConfigServerHandler(shared_from_this());

    // Trigger activity
    assert(m_ConfigServer);
    m_ConfigServer->Start(m_GatewayClientHandlerCollection, m_HdlcdClientHandlerCollection);
    
}

void ConfigServerHandler::Close() {
    if (m_ConfigServer) {
        m_ConfigServer->Close();
        m_ConfigServer.reset();
    } // if
    
    // Deregister from the collection
    if ((m_ConfigServerHandlerCollection) && (m_Registered)) {
        m_ConfigServerHandlerCollection->DeregisterConfigServerHandler(shared_from_this());
        m_Registered = false;
    } // if

    // Drop all shared pointers
    m_GatewayClientHandlerCollection.reset();
    m_HdlcdClientHandlerCollection.reset();
    m_ConfigServerHandlerCollection.reset();
}

void ConfigServerHandler::GatewayClientCreated(uint16_t a_ReferenceNbr) {
    if (m_ConfigServer) {
        // Relay the call to the config server entity
        m_ConfigServer->GatewayClientCreated(a_ReferenceNbr);
    } // if
}

void ConfigServerHandler::GatewayClientDestroyed(uint16_t a_ReferenceNbr) {
    if (m_ConfigServer) {
        // Relay the call to the config server entity
        m_ConfigServer->GatewayClientDestroyed(a_ReferenceNbr);
    } // if
}

void ConfigServerHandler::GatewayClientConnected(uint16_t a_ReferenceNbr) {
    if (m_ConfigServer) {
        // Relay the call to the config server entity
        m_ConfigServer->GatewayClientConnected(a_ReferenceNbr);
    } // if
}

void ConfigServerHandler::GatewayClientDisconnected(uint16_t a_ReferenceNbr) {
    if (m_ConfigServer) {
        // Relay the call to the config server entity
        m_ConfigServer->GatewayClientDisconnected(a_ReferenceNbr);
    } // if
}

void ConfigServerHandler::GatewayClientError(uint16_t a_ReferenceNbr, uint16_t a_ErrorCode) {
    if (m_ConfigServer) {
        // Relay the call to the config server entity
        m_ConfigServer->GatewayClientError(a_ReferenceNbr, a_ErrorCode);
    } // if
}

void ConfigServerHandler::HdlcdClientCreated(uint16_t a_SerialPortNbr) {
    if (m_ConfigServer) {
        // Relay the call to the config server entity
        m_ConfigServer->HdlcdClientCreated(a_SerialPortNbr);
    } // if
}

void ConfigServerHandler::HdlcdClientDestroyed(uint16_t a_SerialPortNbr) {
    if (m_ConfigServer) {
        // Relay the call to the config server entity
        m_ConfigServer->HdlcdClientDestroyed(a_SerialPortNbr);
    } // if
}

void ConfigServerHandler::HdlcdClientNewStatus(uint16_t a_SerialPortNbr, bool a_bIsResumed, bool a_bIsAlive) {
    if (m_ConfigServer) {
        // Relay the call to the config server entity
        m_ConfigServer->HdlcdClientNewStatus(a_SerialPortNbr, a_bIsResumed, a_bIsAlive);
    } // if
}

void ConfigServerHandler::HdlcdClientError(uint16_t a_SerialPortNbr, uint16_t a_ErrorCode) {
    if (m_ConfigServer) {
        // Relay the call to the config server entity
        m_ConfigServer->HdlcdClientError(a_SerialPortNbr, a_ErrorCode);
    } // if
}
