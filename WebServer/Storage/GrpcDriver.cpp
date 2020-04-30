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
using databaseapp::CompanyRequest;
using databaseapp::CompanyResponse;
using databaseapp::DroneResponse;
using databaseapp::ApplicationServer;

GrpcDriver::GrpcDriver(shared_ptr<Channel> channel) : stub(ApplicationServer::NewStub(channel)) {}

template<typename TModel, typename TMapper>
void GrpcDriver::registerMapper() {
    mappers[typeid(TModel)] = new TMapper;
}

vector<Company> GrpcDriver::getAllCompanies() const {
    EmptyRequest req;
    ClientContext context;
    unique_ptr<ClientReader<CompanyResponse>> reader(stub->getAllCompanies(&context, req));

    const CompanyMapper *mapper = static_cast<CompanyMapper *>(mappers.at(typeid(Company)));
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

    return {};
}

Company GrpcDriver::getCompanyById(int companyId) const {
    CompanyRequest req;
    req.set_id(companyId);
    CompanyResponse res;

    ClientContext context;
    Status status = stub->getCompany(&context, req, &res);

    if (status.ok()) {
        const CompanyMapper *mapper = static_cast<CompanyMapper *>(mappers.at(typeid(Company)));
        return mapper->getModel(res);
    }

    return {};
}

vector<Drone> GrpcDriver::getDrones(int companyId) const {
    CompanyRequest req;
    req.set_id(companyId);

    ClientContext context;
    unique_ptr<ClientReader<DroneResponse>> reader(stub->getDrones(&context, req));

    const DroneMapper *mapper = static_cast<DroneMapper *>(mappers.at(typeid(Drone)));
    vector<Drone> result;
    DroneResponse res;
    while (reader->Read(&res)) {
        result.push_back(mapper->getModel(res));
    }

    Status status = reader->Finish();
    if (status.ok()) {
        return result;
    }

    return {};
}

////////////////////////
//templates implementation given we know all the classes that will use our class
////////////////////////

template void GrpcDriver::registerMapper<Company, CompanyMapper>();
template void GrpcDriver::registerMapper<Drone, DroneMapper>();