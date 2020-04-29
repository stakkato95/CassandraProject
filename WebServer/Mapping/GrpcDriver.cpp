//
// Created by Artsiom Kaliaha on 4/28/20.
//

#include "GrpcDriver.h"

using namespace std;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::ClientReader;
using databaseapp::EmptyRequest;
using databaseapp::CompanyResponse;
using databaseapp::ApplicationServer;

GrpcDriver::GrpcDriver(shared_ptr<Channel> channel) : stub(ApplicationServer::NewStub(channel)) {}

template<typename TModel, typename TMapper>
void GrpcDriver::registerMapper() {
    mappers[typeid(TModel)] = new TMapper;
}

vector<Company> GrpcDriver::getAllCompanies() {
    EmptyRequest req;
    ClientContext context;
    unique_ptr<ClientReader<CompanyResponse>> reader (stub->getAllCompanies(&context, req));

    CompanyMapper* mapper = static_cast<CompanyMapper*>(mappers[typeid(Company)]);
    vector<Company> result;
    CompanyResponse res;
    while (reader->Read(&res)) {
        result.push_back(mapper->getModel(res));
        //cout << res.id() << " " << res.name() << " " << res.address() << endl;
    }

    Status status = reader->Finish();
    if (status.ok()) {
        return result;
    }

    return { };
}

////////////////////////
//templates implementation given we know all the classes that will use our class
////////////////////////

template void GrpcDriver::registerMapper<Company, CompanyMapper>();