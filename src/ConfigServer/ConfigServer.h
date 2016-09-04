/**
 * \file      ConfigServer.h
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

#ifndef CONFIG_SERVER_H
#define CONFIG_SERVER_H

#include <memory>
#include <boost/asio.hpp>
class GatewayClientHandlerCollection;
class HdlcdClientHandlerCollection;
class ConfigFrame;

class ConfigServer {
public:
    // CTOR, initializer, and resetter
    ConfigServer(boost::asio::io_service& a_IOService);
    void Initialize(std::shared_ptr<GatewayClientHandlerCollection> a_GatewayClientHandlerCollection,
                    std::shared_ptr<HdlcdClientHandlerCollection> a_HdlcdClientHandlerCollection);
    void Close();
    
    void SendControlPacket(std::shared_ptr<ConfigFrame> a_ConfigFrame);
    void SendOnControlPacketCallback(std::function<void()> a_OnControlPacketCallback = std::function<void()>());

private:
    // Members
    boost::asio::io_service& m_IOService;
    std::shared_ptr<GatewayClientHandlerCollection> m_GatewayClientHandlerCollection;
    std::shared_ptr<HdlcdClientHandlerCollection> m_HdlcdClientHandlerCollection;
    
    std::function<void()> m_OnControlPacketCallback;
};

#endif // CONFIG_SERVER_H
