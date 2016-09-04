/**
 * \file      main-satellite-gateway.cpp
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

#include "Config.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include "ConfigServer/ConfigServerHandlerCollection.h"
#include "GatewayClient/GatewayClientHandlerCollection.h"
#include "HdlcdClient/HdlcdClientHandlerCollection.h"

int main(int argc, char* argv[]) {
    try {
        // Declare the supported options.
        boost::program_options::options_description l_Description("Allowed options");
        l_Description.add_options()
            ("help,h",    "produce this help message")
            ("version,v", "show version information")
            ("port,p",    boost::program_options::value<uint16_t>(),
                          "the TCP port to accept control connections on")
            ("daemon,d",  boost::program_options::value<uint16_t>(),
                          "the TCP port of the HDLC daemon at localhost")
            ("trace,t",   "each relayed packet is dissected and printed")
        ;

        // Parse the command line
        boost::program_options::variables_map l_VariablesMap;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, l_Description), l_VariablesMap);
        boost::program_options::notify(l_VariablesMap);
        if (l_VariablesMap.count("version")) {
            std::cerr << "s-net(r) satellite gateway version " << SATELLITE_GATEWAY_VERSION_MAJOR << "." << SATELLITE_GATEWAY_VERSION_MINOR
                      << " built with hdlcd-devel version " << HDLCD_DEVEL_VERSION_MAJOR << "." << HDLCD_DEVEL_VERSION_MINOR
                      << " and snet-devel version " << SNET_DEVEL_VERSION_MAJOR << "." << SNET_DEVEL_VERSION_MINOR << std::endl;
        } // if

        if (l_VariablesMap.count("help")) {
            std::cout << l_Description << std::endl;
            return 1;
        } // if
                
        if (!l_VariablesMap.count("port")) {
            std::cout << "you have to specify the TCP listener port to accept control connections on" << std::endl;
            return 1;
        } // if
        
        if (!l_VariablesMap.count("daemon")) {
            std::cout << "you have to specify the TCP listener port of the HDLC daemon at localhost" << std::endl;
            return 1;
        } // if
        
        // Install signal handlers
        boost::asio::io_service l_IoService;
        boost::asio::signal_set l_Signals(l_IoService);
        l_Signals.add(SIGINT);
        l_Signals.add(SIGTERM);
        l_Signals.async_wait([&l_IoService](boost::system::error_code, int){ l_IoService.stop(); });

        // Create and initialize components
        auto l_ConfigServerHandlerCollection  = std::make_shared<ConfigServerHandlerCollection>(l_IoService);
        auto l_GatewayClientHandlerCollection = std::make_shared<GatewayClientHandlerCollection>(l_IoService);
        auto l_HdlcdClientHandlerCollection   = std::make_shared<HdlcdClientHandlerCollection>(l_IoService);
        l_ConfigServerHandlerCollection->Initialize (l_GatewayClientHandlerCollection, l_HdlcdClientHandlerCollection);
        l_GatewayClientHandlerCollection->Initialize(l_ConfigServerHandlerCollection, l_HdlcdClientHandlerCollection);
        l_HdlcdClientHandlerCollection->Initialize  (l_ConfigServerHandlerCollection, l_GatewayClientHandlerCollection);
                
        // Start event processing
        l_IoService.run();
        
        // Shutdown
        l_ConfigServerHandlerCollection->SystemShutdown();
        l_GatewayClientHandlerCollection->SystemShutdown();
        l_HdlcdClientHandlerCollection->SystemShutdown();
        
    } catch (std::exception& a_Error) {
        std::cerr << "Exception: " << a_Error.what() << "\n";
        return 1;
    } // catch

    return 0;
}
