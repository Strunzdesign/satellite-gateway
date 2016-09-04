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

class HdlcdClientNewStatus: public ConfigFrame {
public:
    // DTOR and creator
    HdlcdClientNewStatus(){}
    ~HdlcdClientNewStatus(){}
    static std::shared_ptr<HdlcdClientNewStatus> Create(uint16_t a_SerialPortNbr, bool a_bIsResumed, bool a_bIsAlive) {
        auto l_HdlcdClientNewStatus = std::make_shared<HdlcdClientNewStatus>();
        l_HdlcdClientNewStatus->m_SerialPortNbr = a_SerialPortNbr;
        l_HdlcdClientNewStatus->m_bIsResumed = a_bIsResumed;
        l_HdlcdClientNewStatus->m_bIsAlive = a_bIsAlive;
        return l_HdlcdClientNewStatus;
    }
    
    // Getter
    uint16_t GetSerialPortNbr() const { return m_SerialPortNbr; }
    uint16_t GetIsResumed() const { return m_bIsResumed; }
    uint16_t GetIsAlive() const { return m_bIsAlive; }
    
private:
    // Methods
    E_CONFIG_FRAME GetConfigFrameType() const { return CONFIG_FRAME_HDLCD_CLIENT_NEW_STATUS; }
    
    // Members
    uint16_t m_SerialPortNbr;
    bool m_bIsResumed;
    bool m_bIsAlive;
};

#endif // HDLCD_CLIENT_DEVICE_NEW_STATUS_H
