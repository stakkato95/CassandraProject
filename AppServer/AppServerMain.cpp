#include <iostream>

#include "Driver/CassDriverAdapter.h"

#include "ApplicationServerServiceImpl.h"

#include "../Model/Company.h"
#include "../Model/Drone.h"
#include "../Model/Flight.h"

#include "../Model/Mapping/CompanyMapper.h"
#include "../Model/Mapping/DroneMapper.h"

#include "databaseapp.grpc.pb.h"

using namespace std;

using grpc::Server;
using grpc::ServerBuilder;

void RunServer() {
    CassDriverAdapter driver(CassDriverWrapper("127.0.0.1", "autonomousflight"));
    driver.registerAdapter<Company, CompanyAdapter>("company");
    driver.registerAdapter<Drone, DroneAdapter>("drone");
    driver.registerAdapter<Flight, FlightAdapter>("flight");

//    variant<DriverError, vector<Drone>> result = driver.select<Drone, DroneAdapter>({});
//    vector<Drone> drones = get<vector<Drone>>(result);
//    for (const Drone& d : drones) {
//        cout << d.droneId << " " << d.model << endl;
//    }
    
    string serverAddress("localhost:50051");
    ApplicationServerServiceImpl service(driver);
    service.registerMapper<Company, CompanyMapper>();
    service.registerMapper<Drone, DroneMapper>();

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
