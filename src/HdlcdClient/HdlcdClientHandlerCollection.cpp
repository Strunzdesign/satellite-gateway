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

void HdlcdClientHandlerCollection::Initialize(std::shared_ptr<ConfigServerHandlerCollection>  a_ConfigServerHandlerCollection,
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
    for (auto l_HandlerIterator = m_HdlcdClientHandlerMap.begin(); l_HandlerIterator != m_HdlcdClientHandlerMap.end(); ++l_HandlerIterator) {
        auto& l_Handler = l_HandlerIterator->second;
        l_Handler->Close();
        l_Handler.reset();
    } // for
}

void HdlcdClientHandlerCollection::CleanAll() {
    // Silently remove all previous hdlcd client entites
    for (auto l_HandlerIterator = m_HdlcdClientHandlerMap.begin(); l_HandlerIterator != m_HdlcdClientHandlerMap.end(); ++l_HandlerIterator) {
        auto& l_Handler = l_HandlerIterator->second;
        l_Handler->Close();
        l_Handler.reset();
    } // for
}

void HdlcdClientHandlerCollection::CreateHdlcdClient(const std::string &a_DestinationName, uint16_t a_TcpPortNbr, uint16_t a_SerialPortNbr) {
    // First check whether there is already a client handler for the specified serial port
    if (m_HdlcdClientHandlerMap.find(a_SerialPortNbr) == m_HdlcdClientHandlerMap.end()) {
        // The element did not exist yet, create it
        auto l_NewHdlcdClientHandler = std::make_shared<HdlcdClientHandler>(m_IOService, m_ConfigServerHandlerCollection, m_GatewayClientHandlerCollection,
                                                                            a_DestinationName, a_TcpPortNbr, a_SerialPortNbr);
        m_HdlcdClientHandlerMap[a_SerialPortNbr] = l_NewHdlcdClientHandler;
    } else {
        // The client handler already existed
        m_ConfigServerHandlerCollection->HdlcdClientError(a_SerialPortNbr, 0x00);
    } // else
    
    // Deliver the "created" message in each case
    m_ConfigServerHandlerCollection->HdlcdClientCreated(a_SerialPortNbr);
}

void HdlcdClientHandlerCollection::DestroyHdlcdClient(uint16_t a_SerialPortNbr) {
    // First check whether there is a client handler for the specified serial port
    auto l_HandlerIterator = m_HdlcdClientHandlerMap.find(a_SerialPortNbr);
    if (l_HandlerIterator == m_HdlcdClientHandlerMap.end()) {
        // The element did not exist
        m_ConfigServerHandlerCollection->HdlcdClientError(a_SerialPortNbr, 0x00);
    } else {
        // The client handler exists
        l_HandlerIterator->second->Close();
        l_HandlerIterator->second.reset();
        m_HdlcdClientHandlerMap.erase(l_HandlerIterator);
    } // else
    
    // Deliver the "destroyed" message in each case
    m_ConfigServerHandlerCollection->HdlcdClientDestroyed(a_SerialPortNbr);
}

void HdlcdClientHandlerCollection::SuspendHdlcdClient(uint16_t a_SerialPortNbr) {
    // First check whether there is a client handler for the specified serial port
    auto l_HandlerIterator = m_HdlcdClientHandlerMap.find(a_SerialPortNbr);
    if (l_HandlerIterator == m_HdlcdClientHandlerMap.end()) {
        // The element did not exist
        m_ConfigServerHandlerCollection->HdlcdClientError(a_SerialPortNbr, 0x00);
    } else {
        // The client handler exists
        l_HandlerIterator->second->Suspend();
    } // else
}

void HdlcdClientHandlerCollection::ResumeHdlcdClient(uint16_t a_SerialPortNbr) {
    // First check whether there is a client handler for the specified serial port
    auto l_HandlerIterator = m_HdlcdClientHandlerMap.find(a_SerialPortNbr);
    if (l_HandlerIterator == m_HdlcdClientHandlerMap.end()) {
        // The element did not exist
        m_ConfigServerHandlerCollection->HdlcdClientError(a_SerialPortNbr, 0x00);
    } else {
        // The client handler exists
        l_HandlerIterator->second->Resume();
    } // else
}

void HdlcdClientHandlerCollection::SendPacket(uint16_t a_SerialPortNbr, const std::vector<unsigned char> &a_Payload) {
    // First check whether there is a client handler for the specified serial port
    auto l_HandlerIterator = m_HdlcdClientHandlerMap.find(a_SerialPortNbr);
    if (l_HandlerIterator != m_HdlcdClientHandlerMap.end()) {
        // The client handler exists
        l_HandlerIterator->second->SendPacket(a_Payload);
    } else {
        // Do not emit an error message... may be useful, but may also clutter the logs
    } // else
}
