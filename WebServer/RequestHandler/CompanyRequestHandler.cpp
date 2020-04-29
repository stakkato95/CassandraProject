//
// Created by Artsiom Kaliaha on 4/29/20.
//

#include "CompanyRequestHandler.h"

using namespace std;

//using namespace inja;
//using json = nlohmann::json;

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

CompanyRequestHandler::CompanyRequestHandler(const GrpcDriver &d) : driver{d} {}

void CompanyRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) {
    Application &app = Application::instance();
    app.logger().information("Request from " + request.clientAddress().toString());

    vector<Company> companies = driver.getAllCompanies();

    response.setChunkedTransferEncoding(true);
    response.setContentType("text/html");


    ifstream file("../html/companies.html");
    string html((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    std::string singleCompany{"<tr><th>{{id}}</th><th>{{name}}</th><th>{{address}}</th></tr>"};

    mstch::array list;
    for (const Company &comp : companies) {
        list.push_back(mstch::map{
                {"id",      to_string(comp.id)},
                {"name",    comp.name},
                {"address", comp.address}
        });
    }
    mstch::map context{{"companies", list}};

    ostream &ostr = response.send();
    ostr << mstch::render(html, context, {{"singleCompany", singleCompany}}) << std::endl;
}