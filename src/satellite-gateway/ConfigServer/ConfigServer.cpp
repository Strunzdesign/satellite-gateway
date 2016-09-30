/**
 * \file      ConfigServer.cpp
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

#include "ConfigServer.h"
#include "GatewayClientHandlerCollection.h"
#include "HdlcdClientHandlerCollection.h"
#include "GatewayClientCreate.h"
#include "GatewayClientCreated.h"
#include "GatewayClientDestroy.h"
#include "GatewayClientDestroyed.h"
#include "GatewayClientConnected.h"
#include "GatewayClientDisconnected.h"
#include "GatewayClientError.h"
#include "HdlcdClientCreate.h"
#include "HdlcdClientCreated.h"
#include "HdlcdClientDestroy.h"
#include "HdlcdClientDestroyed.h"
#include "HdlcdClientNewStatus.h"
#include "HdlcdClientSuspend.h"
#include "HdlcdClientResume.h"
#include "HdlcdClientError.h"
#include <assert.h>

ConfigServer::ConfigServer(boost::asio::io_service& a_IOService, boost::asio::ip::tcp::tcp::socket& a_TcpSocket): m_IOService(a_IOService) {
    // Init the frame endpoint
    m_FrameEndpoint = std::make_shared<FrameEndpoint>(a_IOService, a_TcpSocket);
    m_FrameEndpoint->RegisterFrameFactory(CONFIG_FRAME_GATEWAY_CLIENT_CREATE,  []()->std::shared_ptr<Frame>{ return GatewayClientCreate::CreateDeserializedFrame (); });
    m_FrameEndpoint->RegisterFrameFactory(CONFIG_FRAME_GATEWAY_CLIENT_DESTROY, []()->std::shared_ptr<Frame>{ return GatewayClientDestroy::CreateDeserializedFrame(); });
    m_FrameEndpoint->RegisterFrameFactory(CONFIG_FRAME_HDLCD_CLIENT_CREATE,    []()->std::shared_ptr<Frame>{ return HdlcdClientCreate::CreateDeserializedFrame   (); });
    m_FrameEndpoint->RegisterFrameFactory(CONFIG_FRAME_HDLCD_CLIENT_DESTROY,   []()->std::shared_ptr<Frame>{ return HdlcdClientDestroy::CreateDeserializedFrame  (); });
    m_FrameEndpoint->RegisterFrameFactory(CONFIG_FRAME_HDLCD_CLIENT_SUSPEND,   []()->std::shared_ptr<Frame>{ return HdlcdClientSuspend::CreateDeserializedFrame  (); });
    m_FrameEndpoint->RegisterFrameFactory(CONFIG_FRAME_HDLCD_CLIENT_RESUME,    []()->std::shared_ptr<Frame>{ return HdlcdClientResume::CreateDeserializedFrame   (); });
    m_FrameEndpoint->SetOnFrameCallback  ([this](std::shared_ptr<Frame> a_Frame)->bool{ return OnFrame(a_Frame); });
    m_FrameEndpoint->SetOnClosedCallback ([this](){ OnClosed(); });
}

void ConfigServer::Start(std::shared_ptr<GatewayClientHandlerCollection> a_GatewayClientHandlerCollection,
                         std::shared_ptr<HdlcdClientHandlerCollection>   a_HdlcdClientHandlerCollection) {
    assert(a_GatewayClientHandlerCollection);
    assert(a_HdlcdClientHandlerCollection);
    m_GatewayClientHandlerCollection = a_GatewayClientHandlerCollection;
    m_HdlcdClientHandlerCollection   = a_HdlcdClientHandlerCollection;
    m_FrameEndpoint->Start();
}

void ConfigServer::Close() {
    // Drop all shared pointers
    m_GatewayClientHandlerCollection.reset();
    m_HdlcdClientHandlerCollection.reset();

    // Close entities
    m_FrameEndpoint->Close();
    if (m_OnClosedCallback) {
        m_OnClosedCallback();
    } // if
}

void ConfigServer::GatewayClientCreated(uint16_t a_ReferenceNbr) {
    // Prepare and send control packet
    m_FrameEndpoint->SendFrame(GatewayClientCreated::Create(a_ReferenceNbr));
}

void ConfigServer::GatewayClientDestroyed(uint16_t a_ReferenceNbr) {
    // Prepare and send control packet
    m_FrameEndpoint->SendFrame(GatewayClientDestroyed::Create(a_ReferenceNbr));
}

void ConfigServer::GatewayClientConnected(uint16_t a_ReferenceNbr) {
    // Prepare and send control packet
    m_FrameEndpoint->SendFrame(GatewayClientConnected::Create(a_ReferenceNbr));
}

void ConfigServer::GatewayClientDisconnected(uint16_t a_ReferenceNbr) {
    // Prepare and send control packet
    m_FrameEndpoint->SendFrame(GatewayClientDisconnected::Create(a_ReferenceNbr));
}

void ConfigServer::GatewayClientError(uint16_t a_ReferenceNbr, E_GATEWAY_CLIENT_ERROR a_ErrorCode) {
    // Prepare and send control packet
    m_FrameEndpoint->SendFrame(GatewayClientError::Create(a_ReferenceNbr, a_ErrorCode));
}

void ConfigServer::HdlcdClientCreated(uint16_t a_SerialPortNbr) {
    // Prepare and send control packet
    m_FrameEndpoint->SendFrame(HdlcdClientCreated::Create(a_SerialPortNbr));
}

void ConfigServer::HdlcdClientDestroyed(uint16_t a_SerialPortNbr) {
    // Prepare and send control packet
    m_FrameEndpoint->SendFrame(HdlcdClientDestroyed::Create(a_SerialPortNbr));
}

void ConfigServer::HdlcdClientNewStatus(uint16_t a_SerialPortNbr, bool a_bIsResumed, bool a_bIsAlive) {
    // Prepare and send control packet
    m_FrameEndpoint->SendFrame(HdlcdClientNewStatus::Create(a_SerialPortNbr, a_bIsResumed, a_bIsAlive));
}

void ConfigServer::HdlcdClientError(uint16_t a_SerialPortNbr, E_HDLCD_CLIENT_ERROR a_ErrorCode) {
    // Prepare and send control packet
    m_FrameEndpoint->SendFrame(HdlcdClientError::Create(a_SerialPortNbr, a_ErrorCode));
}

bool ConfigServer::OnFrame(std::shared_ptr<Frame> a_Frame) {
    // Dispatch
    assert(a_Frame);
    auto l_ConfigFrame = std::dynamic_pointer_cast<ConfigFrame>(a_Frame);
    assert(l_ConfigFrame);
    switch (l_ConfigFrame->GetConfigFrameType()) {
        case CONFIG_FRAME_GATEWAY_CLIENT_CLEANUP: {
            m_GatewayClientHandlerCollection->CleanAll();
            break;
        }
        case CONFIG_FRAME_GATEWAY_CLIENT_CREATE: {
            auto l_GatewayClientCreate = std::dynamic_pointer_cast<GatewayClientCreate>(l_ConfigFrame);
            m_GatewayClientHandlerCollection->CreateClient(l_GatewayClientCreate->GetReferenceNbr(),
                                                           l_GatewayClientCreate->GetRemoteAddress(),
                                                           l_GatewayClientCreate->GetRemotePortNbr());
            break;
        }
        case CONFIG_FRAME_GATEWAY_CLIENT_DESTROY: {
            auto l_GatewayClientDestroy = std::dynamic_pointer_cast<GatewayClientDestroy>(l_ConfigFrame);
            m_GatewayClientHandlerCollection->DestroyClient(l_GatewayClientDestroy->GetReferenceNbr());
            break;
        }
        case CONFIG_FRAME_HDLCD_CLIENT_CLEANUP: {
            m_HdlcdClientHandlerCollection->CleanAll();
            break;
        }
        case CONFIG_FRAME_HDLCD_CLIENT_CREATE: {
            auto l_HdlcdClientCreate = std::dynamic_pointer_cast<HdlcdClientCreate>(l_ConfigFrame);
            m_HdlcdClientHandlerCollection->CreateHdlcdClient(l_HdlcdClientCreate->GetRemoteAddress(), l_HdlcdClientCreate->GetTcpPortNbr(), l_HdlcdClientCreate->GetSerialPortNbr());
            break;
        }
        case CONFIG_FRAME_HDLCD_CLIENT_DESTROY: {
            auto l_HdlcdClientCreate = std::dynamic_pointer_cast<HdlcdClientCreate>(l_ConfigFrame);
            m_HdlcdClientHandlerCollection->DestroyHdlcdClient(l_HdlcdClientCreate->GetSerialPortNbr());
            break;
        }
        case CONFIG_FRAME_HDLCD_CLIENT_SUSPEND: {
            auto l_HdlcdClientSuspend = std::dynamic_pointer_cast<HdlcdClientSuspend>(l_ConfigFrame);
            m_HdlcdClientHandlerCollection->SuspendHdlcdClient(l_HdlcdClientSuspend->GetSerialPortNbr());
            break;
        }
        case CONFIG_FRAME_HDLCD_CLIENT_RESUME: {
            auto l_HdlcdClientResume = std::dynamic_pointer_cast<HdlcdClientResume>(l_ConfigFrame);
            m_HdlcdClientHandlerCollection->ResumeHdlcdClient(l_HdlcdClientResume->GetSerialPortNbr());
            break;
        }
        default:
            // Ignore
            break;
    } // switch
    
    return true;
}

void ConfigServer::OnClosed() {
    Close();
}
