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
#include "../ConfigurationServer/ConfigurationServerHandler.h"
#include "../HdlcdClient/HdlcdClientHandlerCollection.h"
#include <assert.h>

GatewayClientHandler::GatewayClientHandler(boost::asio::io_service& a_IOService): m_IOService(a_IOService) {
}

void GatewayClientHandler::Initialize(std::shared_ptr<ConfigurationServerHandler> a_ConfigurationServerHandler,
                                      std::shared_ptr<HdlcdClientHandlerCollection> a_HdlcdClientHandlerCollection) {
    // Checks
    assert(a_ConfigurationServerHandler);
    assert(a_HdlcdClientHandlerCollection);
    m_ConfigurationServerHandler = a_ConfigurationServerHandler;    
    m_HdlcdClientHandlerCollection = a_HdlcdClientHandlerCollection;
}

void GatewayClientHandler::Reset() {
    // Drop all shared pointers
    m_ConfigurationServerHandler.reset();
    m_HdlcdClientHandlerCollection.reset();
}

void GatewayClientHandler::Connect(uint32_t a_ReferenceNbr) {
}

void GatewayClientHandler::Disconnect() {
}

void GatewayClientHandler::SendPacket(uint16_t a_SerialPortNbr, const std::vector<unsigned char> &a_Buffer) {
}
    
