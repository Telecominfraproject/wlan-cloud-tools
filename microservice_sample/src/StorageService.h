//
//	License type: BSD 3-Clause License
//	License copy: https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/master/LICENSE
//
//	Created by Stephane Bourque on 2021-03-04.
//	Arilia Wireless Inc.
//

#pragma once

#include "framework/MicroService.h"
#include "framework/StorageClass.h"
#include "RESTObjects//RESTAPI_GWobjects.h"
#include "Poco/Net/IPAddress.h"

namespace OpenWifi {

    class Storage : public StorageClass {

    public:

        static Storage *instance() {
			static Storage * instance_ = new Storage;
			return instance_;
        }

		int 	Start() override;
		void 	Stop() override;

	  private:

   };

   inline Storage * StorageService() { return Storage::instance(); }

}  // namespace

