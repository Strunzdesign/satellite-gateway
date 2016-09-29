/**
 * \file      HdlcdClientErrorCodes.h
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

#ifndef HDLCD_CLIENT_ERROR_CODES_H
#define HDLCD_CLIENT_ERROR_CODES_H

typedef enum {
    HDLCD_CLIENT_ERROR_ALREADY_EXISTED  = 0x00,
    HDLCD_CLIENT_ERROR_NOT_EXISTED      = 0x01
} E_HDLCD_CLIENT_ERROR;

#endif // HDLCD_CLIENT_ERROR_CODES_H
