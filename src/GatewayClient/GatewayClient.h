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
#include "GatewayFrames/GatewayFrame.h"
class ConfigServerHandlerCollection;
class HdlcdClientHandlerCollection;

class GatewayClient {
public:
    // CTOR
    GatewayClient(boost::asio::io_service& a_IOService, std::shared_ptr<ConfigServerHandlerCollection> a_ConfigServerHandlerCollection,
                  std::shared_ptr<HdlcdClientHandlerCollection> a_HdlcdClientHandlerCollection);
    void Close();

    // Methods to be called by a HDLCd client entity
    void SendGatewayFrame(const GatewayFrame& a_GatewayFrame);
    
private:
    // Members
    boost::asio::io_service& m_IOService;
    std::shared_ptr<ConfigServerHandlerCollection> m_ConfigServerHandlerCollection;
    std::shared_ptr<HdlcdClientHandlerCollection> m_HdlcdClientHandlerCollection;
};

#endif // GATEWAY_CLIENT_H
