//
// Created by Artsiom Kaliaha on 4/29/20.
//

#include "WebRequestHandlerFactory.h"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;

using namespace std;

static const string COMPANIES = "/companies";
static const string COMPANY = "/company/";
static const string ADD_COMPANY = "/addCompany";
static const string DRONE = "/drone/";

WebRequestHandlerFactory::WebRequestHandlerFactory(const Storage &s) : storage{s} {}

HTTPRequestHandler *WebRequestHandlerFactory::createRequestHandler(const HTTPServerRequest &request) {
    using Poco::Util::Application;
    Application::instance().logger().information(request.getURI());
    if (request.getURI().starts_with(COMPANIES)) {
        return new CompaniesRequestHandler(storage);

    } else if (request.getURI().starts_with(COMPANY)) {
        int companyId = std::stoi(request.getURI().substr(COMPANY.length()));
        return new CompanyRequestHandler(storage, companyId);

    } else if (request.getURI().starts_with(ADD_COMPANY)) {
        return new AddCompanyRequestHandler(storage);

    } else if (request.getURI().starts_with(DRONE)) {
        string ids = request.getURI().substr(DRONE.length());
        int delimiterPosition = ids.find('/');

        int companyId = std::stoi(ids.substr(0, delimiterPosition));
        int droneId = std::stoi(ids.substr(delimiterPosition + 1, ids.length() - delimiterPosition - 1));
        return new DroneRequestHandler(storage, companyId, droneId);
    }

    return 0;
}