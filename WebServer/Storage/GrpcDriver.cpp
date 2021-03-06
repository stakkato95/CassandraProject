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
using databaseapp::CompanyMessage;
using databaseapp::DroneResponse;
using databaseapp::ApplicationServer;
using databaseapp::SaveCompanyResponse;
using databaseapp::FlightRequest;
using databaseapp::FlightResponse;
using databaseapp::DroneRequest;

GrpcDriver::GrpcDriver(shared_ptr<Channel> channel) : stub(ApplicationServer::NewStub(channel)) {}

template<typename TModel, typename TMapper>
void GrpcDriver::registerMapper() {
    mappers[typeid(TModel)] = new TMapper;
}

vector<Company> GrpcDriver::getAllCompanies() const {
    EmptyRequest req;
    ClientContext context;
    unique_ptr<ClientReader<CompanyMessage>> reader(stub->getAllCompanies(&context, req));

    const CompanyMapper *mapper = static_cast<CompanyMapper *>(mappers.at(typeid(Company)));
    vector<Company> result;
    CompanyMessage res;
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
    CompanyMessage res;

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

bool GrpcDriver::saveCompany(const Company &company) const {
    CompanyMessage req;
    req.set_id(company.id);
    req.set_name(company.name);
    req.set_address(company.address);

    SaveCompanyResponse res;
    ClientContext context;
    Status status = stub->saveCompany(&context, req, &res);

    if (status.ok()) {
        return res.issaved();
    }

    return false;
}

vector<Flight> GrpcDriver::getFlights(int companyId, int droneId) const {
    FlightRequest req;
    req.set_companyid(companyId);
    req.set_droneid(droneId);

    ClientContext context;
    unique_ptr<ClientReader<FlightResponse>> reader(stub->getFlights(&context, req));

    const FlightMapper *mapper = static_cast<FlightMapper *>(mappers.at(typeid(Flight)));
    vector<Flight> result;
    FlightResponse res;
    while (reader->Read(&res)) {
        result.push_back(mapper->getModel(res));
    }

    Status status = reader->Finish();
    if (status.ok()) {
        return result;
    }

    return {};
}

Drone GrpcDriver::getDrone(int companyId, int droneId) const {
    DroneRequest req;
    req.set_companyid(companyId);
    req.set_droneid(droneId);

    DroneResponse res;
    ClientContext context;
    Status status = stub->getDrone(&context, req, &res);

    if (status.ok()) {
        const DroneMapper *mapper = static_cast<DroneMapper *>(mappers.at(typeid(Drone)));
        return mapper->getModel(res);
    }

    return {};
}

////////////////////////
//templates implementation given we know all the classes that will use our class
////////////////////////

template void GrpcDriver::registerMapper<Company, CompanyMapper>();

template void GrpcDriver::registerMapper<Drone, DroneMapper>();

template void GrpcDriver::registerMapper<Flight, FlightMapper>();