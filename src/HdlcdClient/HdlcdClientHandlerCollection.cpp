/**
 * \file      HdlcdClientHandlerCollection.cpp
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

#include "HdlcdClientHandlerCollection.h"
#include "../ConfigServer/ConfigServerHandlerCollection.h"
#include "../GatewayClient/GatewayClientHandlerCollection.h"
#include "HdlcdClientHandler.h"
#include <assert.h>

HdlcdClientHandlerCollection::HdlcdClientHandlerCollection(boost::asio::io_service& a_IOService): m_IOService(a_IOService) {
}

void HdlcdClientHandlerCollection::Initialize(std::shared_ptr<ConfigServerHandlerCollection> a_ConfigServerHandlerCollection,
                                              std::shared_ptr<GatewayClientHandlerCollection> a_GatewayClientHandlerCollection) {
    // Checks
    assert(a_ConfigServerHandlerCollection);
    assert(a_GatewayClientHandlerCollection);
    m_ConfigServerHandlerCollection = a_ConfigServerHandlerCollection;
    m_GatewayClientHandlerCollection = a_GatewayClientHandlerCollection;
}

void HdlcdClientHandlerCollection::SystemShutdown() {
    // Drop all shared pointers
    m_ConfigServerHandlerCollection.reset();
    m_GatewayClientHandlerCollection.reset();
    for (auto l_HdlcdClientHandlerIterator = m_HdlcdClientHandlerVector.begin(); l_HdlcdClientHandlerIterator != m_HdlcdClientHandlerVector.end(); ++l_HdlcdClientHandlerIterator) {
        (*l_HdlcdClientHandlerIterator).reset();
    } // for
}

void HdlcdClientHandlerCollection::CleanAll() {
}

void HdlcdClientHandlerCollection::CreateHdlcdClient(uint16_t a_SerialPortNbr) {
}

void HdlcdClientHandlerCollection::DestroyHdlcdClient(uint16_t a_SerialPortNbr) {
}

void HdlcdClientHandlerCollection::SuspendHdlcdClient(uint16_t a_SerialPortNbr) {
}

void HdlcdClientHandlerCollection::ResumeHdlcdClient(uint16_t a_SerialPortNbr) {
}

void HdlcdClientHandlerCollection::SendPacket(uint16_t a_SerialPortNbr, const std::vector<unsigned char> &a_Buffer) {
}
