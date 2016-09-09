/**
 * \file      GatewayClientConnected.h
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

#ifndef GATEWAY_CLIENT_CONNECTED_H
#define GATEWAY_CLIENT_CONNECTED_H

#include "ConfigFrame.h"
#include <memory>

class GatewayClientConnected: public ConfigFrame {
public:
    static GatewayClientConnected Create(uint32_t a_ReferenceNbr) {
        GatewayClientConnected l_GatewayClientConnected;
        l_GatewayClientConnected.m_ReferenceNbr = a_ReferenceNbr;
        return l_GatewayClientConnected;
    }

    static std::shared_ptr<GatewayClientConnected> CreateDeserializedFrame() {
        auto l_GatewayClientConnected(std::shared_ptr<GatewayClientConnected>(new GatewayClientConnected));
        l_GatewayClientConnected->m_eDeserialize = DESERIALIZE_BODY;
        l_GatewayClientConnected->m_BytesRemaining = 5; // Next: read body including the frame type byte
        return l_GatewayClientConnected;
    }

    // Getter
    uint16_t GetReferenceNbr() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_ReferenceNbr;
    }

private:
    // Private CTOR
    GatewayClientConnected(): m_ReferenceNbr(0), m_eDeserialize(DESERIALIZE_FULL) {
    }

    // Methods
    E_CONFIG_FRAME GetConfigFrameType() const { return CONFIG_FRAME_GATEWAY_CLIENT_CONNECTED; }
    
    // Serializer
    const std::vector<unsigned char> Serialize() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        std::vector<unsigned char> l_Buffer;
        l_Buffer.emplace_back(CONFIG_FRAME_GATEWAY_CLIENT_CONNECTED);
        l_Buffer.emplace_back((m_ReferenceNbr >> 24) & 0xFF);
        l_Buffer.emplace_back((m_ReferenceNbr >> 16) & 0xFF);
        l_Buffer.emplace_back((m_ReferenceNbr >>  8) & 0xFF);
        l_Buffer.emplace_back((m_ReferenceNbr >>  0) & 0xFF);
        return l_Buffer;
    }
    
    // Members
    uint32_t m_ReferenceNbr;
    typedef enum {
        DESERIALIZE_ERROR = 0,
        DESERIALIZE_BODY  = 1,
        DESERIALIZE_FULL  = 2
    } E_DESERIALIZE;
    E_DESERIALIZE m_eDeserialize;
};

#endif // GATEWAY_CLIENT_CONNECTED_H
