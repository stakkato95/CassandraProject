//
// Created by Artsiom Kaliaha on 4/29/20.
//

#include "WebRequestHandlerFactory.h"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;

WebRequestHandlerFactory::WebRequestHandlerFactory(const GrpcDriver &d) : driver{d} {}

HTTPRequestHandler *WebRequestHandlerFactory::createRequestHandler(const HTTPServerRequest &request) {
    if (request.getURI() == "/companies")
        return new CompaniesRequestHandler(driver);
    else
        return 0;
}