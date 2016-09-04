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
#include "../GatewayClient/GatewayClientHandlerCollection.h"
#include "../HdlcdClient/HdlcdClientHandlerCollection.h"
#include "ConfigServerHandler.h"
#include <assert.h>

ConfigServerHandlerCollection::ConfigServerHandlerCollection(boost::asio::io_service& a_IOService): m_IOService(a_IOService) {
}

void ConfigServerHandlerCollection::Initialize(std::shared_ptr<GatewayClientHandlerCollection> a_GatewayClientHandlerCollection,
                                               std::shared_ptr<HdlcdClientHandlerCollection>   a_HdlcdClientHandlerCollection) {
    assert(a_GatewayClientHandlerCollection);
    assert(a_HdlcdClientHandlerCollection);
    m_GatewayClientHandlerCollection = a_GatewayClientHandlerCollection;
    m_HdlcdClientHandlerCollection   = a_HdlcdClientHandlerCollection;
}

void ConfigServerHandlerCollection::SystemShutdown() {
}

void ConfigServerHandlerCollection::GatewayClientCreated(uint32_t a_ReferenceNbr) {
}

void ConfigServerHandlerCollection::GatewayClientDestroyed(uint32_t a_ReferenceNbr) {
}

void ConfigServerHandlerCollection::GatewayClientConnected(uint32_t a_ReferenceNbr) {
}

void ConfigServerHandlerCollection::GatewayClientDisconnected(uint32_t a_ReferenceNbr) {
}

void ConfigServerHandlerCollection::GatewayClientError(uint32_t a_ReferenceNbr, uint32_t a_ErrorCode) {
}

void ConfigServerHandlerCollection::HdlcdClientCreated(uint16_t a_SerialPortNbr) {
}

void ConfigServerHandlerCollection::HdlcdClientDestroyed(uint16_t a_SerialPortNbr) {
}

void ConfigServerHandlerCollection::HdlcdClientDeviceFound(uint16_t a_SerialPortNbr) {
}

void ConfigServerHandlerCollection::HdlcdClientDeviceLost(uint16_t a_SerialPortNbr) {
}

void ConfigServerHandlerCollection::HdlcdClientNewStatus(uint16_t a_SerialPortNbr, bool a_bIsResumed, bool a_bIsAlive) {
}

void ConfigServerHandlerCollection::HdlcdClientError(uint16_t a_SerialPortNbr, uint32_t a_ErrorCode) {
}
