/**
 * \file      GatewayClientHandler.h
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

#ifndef GATEWAY_CLIENT_HANDLER_H
#define GATEWAY_CLIENT_HANDLER_H

#include <memory>
#include <boost/asio.hpp>
class ConfigurationServerHandler;
class HdlcdClientHandlerCollection;

class GatewayClientHandler {
public:
    // CTOR, initializer, and resetter
    GatewayClientHandler(boost::asio::io_service& a_IOService);
    void Initialize(std::shared_ptr<ConfigurationServerHandler> a_ConfigurationServerHandler,
                    std::shared_ptr<HdlcdClientHandlerCollection> a_HdlcdClientHandlerCollection);
    void Reset();
    
    // Methods to be called by a configuration server entity
    void Connect(uint32_t a_ReferenceNbr);
    void Disconnect();
    
    // Methods to be called by a HDLCd client entity
    void SendPacket(uint16_t a_SerialPortNbr, const std::vector<unsigned char> &a_Buffer);
    
private:
    // Members
    boost::asio::io_service& m_IOService;
    std::shared_ptr<ConfigurationServerHandler> m_ConfigurationServerHandler;
    std::shared_ptr<HdlcdClientHandlerCollection> m_HdlcdClientHandlerCollection;
};

#endif // GATEWAY_CLIENT_HANDLER_H
