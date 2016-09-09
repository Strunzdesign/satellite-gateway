/**
 * \file      ConfigServerHandler.cpp
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

#include "ConfigServerHandler.h"
#include "../GatewayClient/GatewayClientHandlerCollection.h"
#include "../HdlcdClient/HdlcdClientHandlerCollection.h"
#include "ConfigServer.h"
#include "ConfigFrames/GatewayClientCreate.h"
#include "ConfigFrames/GatewayClientCreated.h"
#include "ConfigFrames/GatewayClientDestroy.h"
#include "ConfigFrames/GatewayClientDestroyed.h"
#include "ConfigFrames/GatewayClientConnected.h"
#include "ConfigFrames/GatewayClientDisconnected.h"
#include "ConfigFrames/GatewayClientError.h"
#include "ConfigFrames/HdlcdClientCreate.h"
#include "ConfigFrames/HdlcdClientCreated.h"
#include "ConfigFrames/HdlcdClientDestroy.h"
#include "ConfigFrames/HdlcdClientDestroyed.h"
#include "ConfigFrames/HdlcdClientNewStatus.h"
#include "ConfigFrames/HdlcdClientSuspend.h"
#include "ConfigFrames/HdlcdClientResume.h"
#include "ConfigFrames/HdlcdClientError.h"
#include <assert.h>

ConfigServerHandler::ConfigServerHandler(boost::asio::io_service& a_IOService, std::shared_ptr<GatewayClientHandlerCollection> a_GatewayClientHandlerCollection,
                                         std::shared_ptr<HdlcdClientHandlerCollection> a_HdlcdClientHandlerCollection):
                                         m_IOService(a_IOService), m_GatewayClientHandlerCollection(a_GatewayClientHandlerCollection),m_HdlcdClientHandlerCollection(a_HdlcdClientHandlerCollection) {
    // Checks
    assert(a_GatewayClientHandlerCollection);
    assert(a_HdlcdClientHandlerCollection);
}

void ConfigServerHandler::ConfigFrameReceived(const std::shared_ptr<ConfigFrame> &a_ConfigFrame) {
    // Dispatch
    assert(a_ConfigFrame);
    switch (a_ConfigFrame->GetConfigFrameType()) {
        case CONFIG_FRAME_GATEWAY_CLIENT_CLEANUP: {
            m_GatewayClientHandlerCollection->CleanAll();
            break;
        }
        case CONFIG_FRAME_GATEWAY_CLIENT_CREATE: {
            auto l_GatewayClientCreate = std::dynamic_pointer_cast<GatewayClientCreate>(a_ConfigFrame);
            m_GatewayClientHandlerCollection->CreateClient(l_GatewayClientCreate->GetReferenceNbr(),
                                                           l_GatewayClientCreate->GetRemoteAddress(),
                                                           l_GatewayClientCreate->GetRemotePortNbr());
            break;
        }
        case CONFIG_FRAME_GATEWAY_CLIENT_DESTROY: {
            auto l_GatewayClientDestroy = std::dynamic_pointer_cast<GatewayClientDestroy>(a_ConfigFrame);
            m_GatewayClientHandlerCollection->DestroyClient(l_GatewayClientDestroy->GetReferenceNbr());
            break;
        }
        case CONFIG_FRAME_HDLCD_CLIENT_CLEANUP: {
            m_HdlcdClientHandlerCollection->CleanAll();
            break;
        }
        case CONFIG_FRAME_HDLCD_CLIENT_CREATE: {
            auto l_HdlcdClientCreate = std::dynamic_pointer_cast<HdlcdClientCreate>(a_ConfigFrame);
            m_HdlcdClientHandlerCollection->CreateHdlcdClient(l_HdlcdClientCreate->GetRemoteAddress(), l_HdlcdClientCreate->GetTcpPortNbr(), l_HdlcdClientCreate->GetSerialPortNbr());
            break;
        }
        case CONFIG_FRAME_HDLCD_CLIENT_DESTROY: {
            auto l_HdlcdClientCreate = std::dynamic_pointer_cast<HdlcdClientCreate>(a_ConfigFrame);
            m_HdlcdClientHandlerCollection->DestroyHdlcdClient(l_HdlcdClientCreate->GetSerialPortNbr());
            break;
        }
        case CONFIG_FRAME_HDLCD_CLIENT_SUSPEND: {
            auto l_HdlcdClientSuspend = std::dynamic_pointer_cast<HdlcdClientSuspend>(a_ConfigFrame);
            m_HdlcdClientHandlerCollection->SuspendHdlcdClient(l_HdlcdClientSuspend->GetSerialPortNbr());
            break;
        }
        case CONFIG_FRAME_HDLCD_CLIENT_RESUME: {
            auto l_HdlcdClientResume = std::dynamic_pointer_cast<HdlcdClientResume>(a_ConfigFrame);
            m_HdlcdClientHandlerCollection->ResumeHdlcdClient(l_HdlcdClientResume->GetSerialPortNbr());
            break;
        }
        case CONFIG_FRAME_UNKNOWN:
        case CONFIG_FRAME_GATEWAY_CLIENT_ERROR:
        case CONFIG_FRAME_GATEWAY_CLIENT_CREATED:
        case CONFIG_FRAME_GATEWAY_CLIENT_DESTROYED:
        case CONFIG_FRAME_GATEWAY_CLIENT_CONNECTED:
        case CONFIG_FRAME_GATEWAY_CLIENT_DISCONNECTED:
        case CONFIG_FRAME_HDLCD_CLIENT_DESTROYED:
        case CONFIG_FRAME_HDLCD_CLIENT_NEW_STATUS:
        default:
            // Ignore
            break;
    } // switch
}

void ConfigServerHandler::Close() {
    // Drop all shared pointers
    if (m_ConfigServer) {
        m_ConfigServer->Close();
        m_ConfigServer.reset();
    } // if

    m_GatewayClientHandlerCollection.reset();
    m_HdlcdClientHandlerCollection.reset();
}

void ConfigServerHandler::GatewayClientCreated(uint16_t a_ReferenceNbr) {
    if (m_ConfigServer) {
        // Prepare and send control packet
        m_ConfigServer->SendConfigFrame(GatewayClientCreated::Create(a_ReferenceNbr));
    } // if
}

void ConfigServerHandler::GatewayClientDestroyed(uint16_t a_ReferenceNbr) {
    if (m_ConfigServer) {
        // Prepare and send control packet
        m_ConfigServer->SendConfigFrame(GatewayClientDestroyed::Create(a_ReferenceNbr));
    } // if
}

void ConfigServerHandler::GatewayClientConnected(uint16_t a_ReferenceNbr) {
    if (m_ConfigServer) {
        // Prepare and send control packet
        m_ConfigServer->SendConfigFrame(GatewayClientConnected::Create(a_ReferenceNbr));
    } // if
}

void ConfigServerHandler::GatewayClientDisconnected(uint16_t a_ReferenceNbr) {
    if (m_ConfigServer) {
        // Prepare and send control packet
        m_ConfigServer->SendConfigFrame(GatewayClientDisconnected::Create(a_ReferenceNbr));
    } // if
}

void ConfigServerHandler::GatewayClientError(uint16_t a_ReferenceNbr, uint16_t a_ErrorCode) {
    if (m_ConfigServer) {
        // Prepare and send control packet
        m_ConfigServer->SendConfigFrame(GatewayClientError::Create(a_ReferenceNbr, a_ErrorCode));
    } // if
}

void ConfigServerHandler::HdlcdClientCreated(uint16_t a_SerialPortNbr) {
    if (m_ConfigServer) {
        // Prepare and send control packet
        m_ConfigServer->SendConfigFrame(HdlcdClientCreated::Create(a_SerialPortNbr));
    } // if
}

void ConfigServerHandler::HdlcdClientDestroyed(uint16_t a_SerialPortNbr) {
    if (m_ConfigServer) {
        // Prepare and send control packet
        m_ConfigServer->SendConfigFrame(HdlcdClientDestroyed::Create(a_SerialPortNbr));
    } // if
}

void ConfigServerHandler::HdlcdClientNewStatus(uint16_t a_SerialPortNbr, bool a_bIsResumed, bool a_bIsAlive) {
    if (m_ConfigServer) {
        // Prepare and send control packet
        m_ConfigServer->SendConfigFrame(HdlcdClientNewStatus::Create(a_SerialPortNbr, a_bIsResumed, a_bIsAlive));
    } // if
}

void ConfigServerHandler::HdlcdClientError(uint16_t a_SerialPortNbr, uint16_t a_ErrorCode) {
    if (m_ConfigServer) {
        // Prepare and send control packet
        m_ConfigServer->SendConfigFrame(HdlcdClientError::Create(a_SerialPortNbr, a_ErrorCode));
    } // if
}
