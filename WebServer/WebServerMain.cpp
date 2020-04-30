#include <iostream>

#include <grpcpp/grpcpp.h>

#include "Storage/GrpcDriver.h"
#include "Storage/Storage.h"

#include "../../Model/Company.h"
#include "../../Model/Drone.h"

#include "Mapping/CompanyMapper.h"
#include "Mapping/DroneMapper.h"

#include "Webserver.h"

using namespace std;

int main(int argc, char **argv) {
    GrpcDriver driver(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    driver.registerMapper<Company, CompanyMapper>();
    driver.registerMapper<Drone, DroneMapper>();
    Storage storage(driver);

    WebServer app(storage);
    return app.run(argc, argv);
}
