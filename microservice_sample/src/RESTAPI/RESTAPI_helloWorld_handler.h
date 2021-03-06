//
// Created by stephane bourque on 2021-11-29.
//

#pragma once

#include "framework/MicroService.h"

namespace OpenWifi {
    class RESTAPI_helloWorld_handler : public RESTAPIHandler {
    public:
        RESTAPI_helloWorld_handler(const RESTAPIHandler::BindingMap &bindings, Poco::Logger &L, RESTAPI_GenericServer & Server, bool Internal)
        : RESTAPIHandler(bindings, L,
                         std::vector<std::string>{
                                Poco::Net::HTTPRequest::HTTP_GET,
                                Poco::Net::HTTPRequest::HTTP_OPTIONS},
                        Server,
                        Internal) {}
        static const std::list<const char *> PathName() { return std::list<const char *>{"/api/v1/hello"}; };

        void DoGet() final;
        void DoPost() final {};
        void DoPut() final {};
        void DoDelete() final {};
    };
}
