//
// Created by Artsiom Kaliaha on 4/29/20.
//

#include "CompaniesRequestHandler.h"

using namespace std;

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

using Poco::Util::Application;

CompaniesRequestHandler::CompaniesRequestHandler(const Storage &s) : storage{s} {}

void CompaniesRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) {
    Application &app = Application::instance();
    app.logger().information("Request from " + request.clientAddress().toString());

    response.setChunkedTransferEncoding(true);
    response.setContentType("text/html");


    vector<Company> companies = storage.getAllCompanies();
    string page = processTemplate<Company>("companies",
                                           "companies",
                                           "singleCompany",
                                           "companiesItem",
                                           companies,
                                           [](const Company &c) {
                                               return mstch::map{{"id",      to_string(c.id)},
                                                                 {"name",    c.name},
                                                                 {"address", c.address}};
                                           });

    response.send() << page;
}