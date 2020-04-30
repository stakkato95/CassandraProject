//
// Created by Artsiom Kaliaha on 4/29/20.
//

#include "CompanyRequestHandler.h"

using namespace std;

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

CompanyRequestHandler::CompanyRequestHandler(const Storage &s, int id) : storage{s}, companyId{id} {}

void CompanyRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) {
    Application &app = Application::instance();
    app.logger().information("Request from " + request.clientAddress().toString());

    CompanyResponse result = storage.getCompanyInfo(companyId);

    response.setChunkedTransferEncoding(true);
    response.setContentType("text/html");

    mstch::map context{
            {"id",      result.company.id},
            {"name",    result.company.name},
            {"address", result.company.address}
    };

    string page = processTemplate<Drone>("company",
                                         "drones",
                                         "singleDrone",
                                         "companyDroneItem",
                                         result.drones,
                                         [](const Drone &d) {
                                             return mstch::map{{"companyId",       to_string(d.companyId)},
                                                               {"companyName",     d.companyName},
                                                               {"droneId",         d.droneId},
                                                               {"model",           d.model},
                                                               {"firmwareVersion", d.firmwareVersion}};
                                         }, context);

    response.send() << page;
}