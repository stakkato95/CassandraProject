//
// Created by Artsiom Kaliaha on 4/29/20.
//

#ifndef WEBSERVER_COMPANYREQUESTHANDLER_H
#define WEBSERVER_COMPANYREQUESTHANDLER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"

#include "../Mapping/GrpcDriver.h"

#include <mstch/mstch.hpp>

#include <fstream>

class CompanyRequestHandler : public Poco::Net::HTTPRequestHandler {
public:
    explicit CompanyRequestHandler(const GrpcDriver &d);

    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);

private:
    const GrpcDriver &driver;
};


#endif //WEBSERVER_COMPANYREQUESTHANDLER_H
