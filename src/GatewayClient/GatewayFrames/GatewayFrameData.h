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
#include <assert.h>

class GatewayFrameData: public GatewayFrame {
public:
    static GatewayFrameData Create(uint16_t a_SerialPortNbr, const std::vector<unsigned char> &a_Payload) {
        GatewayFrameData l_GatewayFrameData;
        l_GatewayFrameData.m_SerialPortNbr = a_SerialPortNbr;
        l_GatewayFrameData.m_Payload = std::move(a_Payload);
        return l_GatewayFrameData;
    }
    
    static std::shared_ptr<GatewayFrameData> CreateDeserializedFrame() {
        auto l_GatewayFrameData(std::shared_ptr<GatewayFrameData>(new GatewayFrameData));
        l_GatewayFrameData->m_eDeserialize = DESERIALIZE_SIZE;
        l_GatewayFrameData->m_BytesRemaining = 2; // Next: read length field
        return l_GatewayFrameData;
    }
    
    // Getter
    uint16_t GetSerialPortNbr() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_SerialPortNbr;
    }
    
    const std::vector<unsigned char>& GetPayload() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_Payload;
    }
    
private:
    // Private CTOR
    GatewayFrameData() {
        m_SerialPortNbr = 0;
        m_eDeserialize = DESERIALIZE_FULL;
        m_PayloadLength = 0; // only for the deserializer
        m_BytesRemaining = 0;
    }
    
    // Internal helpers
    E_GATEWAY_FRAME GetGatewayFrameType() const { return GATEWAY_FRAME_DATA; }
    
    // Serializer and deserializer
    const std::vector<unsigned char> Serialize() const {
//        assert(m_eDeserialize == DESERIALIZE_FULL);
//        assert(m_BytesRemaining == 0);
        std::vector<unsigned char> l_Buffer;
        
        // Prepare length field, serial port number, and payload
        uint16_t l_NbrOfBytes = (2 + m_Payload.size());
        l_Buffer.emplace_back((l_NbrOfBytes    >> 8) & 0x00FF);
        l_Buffer.emplace_back((l_NbrOfBytes    >> 0) & 0x00FF);
        l_Buffer.emplace_back((m_SerialPortNbr >> 8) & 0x00FF);
        l_Buffer.emplace_back((m_SerialPortNbr >> 0) & 0x00FF);
        l_Buffer.insert(l_Buffer.end(), m_Payload.begin(), m_Payload.end());
        return l_Buffer;
    }
    
    size_t BytesNeeded() const {
        return m_BytesRemaining;
    }
    
    bool BytesReceived(const unsigned char *a_ReadBuffer, size_t a_BytesRead) {
        switch (m_eDeserialize) {
        case DESERIALIZE_SIZE: {
            // Read length field
            assert((m_BytesRemaining > 0) && (m_BytesRemaining <= 2));
            assert(a_BytesRead);
            assert(a_BytesRead == 2); // TODO: dirty, might break!
            assert(m_Payload.empty());
            m_PayloadLength  = ntohs(*(reinterpret_cast<const uint16_t*>(a_ReadBuffer)));
            m_BytesRemaining = 2; // Next: read serial port number field
            m_eDeserialize   = DESERIALIZE_PORT;
            break;
        }
        case DESERIALIZE_PORT: {
            // Read length field
            assert((m_BytesRemaining > 0) && (m_BytesRemaining <= 2));
            assert(a_BytesRead);
            assert(a_BytesRead == 2); // TODO: dirty, might break!
            assert(m_Payload.empty());
            m_SerialPortNbr = ntohs(*(reinterpret_cast<const uint16_t*>(a_ReadBuffer)));
            m_BytesRemaining = (m_PayloadLength - 2); // Next: read data // TODO; check is missing
            m_eDeserialize = DESERIALIZE_DATA;
            break;
        }
        case DESERIALIZE_DATA: {
            // Read payload
            assert(m_BytesRemaining <= a_BytesRead);
            assert(a_BytesRead);
            m_Payload.insert(m_Payload.end(), a_ReadBuffer, (a_ReadBuffer + a_BytesRead));
            m_BytesRemaining -= a_BytesRead;
            if (m_BytesRemaining == 0) {
                m_eDeserialize = DESERIALIZE_FULL;
            } // if

            break;
        }
        case DESERIALIZE_FULL:
        default:
            assert(false);
        } // switch
        
        return (true); // no error
    }
    
    // Members
    uint16_t m_SerialPortNbr;
    std::vector<unsigned char> m_Payload;
    typedef enum {
        DESERIALIZE_SIZE = 0,
        DESERIALIZE_PORT = 1,
        DESERIALIZE_DATA = 2,
        DESERIALIZE_FULL = 3
    } E_DESERIALIZE;
    E_DESERIALIZE m_eDeserialize;
    size_t m_PayloadLength;
    size_t m_BytesRemaining;    
};

#endif // GATEWAY_FRAME_DATA_H
