#include <iostream>

#include "Driver/CassDriverAdapter.h"

#include "ApplicationServerServiceImpl.h"

using namespace std;

using grpc::Server;
using grpc::ServerBuilder;

void RunServer() {
    CassDriverAdapter driver(CassDriverWrapper("127.0.0.1", "autonomousflight"));
    driver.registerAdapter<Drone, DroneAdapter>("drone");
    driver.registerAdapter<Company, CompanyAdapter>("company");

//    variant<DriverError, vector<Drone>> result = driver.select<Drone, DroneAdapter>({});
//    vector<Drone> drones = get<vector<Drone>>(result);
//    for (const Drone& d : drones) {
//        cout << d.droneId << " " << d.model << endl;
//    }
    
    string serverAddress("localhost:50051");
    ApplicationServerServiceImpl service(driver);

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    unique_ptr<Server> server(builder.BuildAndStart());

    cout << "Server listening on " << serverAddress << endl;
    server->Wait();
}

int main(int argc, char **argv) {
    RunServer();

    return 0;
}
