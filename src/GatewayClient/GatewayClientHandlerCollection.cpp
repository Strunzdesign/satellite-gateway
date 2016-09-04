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
#include "../ConfigServer/ConfigServerHandlerCollection.h"
#include "../HdlcdClient/HdlcdClientHandlerCollection.h"
#include "GatewayClientHandler.h"
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
}

void GatewayClientHandlerCollection::CleanAll() {
}

void GatewayClientHandlerCollection::Connect(uint32_t a_ReferenceNbr) {
}

void GatewayClientHandlerCollection::Disconnect(uint32_t a_ReferenceNbr) {
}

void GatewayClientHandlerCollection::SendPacket(uint16_t a_SerialPortNbr, const std::vector<unsigned char> &a_Buffer) {
}
