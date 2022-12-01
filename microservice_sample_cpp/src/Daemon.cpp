//
//	License type: BSD 3-Clause License
//	License copy: https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/master/LICENSE
//
//	Created by Stephane Bourque on 2021-03-04.
//	Arilia Wireless Inc.
//

#include <boost/algorithm/string.hpp>
#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Environment.h"

#include "Daemon.h"
#include "StorageService.h"
#include "framework/ConfigurationValidator.h"
#include "framework/MicroService.h"

namespace OpenWifi {
	class Daemon *Daemon::instance() {
	    static Daemon instance(vDAEMON_PROPERTIES_FILENAME,
								   vDAEMON_ROOT_ENV_VAR,
								   vDAEMON_CONFIG_ENV_VAR,
								   vDAEMON_APP_NAME,
								   vDAEMON_BUS_TIMER,
								   SubSystemVec{
									   StorageService()
								   });
        return &instance;
	}

    void Daemon::PostInitialization([[maybe_unused]] Poco::Util::Application &self) {
    }

    void DaemonPostInitialization(Poco::Util::Application &self) {
        Daemon()->PostInitialization(self);
    }

}

int main(int argc, char **argv) {
	try {

		auto App = OpenWifi::Daemon::instance();
		auto ExitCode =  App->run(argc, argv);
		return ExitCode;

	} catch (Poco::Exception &exc) {
		std::cerr << exc.displayText() << std::endl;
		return Poco::Util::Application::EXIT_SOFTWARE;
	}
}

// end of namespace