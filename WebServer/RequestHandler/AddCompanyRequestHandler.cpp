//
// Created by Artsiom Kaliaha on 4/30/20.
//

#include "AddCompanyRequestHandler.h"
//#include "../Helper.cpp"

using namespace std;

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPResponse;

using Poco::Util::Application;

AddCompanyRequestHandler::AddCompanyRequestHandler(const Storage &s) : storage{s} {}

void AddCompanyRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) {
    Application &app = Application::instance();
    app.logger().information("Request from " + request.clientAddress().toString());

    response.setChunkedTransferEncoding(true);
    response.setContentType("text/html");

    HTMLForm form(request, request.stream());
    if (!form.empty()) {
        app.logger().information(form.get("companyId"));
        app.logger().information(form.get("companyName"));
        app.logger().information(form.get("companyAddress"));

        storage.saveCompany({
                                    stoi(form.get("companyId")),
                                    form.get("companyName"),
                                    form.get("companyAddress")
                            });

        response.redirect("/companies", HTTPResponse::HTTP_TEMPORARY_REDIRECT);
        return;
    }

    response.send() << readHtmlFile("addCompany");
}