//
// Created by Artsiom Kaliaha on 4/29/20.
//

#include "CompaniesRequestHandler.h"
#include "../Helper.cpp"

using namespace std;

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

CompaniesRequestHandler::CompaniesRequestHandler(const GrpcDriver &d) : driver{d} {}

void CompaniesRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) {
    Application &app = Application::instance();
    app.logger().information("Request from " + request.clientAddress().toString());

    vector<Company> companies = driver.getAllCompanies();

    response.setChunkedTransferEncoding(true);
    response.setContentType("text/html");

    string page = processTemplate<Company>("companies",
                                           "companies",
                                           "singleCompany",
                                           "<tr><th>{{id}}</th><th>{{name}}</th><th>{{address}}</th></tr>",
                                           companies,
                                           [](const Company &c) {
                                               return mstch::map{{"id",      to_string(c.id)},
                                                                 {"name",    c.name},
                                                                 {"address", c.address}};
                                           });

    response.send() << page;
}