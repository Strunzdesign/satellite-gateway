/**
 * \file      GatewayClientCreated.h
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

#ifndef GATEWAY_CLIENT_CREATED_H
#define GATEWAY_CLIENT_CREATED_H

#include "ConfigFrame.h"

class GatewayClientCreated: public ConfigFrame {
public:
    // DTOR and creator
    GatewayClientCreated(){}
    ~GatewayClientCreated(){}
    static std::shared_ptr<GatewayClientCreated> Create(uint32_t a_ReferenceNbr) {
        auto l_GatewayClientCreated = std::make_shared<GatewayClientCreated>();
        l_GatewayClientCreated->m_ReferenceNbr = a_ReferenceNbr;
        return l_GatewayClientCreated;
    }
    
    uint16_t GetReferenceNbr() const { return m_ReferenceNbr; }
    
private:
    // Methods
    E_CONFIG_FRAME GetConfigFrameType() const { return CONFIG_FRAME_GATEWAY_CLIENT_CREATED; }
    
    // Members
    uint32_t m_ReferenceNbr;
};

#endif // GATEWAY_CLIENT_CREATED_H
