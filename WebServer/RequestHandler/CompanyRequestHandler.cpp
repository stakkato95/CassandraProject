//
// Created by Artsiom Kaliaha on 4/29/20.
//

#include "CompanyRequestHandler.h"
#include "../Helper.cpp"

using namespace std;

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

CompanyRequestHandler::CompanyRequestHandler(const GrpcDriver &d, int id) : driver{d}, companyId{id} {}

void CompanyRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) {
    Application &app = Application::instance();
    app.logger().information("Request from " + request.clientAddress().toString());

    vector<Company> companies = driver.getAllCompanies();

    response.setChunkedTransferEncoding(true);
    response.setContentType("text/html");

    response.send() << "hello world " << companyId;

//    string page = processTemplate<Company>("company",
//                                           "company",
//                                           "singleCompany",
//                                           "<tr><th>{{id}}</th><th>{{name}}</th><th>{{address}}</th></tr>",
//                                           companies,
//                                           [](const Company &c) {
//                                               return mstch::map{{"id",      to_string(c.id)},
//                                                                 {"name",    c.name},
//                                                                 {"address", c.address}};
//                                           });

//    response.send() << page;
}