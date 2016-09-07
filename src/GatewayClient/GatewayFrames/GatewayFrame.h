/**
 * \file      GatewayFrame.h
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

#ifndef GATEWAY_FRAME_H
#define GATEWAY_FRAME_H

#include <vector>
#include <assert.h>

typedef enum {
    GATEWAY_FRAME_UNKNOWN = 0x00,
    GATEWAY_FRAME_DATA    = 0x10
} E_GATEWAY_FRAME;

class GatewayFrame {
public:
    // CTOR and DTOR
    GatewayFrame(): m_BytesRemaining(0) {}
    virtual ~GatewayFrame(){} 
    
    virtual E_GATEWAY_FRAME GetGatewayFrameType() const { return GATEWAY_FRAME_UNKNOWN; }
    
    // Serializer and deserializer
    virtual const std::vector<unsigned char> Serialize() const = 0;
    size_t BytesNeeded() const { return m_BytesRemaining; }
    virtual bool BytesReceived(const unsigned char *a_ReadBuffer, size_t a_BytesRead) {
        // Checks
        assert(m_BytesRemaining);
        assert(m_BytesRemaining <= a_BytesRead);
        assert(a_BytesRead);
        m_Payload.insert(m_Payload.end(), a_ReadBuffer, (a_ReadBuffer + a_BytesRead));
        m_BytesRemaining -= a_BytesRead;
        return (m_BytesRemaining != 0); // true: subsequent bytes are required
    }
    
protected:
    std::vector<unsigned char> m_Payload;
    size_t m_BytesRemaining;
};

#endif // GATEWAY_FRAME_H
