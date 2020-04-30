//
// Created by Artsiom Kaliaha on 4/29/20.
//

#include "Webserver.h"

using namespace std;
using Poco::Net::HTTPServerParams;
using Poco::Net::ServerSocket;
using Poco::Net::HTTPServer;
using Poco::ThreadPool;

WebServer::WebServer(const Storage &s) : storage{s} {}

int WebServer::main(const vector<string> &args) {
    int maxThreads = 16;
    ThreadPool::defaultPool().addCapacity(maxThreads);

    HTTPServerParams *pParams = new HTTPServerParams;
    pParams->setMaxQueued(100);
    pParams->setMaxThreads(maxThreads);

    // set-up a server socket
    ServerSocket svs(9980);
    // set-up a HTTPServer instance
    HTTPServer srv(new WebRequestHandlerFactory(storage), svs, pParams);
    // start the HTTPServer
    srv.start();
    // wait for CTRL-C or kill
    waitForTerminationRequest();
    // Stop the HTTPServer
    srv.stop();

    return Application::EXIT_OK;
}