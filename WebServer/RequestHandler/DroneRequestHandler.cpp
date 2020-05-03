//
// Created by Artsiom Kaliaha on 5/3/20.
//

#include "DroneRequestHandler.h"

using namespace std;

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

DroneRequestHandler::DroneRequestHandler(const Storage &s, int companyId, int droneId)
        : storage{s}, companyId{companyId}, droneId{droneId} {}

void DroneRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) {
    Application &app = Application::instance();
    app.logger().information("Request from " + request.clientAddress().toString());

    DroneResponse result = storage.getDroneInfo(companyId, droneId);

    response.setChunkedTransferEncoding(true);
    response.setContentType("text/html");

    mstch::map context{
            {"companyId",       result.drone.companyId},
            {"companyName",     result.drone.companyName},
            {"droneId",         result.drone.droneId},
            {"model",           result.drone.model},
            {"firmwareVersion", result.drone.firmwareVersion}
    };

    string page = processTemplate<Flight>("drone",
                                          "flights",
                                          "singleFlight",
                                          "droneFlightItem",
                                          result.flights,
                                          [](const Flight &f) {
                                              string time = cassandraTimeToString(f.yearMonthDay, f.hourMinuteSecond);
                                              return mstch::map{{"companyId", f.companyId},
                                                                {"droneId",   f.droneId},
                                                                {"flightId",  f.flightId},
                                                                {"latitude",  f.latitude},
                                                                {"longitude", f.longitude},
                                                                {"elevation", f.elevation},
                                                                {"time",      time}};
                                          }, context);

    response.send() << page;
}
