/**
 * \file      GatewayClientDisconnected.h
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

#ifndef GATEWAY_CLIENT_DISCONNECTED_H
#define GATEWAY_CLIENT_DISCONNECTED_H

#include "ConfigFrame.h"
#include <memory>

class GatewayClientDisconnected: public ConfigFrame {
public:
    static GatewayClientDisconnected Create(uint16_t a_ReferenceNbr) {
        GatewayClientDisconnected l_GatewayClientDisconnected;
        l_GatewayClientDisconnected.m_ReferenceNbr = a_ReferenceNbr;
        return l_GatewayClientDisconnected;
    }

    static std::shared_ptr<GatewayClientDisconnected> CreateDeserializedFrame() {
        auto l_GatewayClientDisconnected(std::shared_ptr<GatewayClientDisconnected>(new GatewayClientDisconnected));
        l_GatewayClientDisconnected->m_eDeserialize = DESERIALIZE_BODY;
        l_GatewayClientDisconnected->m_BytesRemaining = 3; // Next: read body including the frame type byte
        return l_GatewayClientDisconnected;
    }

    // Getter
    uint16_t GetReferenceNbr() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_ReferenceNbr;
    }

private:
    // Private CTOR
    GatewayClientDisconnected(): m_ReferenceNbr(0), m_eDeserialize(DESERIALIZE_FULL) {
    }

    // Methods
    E_CONFIG_FRAME GetConfigFrameType() const { return CONFIG_FRAME_GATEWAY_CLIENT_DISCONNECTED; }

    // Serializer
    const std::vector<unsigned char> Serialize() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        std::vector<unsigned char> l_Buffer;
        l_Buffer.emplace_back(CONFIG_FRAME_GATEWAY_CLIENT_DISCONNECTED);
        l_Buffer.emplace_back((m_ReferenceNbr >> 8) & 0xFF);
        l_Buffer.emplace_back((m_ReferenceNbr >> 0) & 0xFF);
        return l_Buffer;
    }

    // Deserializer
    bool Deserialize() {
        // All requested bytes are available
        switch (m_eDeserialize) {
        case DESERIALIZE_BODY: {
            // Deserialize the body including the frame type byte
            assert(m_Buffer.size() == 3);
            assert(m_Buffer[0] == CONFIG_FRAME_GATEWAY_CLIENT_DISCONNECTED);
            m_ReferenceNbr = ntohs(*(reinterpret_cast<const uint16_t*>(&m_Buffer[1])));
            m_eDeserialize = DESERIALIZE_FULL;
            break;
        }
        case DESERIALIZE_ERROR:
        case DESERIALIZE_FULL:
        default:
            assert(false);
        } // switch

        // No error
        return true;
    }

    // Members
    uint16_t m_ReferenceNbr;
    typedef enum {
        DESERIALIZE_ERROR = 0,
        DESERIALIZE_BODY  = 1,
        DESERIALIZE_FULL  = 2
    } E_DESERIALIZE;
    E_DESERIALIZE m_eDeserialize;
};

#endif // GATEWAY_CLIENT_DISCONNECTED_H
