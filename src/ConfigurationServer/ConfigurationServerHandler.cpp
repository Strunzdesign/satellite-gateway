/**
 * \file      ConfigurationServerHandler.cpp
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

#include "ConfigurationServerHandler.h"
#include "../GatewayClient/GatewayClientHandler.h"
#include "../HdlcdClient/HdlcdClientHandlerCollection.h"
#include <assert.h>

ConfigurationServerHandler::ConfigurationServerHandler(boost::asio::io_service& a_IOService): m_IOService(a_IOService) {
}

void ConfigurationServerHandler::Initialize(std::shared_ptr<GatewayClientHandler> a_GatewayClientHandler,
                                            std::shared_ptr<HdlcdClientHandlerCollection> a_HdlcdClientHandlerCollection) {
    // Checks
    assert(a_GatewayClientHandler);
    assert(a_HdlcdClientHandlerCollection);
    m_GatewayClientHandler = a_GatewayClientHandler;
    m_HdlcdClientHandlerCollection = a_HdlcdClientHandlerCollection;
}

void ConfigurationServerHandler::Reset() {
    // Drop all shared pointers
    m_GatewayClientHandler.reset();
    m_HdlcdClientHandlerCollection.reset();
}

void ConfigurationServerHandler::GatewayClientCreated(uint32_t a_ReferenceNbr) {
    // Send control packet
}

void ConfigurationServerHandler::GatewayClientDestroyed(uint32_t a_ReferenceNbr) {
    // Send control packet
}

void ConfigurationServerHandler::GatewayClientConnected(uint32_t a_ReferenceNbr) {
    // Send control packet
}

void ConfigurationServerHandler::GatewayClientDisconnected(uint32_t a_ReferenceNbr) {
    // Send control packet
}

void ConfigurationServerHandler::GatewayClientError(uint32_t a_ReferenceNbr, uint32_t a_ErrorCode) {
}

void ConfigurationServerHandler::HdlcdClientCreated(uint16_t a_SerialPortNbr) {
    // Send control packet
}

void ConfigurationServerHandler::HdlcdClientDestroyed(uint16_t a_SerialPortNbr) {
    // Send control packet
}

void ConfigurationServerHandler::HdlcdClientDeviceFound(uint16_t a_SerialPortNbr) {
    // Send control packet
}

void ConfigurationServerHandler::HdlcdClientDeviceLost(uint16_t a_SerialPortNbr) {
    // Send control packet
}

void ConfigurationServerHandler::HdlcdClientNewStatus(uint16_t a_SerialPortNbr, bool a_bIsResumed, bool a_bIsAlive) {
    // Send control packet
}

void ConfigurationServerHandler::HdlcdClientError(uint16_t a_SerialPortNbr, uint32_t a_ErrorCode) {
    // Send control packet
}
