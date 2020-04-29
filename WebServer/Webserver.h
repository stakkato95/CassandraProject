//
// Created by Artsiom Kaliaha on 4/29/20.
//

#ifndef WEBSERVER_WEBSERVER_H
#define WEBSERVER_WEBSERVER_H

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

#include "RequestHandler/WebRequestHandlerFactory.h"

class WebServer : public Poco::Util::ServerApplication {
protected:
    int main(const std::vector<std::string> &args) {
        int maxThreads = 16;
        Poco::ThreadPool::defaultPool().addCapacity(maxThreads);

        Poco::Net::HTTPServerParams *pParams = new Poco::Net::HTTPServerParams;
        pParams->setMaxQueued(maxThreads);
        pParams->setMaxThreads(16);

        // set-up a server socket
        Poco::Net::ServerSocket svs(9980);
        // set-up a HTTPServer instance
        Poco::Net::HTTPServer srv(new WebRequestHandlerFactory(), svs, pParams);
        // start the HTTPServer
        srv.start();
        // wait for CTRL-C or kill
        waitForTerminationRequest();
        // Stop the HTTPServer
        srv.stop();

        return Application::EXIT_OK;
    }
};


#endif //WEBSERVER_WEBSERVER_H
