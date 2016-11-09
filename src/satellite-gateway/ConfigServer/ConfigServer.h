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
#include "ConfigFrame.h"
#include "FrameEndpoint.h"
#include "GatewayClientErrorCodes.h"
#include "HdlcdClientErrorCodes.h"
class GatewayClientHandlerCollection;
class HdlcdClientHandlerCollection;

class ConfigServer {
public:
    // CTOR, initializer, and resetter
    ConfigServer(boost::asio::io_service& a_IOService, boost::asio::ip::tcp::tcp::socket& a_TcpSocket);
    void SetOnClosedCallback(std::function<void()> a_OnClosedCallback) {
        m_OnClosedCallback = a_OnClosedCallback;
    }
    
    void Start(std::shared_ptr<GatewayClientHandlerCollection> a_GatewayClientHandlerCollection,
               std::shared_ptr<HdlcdClientHandlerCollection>   a_HdlcdClientHandlerCollection);
    void Close();
    
    // Methods to be called by a gateway client entity
    void GatewayClientCreated     (uint16_t a_ReferenceNbr);
    void GatewayClientDestroyed   (uint16_t a_ReferenceNbr);
    void GatewayClientConnected   (uint16_t a_ReferenceNbr);
    void GatewayClientDisconnected(uint16_t a_ReferenceNbr);
    void GatewayClientError       (uint16_t a_ReferenceNbr, E_GATEWAY_CLIENT_ERROR a_ErrorCode);
    
    // Methods to be called by a HDLCd client entity
    void HdlcdClientCreated     (uint16_t a_SerialPortNbr);
    void HdlcdClientDestroyed   (uint16_t a_SerialPortNbr);
    void HdlcdClientConnected   (uint16_t a_SerialPortNbr);
    void HdlcdClientDisconnected(uint16_t a_SerialPortNbr);
    void HdlcdClientNewStatus   (uint16_t a_SerialPortNbr, bool a_bIsResumed, bool a_bIsAlive);
    void HdlcdClientError       (uint16_t a_SerialPortNbr, E_HDLCD_CLIENT_ERROR a_ErrorCode);

private:
    // Internal helpers
    bool OnFrame(std::shared_ptr<Frame> a_Frame);
    void OnClosed();
    
    // Members
    boost::asio::io_service& m_IOService;
    std::shared_ptr<GatewayClientHandlerCollection> m_GatewayClientHandlerCollection;
    std::shared_ptr<HdlcdClientHandlerCollection> m_HdlcdClientHandlerCollection;

    // The communication end point
    std::shared_ptr<FrameEndpoint> m_FrameEndpoint;
    std::function<void()> m_OnClosedCallback;
};

#endif // CONFIG_SERVER_H
