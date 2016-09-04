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

class HdlcdClientDestroy: public ConfigFrame {
public:
    // DTOR and creator
    HdlcdClientDestroy(){}
    ~HdlcdClientDestroy(){}
    static std::shared_ptr<HdlcdClientDestroy> Create(uint16_t a_SerialPortNbr) {
        auto l_HdlcdClientDestroy = std::make_shared<HdlcdClientDestroy>();
        l_HdlcdClientDestroy->m_SerialPortNbr = a_SerialPortNbr;
        return l_HdlcdClientDestroy;
    }
    
    // Getter
    uint16_t GetSerialPortNbr() const { return m_SerialPortNbr; }
    
private:
    // Methods
    E_CONFIG_FRAME GetConfigFrameType() const { return CONFIG_FRAME_HDLCD_CLIENT_DESTROY; }
    
    // Members
    uint16_t m_SerialPortNbr;
};

#endif // HDLCD_CLIENT_DESTROY_H
