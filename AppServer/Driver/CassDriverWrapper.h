//
// Created by Artsiom Kaliaha on 4/27/20.
//

#ifndef APPSERVER_CASSDRIVERWRAPPER_H
#define APPSERVER_CASSDRIVERWRAPPER_H

#include <cassandra.h>

#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <utility>
#include <optional>
#include <variant>
#include <typeindex>
#include <cstdint>

using object = std::variant<std::string, float, int, int64_t, uint32_t>;
using ContentValues = std::unordered_map<std::string, object>;
using ContentMappings = std::unordered_map<std::string, std::type_index>;
using ResultCollection = std::vector<ContentValues>;

struct DriverError {
    CassError error = CASS_OK;
    std::string message;
};

class CassDriverWrapper {
public:
    CassDriverWrapper(std::string ip, std::string keyspace);

    ~CassDriverWrapper();

    bool connect();

    void disconnect();

    std::variant<DriverError, ResultCollection> select(const std::string &table,
                                                       const ContentValues &where,
                                                       const ContentMappings &attrs);

    std::optional<DriverError> insert(const std::string &table, const ContentValues &values);

    std::optional<DriverError> update(const std::string &table,
                                      const ContentValues &where,
                                      const ContentValues &values);

    std::optional<DriverError> del(const std::string &table, const ContentValues &where);

private:
    static std::string getSelectClause(const std::string &table, const ContentValues &values);

    static std::string getInsertClause(const std::string &table, const ContentValues &values);

    static std::string getUpdateClause(const std::string &table,
                                       const ContentValues &where,
                                       const ContentValues &values);

    static std::string getDeleteClause(const std::string &table, const ContentValues &values);

    std::string ip;
    std::string keyspace;
    bool isConnected;

    CassCluster *cluster;
    CassSession *session;

    CassCluster *createCluster();

    std::optional<DriverError> connectSession();

    std::string getError(CassFuture *future);

    void bindParams(CassStatement *statement, const ContentValues &values, int startIndex = 0);

    ContentValues getRow(const CassRow *row, const ContentMappings &attrs);

    void freeQuery(CassStatement *statement, CassFuture *future);

    DriverError returnError(CassError rc, CassStatement *statement, CassFuture *future);
};


#endif //APPSERVER_CASSDRIVERWRAPPER_H
