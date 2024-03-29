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
#include "Poco/Net/SSLManager.h"

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
    int ExitCode;
    try {
        Poco::Net::SSLManager::instance().initializeServer(nullptr, nullptr, nullptr);
        auto App = OpenWifi::Daemon::instance();
        ExitCode =  App->run(argc, argv);
        Poco::Net::SSLManager::instance().shutdown();
    } catch (Poco::Exception &exc) {
        ExitCode = Poco::Util::Application::EXIT_SOFTWARE;
        std::cout << exc.displayText() << std::endl;
    } catch (std::exception &exc) {
        ExitCode = Poco::Util::Application::EXIT_TEMPFAIL;
        std::cout << exc.what() << std::endl;
    } catch (...) {
        ExitCode = Poco::Util::Application::EXIT_TEMPFAIL;
        std::cout << "Exception on closure" << std::endl;
    }

    std::cout << "Exitcode: " << ExitCode << std::endl;
    return ExitCode;
}

// end of namespace