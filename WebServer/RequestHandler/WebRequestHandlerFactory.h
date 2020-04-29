//
// Created by Artsiom Kaliaha on 4/29/20.
//

#ifndef WEBSERVER_WEBREQUESTHANDLERFACTORY_H
#define WEBSERVER_WEBREQUESTHANDLERFACTORY_H

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

#include "CompaniesRequestHandler.h"
#include "../Mapping/GrpcDriver.h"

class WebRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
    explicit WebRequestHandlerFactory(const GrpcDriver &d);

public:
    Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &request);

private:
    const GrpcDriver &driver;
};


#endif //WEBSERVER_WEBREQUESTHANDLERFACTORY_H
