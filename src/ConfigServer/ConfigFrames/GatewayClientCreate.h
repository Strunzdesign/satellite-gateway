/**
 * \file      GatewayClientCreate.h
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

#ifndef GATEWAY_CLIENT_CREATE_H
#define GATEWAY_CLIENT_CREATE_H

#include "ConfigFrame.h"
#include <string>

class GatewayClientCreate: public ConfigFrame {
public:
    // DTOR and creator
    GatewayClientCreate(){}
    ~GatewayClientCreate(){}
    static std::shared_ptr<GatewayClientCreate> Create(uint32_t a_ReferenceNbr, std::string a_RemoteAddress, uint16_t a_RemotePortNbr) {
        auto l_GatewayClientCreate = std::make_shared<GatewayClientCreate>();
        l_GatewayClientCreate->m_ReferenceNbr = a_ReferenceNbr;
        l_GatewayClientCreate->m_RemoteAddress = a_RemoteAddress;
        l_GatewayClientCreate->m_RemotePortNbr = a_RemotePortNbr;
        return l_GatewayClientCreate;
    }
    
    // Getter
    uint16_t GetReferenceNbr() const { return m_ReferenceNbr; }
    std::string GetRemoteAddress() const { return m_RemoteAddress; }
    uint16_t GetRemotePortNbr() const { return m_RemotePortNbr; }
    
private:
    // Methods
    E_CONFIG_FRAME GetConfigFrameType() const { return CONFIG_FRAME_GATEWAY_CLIENT_CREATE; }
    
    // Members
    uint32_t m_ReferenceNbr;
    std::string m_RemoteAddress;
    uint16_t m_RemotePortNbr;
};

#endif // GATEWAY_CLIENT_CREATE_H
