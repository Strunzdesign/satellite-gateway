/**
 * \file      HdlcdClientDeviceNewStatus.h
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

#ifndef HDLCD_CLIENT_DEVICE_NEW_STATUS_H
#define HDLCD_CLIENT_DEVICE_NEW_STATUS_H

#include "ConfigFrame.h"
#include <memory>

class HdlcdClientNewStatus: public ConfigFrame {
public:
    static HdlcdClientNewStatus Create(uint16_t a_SerialPortNbr, bool a_bIsResumed, bool a_bIsAlive) {
        HdlcdClientNewStatus l_HdlcdClientNewStatus;
        l_HdlcdClientNewStatus.m_SerialPortNbr = a_SerialPortNbr;
        l_HdlcdClientNewStatus.m_bIsResumed = a_bIsResumed;
        l_HdlcdClientNewStatus.m_bIsAlive = a_bIsAlive;
        return l_HdlcdClientNewStatus;
    }

    static std::shared_ptr<HdlcdClientNewStatus> CreateDeserializedFrame() {
        auto l_HdlcdClientNewStatus(std::shared_ptr<HdlcdClientNewStatus>(new HdlcdClientNewStatus));
        l_HdlcdClientNewStatus->m_eDeserialize = DESERIALIZE_BODY;
        l_HdlcdClientNewStatus->m_BytesRemaining = 4; // Next: read body including the frame type byte
        return l_HdlcdClientNewStatus;
    }

    // Getter
    uint16_t GetSerialPortNbr() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_SerialPortNbr;
    }

    uint16_t GetIsResumed() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_bIsResumed;
    }

    uint16_t GetIsAlive() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_bIsAlive;
    }

private:
    // Private CTOR
    HdlcdClientNewStatus(): m_SerialPortNbr(0), m_bIsResumed(false), m_bIsAlive(false), m_eDeserialize(DESERIALIZE_FULL) {
    }

    // Methods
    E_CONFIG_FRAME GetConfigFrameType() const { return CONFIG_FRAME_HDLCD_CLIENT_NEW_STATUS; }

    // Serializer
    const std::vector<unsigned char> Serialize() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        std::vector<unsigned char> l_Buffer;
        l_Buffer.emplace_back(CONFIG_FRAME_HDLCD_CLIENT_NEW_STATUS);
        l_Buffer.emplace_back((m_SerialPortNbr >> 8) & 0xFF);
        l_Buffer.emplace_back((m_SerialPortNbr >> 0) & 0xFF);
        unsigned char l_Control = 0x00;
        if (m_bIsResumed) {
            l_Control |= 0x01;
        } // if
        
        if (m_bIsAlive) {
            l_Control |= 0x02;
        } // if

        l_Buffer.emplace_back(l_Control);
        return l_Buffer;
    }

    // Deserializer
    bool BytesReceived(const unsigned char *a_ReadBuffer, size_t a_BytesRead) {
        if (Frame::BytesReceived(a_ReadBuffer, a_BytesRead)) {
            // Subsequent bytes are required
            return true; // no error (yet)
        } // if

        // All requested bytes are available
        switch (m_eDeserialize) {
        case DESERIALIZE_BODY: {
            // Deserialize the body including the frame type byte
            assert(m_Payload.size() == 4);
            assert(m_Payload[0] == CONFIG_FRAME_HDLCD_CLIENT_NEW_STATUS);
            m_SerialPortNbr = ntohs(*(reinterpret_cast<const uint16_t*>(&m_Payload[1])));            
            const unsigned char &l_Status = m_Payload[3];
            m_bIsResumed = (l_Status & 0x01);
            m_bIsAlive   = (l_Status & 0x02);
            m_eDeserialize = DESERIALIZE_FULL;
            break;
        }
        case DESERIALIZE_ERROR:
        case DESERIALIZE_FULL:
        default:
            assert(false);
        } // switch

        // No error, maybe subsequent bytes are required
        return true;
    }

    // Members
    uint16_t m_SerialPortNbr;
    bool m_bIsResumed;
    bool m_bIsAlive;
    typedef enum {
        DESERIALIZE_ERROR = 0,
        DESERIALIZE_BODY  = 1,
        DESERIALIZE_FULL  = 2
    } E_DESERIALIZE;
    E_DESERIALIZE m_eDeserialize;
};

#endif // HDLCD_CLIENT_DEVICE_NEW_STATUS_H
