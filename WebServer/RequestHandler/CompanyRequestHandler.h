//
// Created by Artsiom Kaliaha on 4/29/20.
//

#ifndef WEBSERVER_COMPANYREQUESTHANDLER_H
#define WEBSERVER_COMPANYREQUESTHANDLER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"

class CompanyRequestHandler : public Poco::Net::HTTPRequestHandler {
public:
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
    {
        Poco::Util::Application& app = Poco::Util::Application::instance();
        app.logger().information("Request from " + request.clientAddress().toString());

        //Poco::Timestamp now;
        //std::string dt(Poco::DateTimeFormatter::format(now, _format));

        response.setChunkedTransferEncoding(true);
        response.setContentType("text/html");

        std::ostream& ostr = response.send();
        ostr << "<html><head><title>HTTPTimeServer powered by POCO C++ Libraries</title></head>";
        ostr << "<body><p style=\"text-align: center; font-size: 48px;\">";
        ostr << "Hello world!!!";
        ostr << "</p></body></html>";
    }
};


#endif //WEBSERVER_COMPANYREQUESTHANDLER_H
