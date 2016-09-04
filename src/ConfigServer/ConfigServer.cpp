/**
 * \file      ConfigServer.cpp
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

#include "ConfigServer.h"
#include "../GatewayClient/GatewayClientHandlerCollection.h"
#include "../HdlcdClient/HdlcdClientHandlerCollection.h"
#include <assert.h>

ConfigServer::ConfigServer(boost::asio::io_service& a_IOService): m_IOService(a_IOService) {
}

void ConfigServer::Initialize(std::shared_ptr<GatewayClientHandlerCollection> a_GatewayClientHandlerCollection,
                              std::shared_ptr<HdlcdClientHandlerCollection>   a_HdlcdClientHandlerCollection) {
    assert(a_GatewayClientHandlerCollection);
    assert(a_HdlcdClientHandlerCollection);
    m_GatewayClientHandlerCollection = a_GatewayClientHandlerCollection;
    m_HdlcdClientHandlerCollection   = a_HdlcdClientHandlerCollection;
}

void ConfigServer::Close() {
}

void ConfigServer::GatewayClientCreated(uint32_t a_ReferenceNbr) {
}

void ConfigServer::GatewayClientDestroyed(uint32_t a_ReferenceNbr) {
}

void ConfigServer::GatewayClientConnected(uint32_t a_ReferenceNbr) {
}

void ConfigServer::GatewayClientDisconnected(uint32_t a_ReferenceNbr) {
}

void ConfigServer::GatewayClientError(uint32_t a_ReferenceNbr, uint32_t a_ErrorCode) {
}

void ConfigServer::HdlcdClientCreated(uint16_t a_SerialPortNbr) {
}

void ConfigServer::HdlcdClientDestroyed(uint16_t a_SerialPortNbr) {
}

void ConfigServer::HdlcdClientDeviceFound(uint16_t a_SerialPortNbr) {
}

void ConfigServer::HdlcdClientDeviceLost(uint16_t a_SerialPortNbr) {
}

void ConfigServer::HdlcdClientNewStatus(uint16_t a_SerialPortNbr, bool a_bIsResumed, bool a_bIsAlive) {
}

void ConfigServer::HdlcdClientError(uint16_t a_SerialPortNbr, uint32_t a_ErrorCode) {
}
