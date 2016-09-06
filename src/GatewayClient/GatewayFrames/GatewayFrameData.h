/**
 * \file      GatewayFrameData..h
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

#ifndef GATEWAY_FRAME_DATA_H
#define GATEWAY_FRAME_DATA_H

#include "GatewayFrame.h"

class GatewayFrameData: public GatewayFrame {
public:
    // DTOR and creator
    GatewayFrameData(){}
    ~GatewayFrameData(){}
    static std::shared_ptr<GatewayFrameData> Create(uint16_t a_SerialPortNbr, const std::vector<unsigned char> &a_Payload) {
        auto l_GatewayFrameData = std::make_shared<GatewayFrameData>();
        l_GatewayFrameData->m_SerialPortNbr = a_SerialPortNbr;
        l_GatewayFrameData->m_Payload = std::move(a_Payload);
        return l_GatewayFrameData;
    }
    
    // Getter
    uint16_t GetSerialPortNbr() const { return m_SerialPortNbr; }
    const std::vector<unsigned char>& GetPayload() const { return m_Payload; }
    
private:
    // Methods
    E_GATEWAY_FRAME GetGatewayFrameType() const { return GATEWAY_FRAME_DATA; }
    
    // Members
    uint16_t m_SerialPortNbr;
    std::vector<unsigned char> m_Payload;
};

#endif // GATEWAY_FRAME_DATA_H
