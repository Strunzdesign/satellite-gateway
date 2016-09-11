/**
 * \file      GatewayClientCleanup.h
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

#ifndef GATEWAY_CLIENT_CLEANUP_H
#define GATEWAY_CLIENT_CLEANUP_H

#include "ConfigFrame.h"
#include <memory>

class GatewayClientCleanup: public ConfigFrame {
public:
    static GatewayClientCleanup Create() {
        GatewayClientCleanup l_GatewayClientCleanup;
        return l_GatewayClientCleanup;
    }

    static std::shared_ptr<GatewayClientCleanup> CreateDeserializedFrame() {
        auto l_GatewayClientCleanup(std::shared_ptr<GatewayClientCleanup>(new GatewayClientCleanup));
        l_GatewayClientCleanup->m_eDeserialize = DESERIALIZE_BODY;
        l_GatewayClientCleanup->m_BytesRemaining = 1; // Next: consume the frame type byte
        return l_GatewayClientCleanup;
    }

private:
    // Private CTOR
    GatewayClientCleanup(): m_eDeserialize(DESERIALIZE_FULL) {
    }

    // Methods
    E_CONFIG_FRAME GetConfigFrameType() const { return CONFIG_FRAME_GATEWAY_CLIENT_CLEANUP; }

    // Serializer
    const std::vector<unsigned char> Serialize() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        std::vector<unsigned char> l_Buffer;
        l_Buffer.emplace_back(CONFIG_FRAME_GATEWAY_CLIENT_CLEANUP);
        return l_Buffer;
    }

    // Deserializer
    bool Deserialize() {
        // All requested bytes are available
        switch (m_eDeserialize) {
        case DESERIALIZE_BODY: {
            // Deserialize the frame type byte
            assert(m_Buffer.size() == 1);
            assert(m_Buffer[0] == CONFIG_FRAME_GATEWAY_CLIENT_CLEANUP);
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
    typedef enum {
        DESERIALIZE_ERROR = 0,
        DESERIALIZE_BODY  = 1,
        DESERIALIZE_FULL  = 2
    } E_DESERIALIZE;
    E_DESERIALIZE m_eDeserialize;
};

#endif // GATEWAY_CLIENT_CLEANUP_H
