/**
 * \file      HdlcdClientDeviceError.h
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

#ifndef HDLCD_CLIENT_DEVICE_ERROR_H
#define HDLCD_CLIENT_DEVICE_ERROR_H

#include "ConfigFrame.h"

class HdlcdClientError: public ConfigFrame {
public:
    // DTOR and creator
    HdlcdClientError(){}
    ~HdlcdClientError(){}
    static std::shared_ptr<HdlcdClientError> Create(uint16_t a_SerialPortNbr, uint32_t a_ErrorCode) {
        auto l_HdlcdClientError = std::make_shared<HdlcdClientError>();
        l_HdlcdClientError->m_SerialPortNbr = a_SerialPortNbr;
        l_HdlcdClientError->m_ErrorCode = a_ErrorCode;
        return l_HdlcdClientError;
    }
    
    // Getter
    uint16_t GetSerialPortNbr() const { return m_SerialPortNbr; }
    uint16_t GetErrorCode() const { return m_ErrorCode; }
    
private:
    // Methods
    E_CONFIG_FRAME GetConfigFrameType() const { return CONFIG_FRAME_HDLCD_CLIENT_ERROR; }
    
    // Members
    uint16_t m_SerialPortNbr;
    uint32_t m_ErrorCode;
};

#endif // HDLCD_CLIENT_DEVICE_ERROR_H
