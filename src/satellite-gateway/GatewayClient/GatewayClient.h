/**
 * \file      GatewayClient.h
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

#ifndef GATEWAY_CLIENT_H
#define GATEWAY_CLIENT_H

#include <memory>
#include <boost/asio.hpp>
#include "GatewayFrame.h"
#include "FrameEndpoint.h"
#include "GatewayClientConnectGuard.h"
class ConfigServerHandlerCollection;
class HdlcdClientHandlerCollection;

class GatewayClient {
public:
    // CTOR and DTOR
    GatewayClient(boost::asio::io_service& a_IOService, boost::asio::ip::tcp::resolver::iterator a_EndpointIterator, std::shared_ptr<ConfigServerHandlerCollection> a_ConfigServerHandlerCollection,
                  std::shared_ptr<HdlcdClientHandlerCollection> a_HdlcdClientHandlerCollection, GatewayClientConnectGuard& a_GatewayClientConnectGuard, uint16_t a_ReferenceNbr);
    ~GatewayClient();

    void Shutdown();
    void Close();

    // Methods to be called by a HDLCd client entity
    void SendPacket(uint16_t a_SerialPortNbr, const std::vector<unsigned char> &a_Payload);
    
    void SetOnClosedCallback(std::function<void()> a_OnClosedCallback);
    
private:
    // Internal helpers
    bool OnFrame(std::shared_ptr<Frame> a_Frame);
    void OnClosed();

    // Members
    boost::asio::io_service& m_IOService;
    std::shared_ptr<ConfigServerHandlerCollection> m_ConfigServerHandlerCollection;
    std::shared_ptr<HdlcdClientHandlerCollection> m_HdlcdClientHandlerCollection;
    GatewayClientConnectGuard m_GatewayClientConnectGuard;
    
    // The communication end point
    boost::asio::ip::tcp::socket m_TcpSocket;
    std::shared_ptr<FrameEndpoint> m_FrameEndpoint;
    uint16_t m_ReferenceNbr;
    bool m_bClosed;
    
    std::function<void()> m_OnClosedCallback;
};

#endif // GATEWAY_CLIENT_H
