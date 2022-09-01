//
// Created by stephane bourque on 2021-11-29.
//

#include "RESTAPI_helloWorld_handler.h"

namespace OpenWifi {

    void RESTAPI_helloWorld_handler::DoGet() {
        Poco::JSON::Object  Answer;

        Answer.set("response","World!");
        Answer.set("From", UserInfo_.userinfo.email);
        Answer.set("time", std::time(nullptr));

        return ReturnObject(Answer);
    }

}