#include <iostream>

#include <grpcpp/grpcpp.h>

#include "../../Model/Company.h"
#include "Mapping/GrpcDriver.h"
#include "Mapping/CompanyMapper.h"

using namespace std;

int main(int argc, char **argv) {
    GrpcDriver driver(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    driver.registerMapper<Company, CompanyMapper>();

    vector<Company> companies = driver.getAllCompanies();
    for (const Company& comp : companies) {
        cout << comp.id << " " << comp.name << " " << comp.address << endl;
    }

    return 0;
}
