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
#include <string>
class ConfigServerHandlerCollection;
class HdlcdClientHandlerCollection;
class GatewayFrame;
class GatewayClient;

class GatewayClientHandler {
public:
    // CTOR, initializer, and resetter
    GatewayClientHandler(boost::asio::io_service& a_IOService, std::shared_ptr<ConfigServerHandlerCollection> a_ConfigServerHandlerCollection,
                         std::shared_ptr<HdlcdClientHandlerCollection> a_HdlcdClientHandlerCollection, uint32_t a_ReferenceNbr,
                         std::string a_RemoteAddress, uint16_t a_RemotePortNbr);
    void Close();
    uint32_t GetReferenceNbr() const { return m_ReferenceNbr; }
    void GatewayFrameReceived(const std::shared_ptr<GatewayFrame> &a_GatewayFrame);
    
    // Methods to be called by a HDLCd client entity
    void SendPacket(uint16_t a_SerialPortNbr, const std::vector<unsigned char> &a_Payload);
    
private:
    // Members
    boost::asio::io_service& m_IOService;
    std::shared_ptr<ConfigServerHandlerCollection> m_ConfigServerHandlerCollection;
    std::shared_ptr<HdlcdClientHandlerCollection> m_HdlcdClientHandlerCollection;
    std::string m_RemoteAddress;
    uint16_t m_RemotePortNbr;

    std::shared_ptr<GatewayClient> m_GatewayClient;
    uint32_t m_ReferenceNbr;
};

#endif // GATEWAY_CLIENT_HANDLER_H
