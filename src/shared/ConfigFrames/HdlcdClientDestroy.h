/**
 * \file      HdlcdClientDestroy.h
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

#ifndef HDLCD_CLIENT_DESTROY_H
#define HDLCD_CLIENT_DESTROY_H

#include "ConfigFrame.h"
#include <memory>

class HdlcdClientDestroy: public ConfigFrame {
public:
    static HdlcdClientDestroy Create(uint16_t a_SerialPortNbr) {
        HdlcdClientDestroy l_HdlcdClientDestroy;
        l_HdlcdClientDestroy.m_SerialPortNbr = a_SerialPortNbr;
        return l_HdlcdClientDestroy;
    }

    static std::shared_ptr<HdlcdClientDestroy> CreateDeserializedFrame() {
        auto l_HdlcdClientDestroy(std::shared_ptr<HdlcdClientDestroy>(new HdlcdClientDestroy));
        l_HdlcdClientDestroy->m_eDeserialize = DESERIALIZE_BODY;
        l_HdlcdClientDestroy->m_BytesRemaining = 3; // Next: read body including the frame type byte
        return l_HdlcdClientDestroy;
    }

    // Getter
    uint16_t GetSerialPortNbr() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_SerialPortNbr;
    }

private:
    // Private CTOR
    HdlcdClientDestroy(): m_SerialPortNbr(0), m_eDeserialize(DESERIALIZE_FULL) {
    }

    // Methods
    E_CONFIG_FRAME GetConfigFrameType() const { return CONFIG_FRAME_HDLCD_CLIENT_DESTROY; }

    // Serializer
    const std::vector<unsigned char> Serialize() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        std::vector<unsigned char> l_Buffer;
        l_Buffer.emplace_back(CONFIG_FRAME_HDLCD_CLIENT_DESTROY);
        l_Buffer.emplace_back((m_SerialPortNbr >>  8) & 0xFF);
        l_Buffer.emplace_back((m_SerialPortNbr >>  0) & 0xFF);
        return l_Buffer;
    }

    // Deserializer
    bool Deserialize() {
        // All requested bytes are available
        switch (m_eDeserialize) {
        case DESERIALIZE_BODY: {
            // Deserialize the body including the frame type byte
            assert(m_Buffer.size() == 3);
            assert(m_Buffer[0] == CONFIG_FRAME_HDLCD_CLIENT_DESTROY);
            m_SerialPortNbr = ntohs(*(reinterpret_cast<const uint16_t*>(&m_Buffer[1])));
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
    uint16_t m_SerialPortNbr;
    typedef enum {
        DESERIALIZE_ERROR = 0,
        DESERIALIZE_BODY  = 1,
        DESERIALIZE_FULL  = 2
    } E_DESERIALIZE;
    E_DESERIALIZE m_eDeserialize;
};

#endif // HDLCD_CLIENT_DESTROY_H
