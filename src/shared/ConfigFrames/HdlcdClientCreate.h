/**
 * \file      HdlcdClientCreate.h
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

#ifndef HDLCD_CLIENT_CREATE_H
#define HDLCD_CLIENT_CREATE_H

#include "ConfigFrame.h"
#include <memory>
#include <string>

class HdlcdClientCreate: public ConfigFrame {
public:
    static HdlcdClientCreate Create(const std::string &a_RemoteAddress, uint16_t a_TcpPortNbr, uint16_t a_SerialPortNbr) {
        HdlcdClientCreate l_HdlcdClientCreate;
        l_HdlcdClientCreate.m_RemoteAddress = a_RemoteAddress;
        l_HdlcdClientCreate.m_TcpPortNbr    = a_TcpPortNbr;
        l_HdlcdClientCreate.m_SerialPortNbr = a_SerialPortNbr;
        return l_HdlcdClientCreate;
    }

    static std::shared_ptr<HdlcdClientCreate> CreateDeserializedFrame() {
        auto l_HdlcdClientCreate(std::shared_ptr<HdlcdClientCreate>(new HdlcdClientCreate));
        l_HdlcdClientCreate->m_eDeserialize = DESERIALIZE_HEADER;
        l_HdlcdClientCreate->m_BytesRemaining = 6; // Next: read the header including the frame type byte
        return l_HdlcdClientCreate;
    }

    // Getter
    const std::string& GetRemoteAddress() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_RemoteAddress;
    }

    uint16_t GetTcpPortNbr() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_TcpPortNbr;
    }

    uint16_t GetSerialPortNbr() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_SerialPortNbr;
    }

private:
    // Private CTOR
    HdlcdClientCreate(): m_TcpPortNbr(0), m_SerialPortNbr(0), m_eDeserialize(DESERIALIZE_FULL) {
    }

    // Methods
    E_CONFIG_FRAME GetConfigFrameType() const { return CONFIG_FRAME_HDLCD_CLIENT_CREATE; }

    // Serializer
    const std::vector<unsigned char> Serialize() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        std::vector<unsigned char> l_Buffer;
        l_Buffer.emplace_back(CONFIG_FRAME_HDLCD_CLIENT_CREATE);
        l_Buffer.emplace_back((m_SerialPortNbr >>  8)  & 0xFF);
        l_Buffer.emplace_back((m_SerialPortNbr >>  0)  & 0xFF);
        l_Buffer.emplace_back((m_TcpPortNbr    >>  8)  & 0xFF);
        l_Buffer.emplace_back((m_TcpPortNbr    >>  0)  & 0xFF);
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
            assert(m_Buffer[0] == CONFIG_FRAME_HDLCD_CLIENT_CREATE);
            m_SerialPortNbr = ntohs(*(reinterpret_cast<const uint16_t*>(&m_Buffer[1])));
            m_TcpPortNbr    = ntohs(*(reinterpret_cast<const uint16_t*>(&m_Buffer[3])));
            m_BytesRemaining = m_Buffer[5];
            m_eDeserialize = DESERIALIZE_BODY;
            m_Buffer.clear();
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
    std::string m_RemoteAddress;
    uint16_t m_TcpPortNbr;
    uint16_t m_SerialPortNbr;
    typedef enum {
        DESERIALIZE_ERROR  = 0,
        DESERIALIZE_HEADER = 1,
        DESERIALIZE_BODY   = 2,
        DESERIALIZE_FULL   = 3
    } E_DESERIALIZE;
    E_DESERIALIZE m_eDeserialize;
};

#endif // HDLCD_CLIENT_CREATE_H
