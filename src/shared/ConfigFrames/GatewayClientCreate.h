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
#include <memory>
#include <string>

class GatewayClientCreate: public ConfigFrame {
public:
    static GatewayClientCreate Create(uint16_t a_ReferenceNbr, std::string a_RemoteAddress, uint16_t a_RemotePortNbr) {
        GatewayClientCreate l_GatewayClientCreate;
        l_GatewayClientCreate.m_ReferenceNbr  = a_ReferenceNbr;
        l_GatewayClientCreate.m_RemoteAddress = a_RemoteAddress;
        l_GatewayClientCreate.m_RemotePortNbr = a_RemotePortNbr;
        return l_GatewayClientCreate;
    }

    static std::shared_ptr<GatewayClientCreate> CreateDeserializedFrame() {
        auto l_GatewayClientCreate(std::shared_ptr<GatewayClientCreate>(new GatewayClientCreate));
        l_GatewayClientCreate->m_eDeserialize = DESERIALIZE_HEADER;
        l_GatewayClientCreate->m_BytesRemaining = 6; // Next: read the header including the frame type byte
        return l_GatewayClientCreate;
    }

    // Getter
    uint16_t GetReferenceNbr() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_ReferenceNbr;
    }

    std::string GetRemoteAddress() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_RemoteAddress;
    }

    uint16_t GetRemotePortNbr() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_RemotePortNbr;
    }

private:
    // Private CTOR
    GatewayClientCreate(): m_ReferenceNbr(0), m_RemotePortNbr(0), m_eDeserialize(DESERIALIZE_FULL) {
    }
    
    // Methods
    E_CONFIG_FRAME GetConfigFrameType() const { return CONFIG_FRAME_GATEWAY_CLIENT_CREATE; }

    // Serializer
    const std::vector<unsigned char> Serialize() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        std::vector<unsigned char> l_Buffer;
        l_Buffer.emplace_back(CONFIG_FRAME_GATEWAY_CLIENT_CREATE);
        l_Buffer.emplace_back((m_ReferenceNbr  >> 8)   & 0xFF);
        l_Buffer.emplace_back((m_ReferenceNbr  >> 0)   & 0xFF);
        l_Buffer.emplace_back((m_RemotePortNbr >> 8)   & 0xFF);
        l_Buffer.emplace_back((m_RemotePortNbr >> 0)   & 0xFF);
        l_Buffer.emplace_back((m_RemoteAddress.size()) & 0xFF);
        l_Buffer.insert(l_Buffer.end(), m_RemoteAddress.data(), (m_RemoteAddress.data() + m_RemoteAddress.size()));
        return l_Buffer;
    }

    // Deserializer
    bool Deserialize() {
        // All requested bytes are available
        switch (m_eDeserialize) {
        case DESERIALIZE_HEADER: {
            // Deserialize the body including the frame type byte
            assert(m_Buffer.size() == 6);
            assert(m_Buffer[0] == CONFIG_FRAME_GATEWAY_CLIENT_CREATE);
            m_ReferenceNbr   = ntohs(*(reinterpret_cast<const uint16_t*>(&m_Buffer[1])));
            m_RemotePortNbr  = ntohs(*(reinterpret_cast<const uint16_t*>(&m_Buffer[3])));
            m_BytesRemaining = m_Buffer[5];
            m_Buffer.clear();
            if (m_BytesRemaining) {
                m_eDeserialize = DESERIALIZE_BODY;
            } else {
                m_eDeserialize = DESERIALIZE_FULL;
            } // else

            break;
        }
        case DESERIALIZE_BODY: {
            m_RemoteAddress.append(m_Buffer.begin(), m_Buffer.end());
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
    std::string m_RemoteAddress;
    uint16_t m_RemotePortNbr;
    typedef enum {
        DESERIALIZE_ERROR  = 0,
        DESERIALIZE_HEADER = 1,
        DESERIALIZE_BODY   = 2,
        DESERIALIZE_FULL   = 3
    } E_DESERIALIZE;
    E_DESERIALIZE m_eDeserialize;
};

#endif // GATEWAY_CLIENT_CREATE_H
