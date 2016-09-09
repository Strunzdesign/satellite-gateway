/**
 * \file      HdlcdClientCreated.h
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

#ifndef HDLCD_CLIENT_CREATED_H
#define HDLCD_CLIENT_CREATED_H

#include "ConfigFrame.h"
#include <memory>

class HdlcdClientCreated: public ConfigFrame {
public:
    static HdlcdClientCreated Create(uint16_t a_SerialPortNbr) {
        HdlcdClientCreated l_HdlcdClientCreated;
        l_HdlcdClientCreated.m_SerialPortNbr = a_SerialPortNbr;
        return l_HdlcdClientCreated;
    }

    static std::shared_ptr<HdlcdClientCreated> CreateDeserializedFrame() {
        auto l_HdlcdClientCreated(std::shared_ptr<HdlcdClientCreated>(new HdlcdClientCreated));
        l_HdlcdClientCreated->m_eDeserialize = DESERIALIZE_BODY;
        l_HdlcdClientCreated->m_BytesRemaining = 3; // Next: read body including the frame type byte
        return l_HdlcdClientCreated;
    }

    // Getter
    uint16_t GetSerialPortNbr() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_SerialPortNbr;
    }

private:
    // Private CTOR
    HdlcdClientCreated(): m_SerialPortNbr(0), m_eDeserialize(DESERIALIZE_FULL) {
    }
    
    // Methods
    E_CONFIG_FRAME GetConfigFrameType() const { return CONFIG_FRAME_HDLCD_CLIENT_CREATED; }

    // Serializer
    const std::vector<unsigned char> Serialize() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        std::vector<unsigned char> l_Buffer;
        l_Buffer.emplace_back(CONFIG_FRAME_HDLCD_CLIENT_CREATED);
        l_Buffer.emplace_back((m_SerialPortNbr >>  8) & 0xFF);
        l_Buffer.emplace_back((m_SerialPortNbr >>  0) & 0xFF);
        return l_Buffer;
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

#endif // HDLCD_CLIENT_CREATED_H
