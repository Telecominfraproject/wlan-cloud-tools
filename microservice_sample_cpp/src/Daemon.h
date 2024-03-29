//
//	License type: BSD 3-Clause License
//	License copy: https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/master/LICENSE
//
//	Created by Stephane Bourque on 2021-03-04.
//	Arilia Wireless Inc.
//

#pragma once

#include <array>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>

#include "Poco/Util/Application.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/ErrorHandler.h"
#include "Poco/Crypto/RSAKey.h"
#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Crypto/Cipher.h"

#include "framework/MicroService.h"
#include "framework/OpenWifiTypes.h"

namespace OpenWifi {

	static const char * vDAEMON_PROPERTIES_FILENAME = "ow_helloworld.properties";
	static const char * vDAEMON_ROOT_ENV_VAR = "OWHELLOWORLD_ROOT";
	static const char * vDAEMON_CONFIG_ENV_VAR = "OWHELLOWORLD_CONFIG";
	static const char * vDAEMON_APP_NAME = "ow_helloworld";
	static const uint64_t vDAEMON_BUS_TIMER = 10000;

    class Daemon : public MicroService {
		public:
			explicit Daemon(const std::string & PropFile,
							const std::string & RootEnv,
							const std::string & ConfigEnv,
							const std::string & AppName,
						  	uint64_t 	BusTimer,
							const SubSystemVec & SubSystems) :
				MicroService( PropFile, RootEnv, ConfigEnv, AppName, BusTimer, SubSystems) {};

            void PostInitialization(Poco::Util::Application &self);
			static Daemon *instance();
			Poco::Logger & Log() { return Poco::Logger::get(AppName()); }
	  	private:

    };

	inline Daemon * Daemon() { return Daemon::instance(); }
    void DaemonPostInitialization(Poco::Util::Application &self);
}

