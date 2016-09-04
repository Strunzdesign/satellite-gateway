/**
 * \file      ConfigFrame.h
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

#ifndef CONFIG_FRAME_H
#define CONFIG_FRAME_H

typedef enum {
    CONFIG_FRAME_UNKNOWN                     = 0x00,
    CONFIG_FRAME_GATEWAY_CLIENT_ERROR        = 0x10,
    CONFIG_FRAME_GATEWAY_CLIENT_CLEANUP      = 0x11,
    CONFIG_FRAME_GATEWAY_CLIENT_CREATE       = 0x12,
    CONFIG_FRAME_GATEWAY_CLIENT_CREATED      = 0x13,
    CONFIG_FRAME_GATEWAY_CLIENT_DESTROY      = 0x14,
    CONFIG_FRAME_GATEWAY_CLIENT_DESTROYED    = 0x15,
    CONFIG_FRAME_GATEWAY_CLIENT_CONNECTED    = 0x16,
    CONFIG_FRAME_GATEWAY_CLIENT_DISCONNECTED = 0x17,
    
    CONFIG_FRAME_HDLCD_CLIENT_ERROR          = 0x40,
    CONFIG_FRAME_HDLCD_CLIENT_CLEANUP        = 0x41,
    CONFIG_FRAME_HDLCD_CLIENT_CREATE         = 0x42,
    CONFIG_FRAME_HDLCD_CLIENT_CREATED        = 0x43,
    CONFIG_FRAME_HDLCD_CLIENT_DESTROY        = 0x44,
    CONFIG_FRAME_HDLCD_CLIENT_DESTROYED      = 0x45,
    CONFIG_FRAME_HDLCD_CLIENT_DEVICE_FOUND   = 0x46,
    CONFIG_FRAME_HDLCD_CLIENT_DEVICE_LOST    = 0x47,
    CONFIG_FRAME_HDLCD_CLIENT_SUSPEND        = 0x48,
    CONFIG_FRAME_HDLCD_CLIENT_RESUME         = 0x49,
    CONFIG_FRAME_HDLCD_CLIENT_NEW_STATUS     = 0x4A
} E_CONFIG_FRAME;

class ConfigFrame {
public:
    // CTOR and DTOR
    ConfigFrame(){}
    virtual ~ConfigFrame(){} 
    
    virtual E_CONFIG_FRAME GetConfigFrameType() const { return CONFIG_FRAME_UNKNOWN; }
};

#endif // CONFIG_FRAME_H
