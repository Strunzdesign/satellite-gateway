/**
 * \file      GatewayClientError.h
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

#ifndef GATEWAY_CLIENT_ERROR_H
#define GATEWAY_CLIENT_ERROR_H

#include "ConfigFrame.h"
#include <memory>

class GatewayClientError: public ConfigFrame {
public:
    static GatewayClientError Create(uint16_t a_ReferenceNbr, uint16_t a_ErrorCode) {
        GatewayClientError l_GatewayClientError;
        l_GatewayClientError.m_ReferenceNbr = a_ReferenceNbr;
        l_GatewayClientError.m_ErrorCode = a_ErrorCode;
        return l_GatewayClientError;
    }

    static std::shared_ptr<GatewayClientError> CreateDeserializedFrame() {
        auto l_GatewayClientError(std::shared_ptr<GatewayClientError>(new GatewayClientError));
        l_GatewayClientError->m_eDeserialize = DESERIALIZE_BODY;
        l_GatewayClientError->m_BytesRemaining = 5; // Next: read body including the frame type byte
        return l_GatewayClientError;
    }

    // Getter
    uint16_t GetReferenceNbr() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_ReferenceNbr;
    }

    uint16_t GetErrorCode() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_ErrorCode;
    }

private:
    // Private CTOR
    GatewayClientError(): m_ReferenceNbr(0), m_ErrorCode(0), m_eDeserialize(DESERIALIZE_FULL) {
    }
    
    // Methods
    E_CONFIG_FRAME GetConfigFrameType() const { return CONFIG_FRAME_GATEWAY_CLIENT_ERROR; }
    
    // Serializer
    const std::vector<unsigned char> Serialize() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        std::vector<unsigned char> l_Buffer;
        l_Buffer.emplace_back(CONFIG_FRAME_GATEWAY_CLIENT_ERROR);
        l_Buffer.emplace_back((m_ReferenceNbr >> 8) & 0xFF);
        l_Buffer.emplace_back((m_ReferenceNbr >> 0) & 0xFF);
        l_Buffer.emplace_back((m_ErrorCode    >> 8) & 0xFF);
        l_Buffer.emplace_back((m_ErrorCode    >> 0) & 0xFF);
        return l_Buffer;
    }
    
    // Members
    uint16_t m_ReferenceNbr;
    uint16_t m_ErrorCode;
    typedef enum {
        DESERIALIZE_ERROR = 0,
        DESERIALIZE_BODY  = 1,
        DESERIALIZE_FULL  = 2
    } E_DESERIALIZE;
    E_DESERIALIZE m_eDeserialize;
};

#endif // GATEWAY_CLIENT_ERROR_H
