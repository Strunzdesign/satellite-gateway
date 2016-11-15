/**
 * \file      HdlcdClientHandler.h
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

#ifndef HDLCD_CLIENT_HANDLER_H
#define HDLCD_CLIENT_HANDLER_H

#include <memory>
#include <vector>
#include <string>
#include <boost/asio.hpp>
#include "HdlcdClientConnectGuard.h"
class ConfigServerHandlerCollection;
class GatewayClientHandlerCollection;
class HdlcdClient;

class HdlcdClientHandler {
public:
    HdlcdClientHandler(boost::asio::io_service& a_IOService, std::shared_ptr<ConfigServerHandlerCollection> a_ConfigServerHandlerCollection,
                       std::shared_ptr<GatewayClientHandlerCollection> a_GatewayClientHandlerCollection, const std::string& a_RemoteAddress, uint16_t a_TcpPortNbr, uint16_t a_SerialPortNbr);
    void Close();
    void Suspend();
    void Resume();
    void SendPacket(const std::vector<unsigned char> &a_Payload);
    
private:
    // Helpers
    void ResolveDestination();
    
    // Members
    boost::asio::io_service& m_IOService;
    std::shared_ptr<ConfigServerHandlerCollection>  m_ConfigServerHandlerCollection;
    std::shared_ptr<GatewayClientHandlerCollection> m_GatewayClientHandlerCollection;
    HdlcdClientConnectGuard m_HdlcdClientConnectGuard;
    
    const std::string m_RemoteAddress;
    const uint16_t m_TcpPortNbr;
    const uint16_t m_SerialPortNbr;
    
    // Resolver
    boost::asio::ip::tcp::resolver m_Resolver;
    boost::asio::deadline_timer m_ConnectionRetryTimer;
    
    // The connection to the HDLC Daemon
    std::shared_ptr<HdlcdClient> m_HdlcdClient;
    
    // Flags
    bool m_bSuspendSerialPort;
};

#endif // HDLCD_CLIENT_HANDLER_H
