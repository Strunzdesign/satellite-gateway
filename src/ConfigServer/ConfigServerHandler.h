/**
 * \file      ConfigServerHandler.h
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

#ifndef CONFIG_SERVER_HANDLER_H
#define CONFIG_SERVER_HANDLER_H

#include <memory>
#include <boost/asio.hpp>
class GatewayClientHandlerCollection;
class HdlcdClientHandlerCollection;
class ConfigFrame;
class ConfigServer;

class ConfigServerHandler {
public:
    // CTOR, initializer, and resetter
    ConfigServerHandler(boost::asio::io_service& a_IOService, std::shared_ptr<GatewayClientHandlerCollection> a_GatewayClientHandlerCollection,
                        std::shared_ptr<HdlcdClientHandlerCollection> a_HdlcdClientHandlerCollection);
    void Close();
    void ConfigFrameReceived(const std::shared_ptr<ConfigFrame> &a_ConfigFrame);
    
    // Methods to be called by a gateway client entity
    void GatewayClientCreated     (uint32_t a_ReferenceNbr);
    void GatewayClientDestroyed   (uint32_t a_ReferenceNbr);
    void GatewayClientConnected   (uint32_t a_ReferenceNbr);
    void GatewayClientDisconnected(uint32_t a_ReferenceNbr);
    void GatewayClientError       (uint32_t a_ReferenceNbr, uint16_t a_ErrorCode);
    
    // Methods to be called by a HDLCd client entity
    void HdlcdClientCreated    (uint16_t a_SerialPortNbr);
    void HdlcdClientDestroyed  (uint16_t a_SerialPortNbr);
    void HdlcdClientNewStatus  (uint16_t a_SerialPortNbr, bool a_bIsResumed, bool a_bIsAlive);
    void HdlcdClientError      (uint16_t a_SerialPortNbr, uint16_t a_ErrorCode);

private:
    // Members
    boost::asio::io_service& m_IOService;
    std::shared_ptr<GatewayClientHandlerCollection> m_GatewayClientHandlerCollection;
    std::shared_ptr<HdlcdClientHandlerCollection> m_HdlcdClientHandlerCollection;
    
    // One config server handler is responsible for one config server object
    std::shared_ptr<ConfigServer> m_ConfigServer;
};

#endif // CONFIG_SERVER_HANDLER_H
