#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "databaseapp.grpc.pb.h"

using namespace std;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using databaseapp::ServerRequest;
using databaseapp::ServerResponse;
using databaseapp::ApplicationServer;

// Logic and data behind the server's behavior.
class ApplicationServerServiceImpl final : public ApplicationServer::Service {
    Status getResponse(ServerContext *context, const ServerRequest *request, ServerResponse *reply) override {
        string prefix("Hello ");
        reply->set_content(prefix + request->topic());
        return Status::OK;
    }
};

void RunServer() {
    string server_address("localhost:50051");
    ApplicationServerServiceImpl service;

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    unique_ptr<Server> server(builder.BuildAndStart());
    cout << "Server listening on " << server_address << endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char **argv) {
    RunServer();
    return 0;
}
