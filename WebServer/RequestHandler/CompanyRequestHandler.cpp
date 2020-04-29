//
// Created by Artsiom Kaliaha on 4/29/20.
//

#include "CompanyRequestHandler.h"

using namespace std;

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

CompanyRequestHandler::CompanyRequestHandler(const GrpcDriver &d) : driver{d} {}

void CompanyRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) {
    Application &app = Application::instance();
    app.logger().information("Request from " + request.clientAddress().toString());


    vector<Company> companies = driver.getAllCompanies();
    for (const Company &comp : companies) {
        cout << comp.id << " " << comp.name << " " << comp.address << endl;
    }

    //Poco::Timestamp now;
    //std::string dt(Poco::DateTimeFormatter::format(now, _format));

    response.setChunkedTransferEncoding(true);
    response.setContentType("text/html");

    std::ostream &ostr = response.send();
    ostr << "<html><head><title>HTTPTimeServer powered by POCO C++ Libraries</title></head>";
    ostr << "<body><p style=\"text-align: center; font-size: 48px;\">";
    ostr << "Hello world!!!";
    ostr << "</p>";

    ostr << "<table style=\"width:100%\">";
    for (const Company &comp : companies) {
        ostr << "<tr>";
        ostr << "<th>" << comp.id << "</th>";
        ostr << "<th>" << comp.name << "</th>";
        ostr << "<th>" << comp.address << "</th>";
        ostr << "</tr>";
    }
    ostr << "</table>";

    ostr << "</body></html>";
}