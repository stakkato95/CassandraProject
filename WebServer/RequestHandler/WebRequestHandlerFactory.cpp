//
// Created by Artsiom Kaliaha on 4/29/20.
//

#include "WebRequestHandlerFactory.h"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;

using namespace std;

static const string COMPANIES = "/companies";
static const string COMPANY = "/company/";

WebRequestHandlerFactory::WebRequestHandlerFactory(const Storage &s) : storage{s} {}

HTTPRequestHandler *WebRequestHandlerFactory::createRequestHandler(const HTTPServerRequest &request) {
    if (request.getURI() == COMPANIES) {
        return new CompaniesRequestHandler(storage);
    } else if (request.getURI().starts_with(COMPANY)) {
        int companyId = std::stoi(request.getURI().substr(COMPANY.length()));
        return new CompanyRequestHandler(storage, companyId);
    }

    return 0;
}