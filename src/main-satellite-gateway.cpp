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
#include "HdlcdClient/HdlcdClientHandlerCollection.h"

int main(int argc, char* argv[]) {
    try {
        // Declare the supported options.
        boost::program_options::options_description l_Description("Allowed options");
        l_Description.add_options()
            ("help,h",    "produce this help message")
            ("version,v", "show version information")
            ("cport,cp",  boost::program_options::value<uint16_t>(),
                          "the TCP port to accept control connections on")
            ("hport,hp",  boost::program_options::value<uint16_t>(),
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
                
        if (!l_VariablesMap.count("cport")) {
            std::cout << "you have to specify the TCP listener port to accept control connections on" << std::endl;
            return 1;
        } // if
        
        if (!l_VariablesMap.count("hport")) {
            std::cout << "you have to specify the TCP listener port of the HDLC daemon at localhost" << std::endl;
            return 1;
        } // if

        if (!l_VariablesMap.count("connect")) {
            std::cout << "you have to specify at least one device to connect to!" << std::endl;
            return 1;
        } // if
        
        // Install signal handlers
        boost::asio::io_service l_IoService;
        boost::asio::signal_set l_Signals(l_IoService);
        l_Signals.add(SIGINT);
        l_Signals.add(SIGTERM);
        l_Signals.async_wait([&l_IoService](boost::system::error_code, int){ l_IoService.stop(); });

        //ToolHandlerCollection l_ToolHandlerCollection;
        HdlcdClientHandlerCollection l_HdlcdClientHandlerCollection(l_IoService);
        //ToolAcceptor l_ToolAcceptor(l_IoService, l_VariablesMap["cport"].as<uint16_t>(), l_ToolHandlerCollection);
        
        /*
        // Routing entity
        Routing l_Routing(l_ToolHandlerCollection, l_HdlcdClientHandlerCollection, l_VariablesMap.count("trace"));
        l_ToolHandlerCollection.RegisterRoutingEntity(&l_Routing);
        l_HdlcdClientHandlerCollection.RegisterRoutingEntity(&l_Routing);
        */
        
        // Create HDLCd client entities
        auto l_DestSpecifiers(l_VariablesMap["connect"].as<std::vector<std::string>>());
        for (auto l_DestSpecifier = l_DestSpecifiers.begin(); l_DestSpecifier != l_DestSpecifiers.end(); ++l_DestSpecifier) {
            l_HdlcdClientHandlerCollection.CreateHdlcdClientHandler("localhost", "10001", "/dev/ttyUSB0");
        } // for
        
        // Start event processing
        l_IoService.run();
    } catch (std::exception& a_Error) {
        std::cerr << "Exception: " << a_Error.what() << "\n";
        return 1;
    } // catch

    return 0;
}
