#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "databaseapp.grpc.pb.h"

using namespace std;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::ClientReader;
using databaseapp::EmptyRequest;
using databaseapp::CompanyResponse;
using databaseapp::ApplicationServer;

class ApplicationServerClient {
public:
    ApplicationServerClient(shared_ptr<Channel> channel) : stub_(ApplicationServer::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    string getResponse(const string &user) {
        // Follows the same pattern as SayHello.
        EmptyRequest request;
        CompanyResponse reply;
        ClientContext context;


        unique_ptr<ClientReader<CompanyResponse>> reader (stub_->getAllCompanies(&context, request));

        CompanyResponse response;
        while (reader->Read(&response)) {
            cout << response.id() << " " << response.name() << " " << response.address() << endl;
        }

        Status status = reader->Finish();
        if (status.ok()) {
            return "ok!!!";
        } else {
            return "error=(";
        }






        // Here we can use the stub's newly available method we just added.
//        Status status = stub_->getResponse(&context, request, &reply);
//        if (status.ok()) {
//            return reply.content();
//        } else {
//            cout << status.error_code() << ": " << status.error_message() << endl;
//            return "RPC failed";
//        }
    }

private:
    unique_ptr<ApplicationServer::Stub> stub_;
};

int main(int argc, char **argv) {
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint specified by
    // the argument "--target=" which is the only expected argument.
    // We indicate that the channel isn't authenticated (use of
    // InsecureChannelCredentials()).

    string target_str;
    string arg_str("--target");
    if (argc > 1) {
        string arg_val = argv[1];
        size_t start_pos = arg_val.find(arg_str);
        if (start_pos != string::npos) {
            start_pos += arg_str.size();
            if (arg_val[start_pos] == '=') {
                target_str = arg_val.substr(start_pos + 1);
            } else {
                cout << "The only correct argument syntax is --target=" << endl;
                return 0;
            }
        } else {
            cout << "The only acceptable argument is --target=" << endl;
            return 0;
        }
    } else {
        target_str = "localhost:50051";
    }

    ApplicationServerClient greeter(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
    string user("world");

    string reply = greeter.getResponse(user);
    cout << "Greeter received: " << reply << endl;

    return 0;
}
