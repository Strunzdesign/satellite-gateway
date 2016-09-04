/**
 * \file      GatewayClientHandlerCollection.h
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

#ifndef GATEWAY_CLIENT_HANDLER_COLLECTION_H
#define GATEWAY_CLIENT_HANDLER_COLLECTION_H

#include <memory>
#include <boost/asio.hpp>
#include <string>
class ConfigServerHandlerCollection;
class HdlcdClientHandlerCollection;
class GatewayClientHandler;

class GatewayClientHandlerCollection {
public:
    // CTOR, initializer, and resetter
    GatewayClientHandlerCollection(boost::asio::io_service& a_IOService);
    void Initialize(std::shared_ptr<ConfigServerHandlerCollection> a_ConfigServerHandlerCollection,
                    std::shared_ptr<HdlcdClientHandlerCollection>  a_HdlcdClientHandlerCollection);
    void SystemShutdown();
    
    // Methods to be called by a configuration server entity
    void CleanAll();    
    void CreateClient (uint32_t a_ReferenceNbr, std::string a_RemoteAddress, uint16_t a_RemotePortNbr);
    void DestroyClient(uint32_t a_ReferenceNbr);
    
    // Methods to be called by a HDLCd client entity
    void SendPacket(uint16_t a_SerialPortNbr, const std::vector<unsigned char> &a_Buffer);

private:
    // Members
    boost::asio::io_service& m_IOService;
    std::shared_ptr<ConfigServerHandlerCollection> m_ConfigServerHandlerCollection;
    std::shared_ptr<HdlcdClientHandlerCollection>  m_HdlcdClientHandlerCollection;
    
    // The gateway client handler entity
    std::shared_ptr<GatewayClientHandler> m_GatewayClientHandler;
};

#endif // GATEWAY_CLIENT_HANDLER_COLLECTION_H
