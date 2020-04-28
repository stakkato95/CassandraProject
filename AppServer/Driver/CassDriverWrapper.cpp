//
// Created by Artsiom Kaliaha on 4/27/20.
//

#include "CassDriverWrapper.h"

using namespace std;

/////////////////////////////////////////
//helper
/////////////////////////////////////////



/////////////////////////////////////////
//public
/////////////////////////////////////////

CassDriverWrapper::CassDriverWrapper(string ip, string keyspace)
        : ip{move(ip)}, keyspace{move(keyspace)}, isConnected{false} {}

CassDriverWrapper::~CassDriverWrapper() {
    if (isConnected) {
        disconnect();
    }
}

bool CassDriverWrapper::connect() {
    cluster = createCluster();

    if (auto connectResult = connectSession(); connectResult.has_value()) {
        disconnect();
        return false;
    } else {
        isConnected = true;
        return true;
    }
}

void CassDriverWrapper::disconnect() {
    cass_cluster_free(cluster);
    cass_session_free(session);
}

variant<DriverError, ResultCollection>
CassDriverWrapper::select(const string &table,
                          const ContentValues &where,
                          const ContentMappings &attrs) {
    CassStatement *statement = cass_statement_new(getSelectClause(table, where).c_str(), where.size());
    bindParams(statement, where);

    CassFuture *future = cass_session_execute(session, statement);
    cass_future_wait(future);

    ResultCollection resultRows;
    CassError rc = CASS_OK;
    rc = cass_future_error_code(future);
    if (rc != CASS_OK) {
        return returnError(rc, statement, future);
    } else {
        const CassResult *result = cass_future_get_result(future);
        CassIterator *iterator = cass_iterator_from_result(result);

        while (cass_iterator_next(iterator)) {
            const CassRow *row = cass_iterator_get_row(iterator);
            resultRows.push_back(getRow(row, attrs));
        }

        cass_iterator_free(iterator);
        cass_result_free(result);
    }

    freeQuery(statement, future);

    return resultRows;
}

optional<DriverError> CassDriverWrapper::insert(const string &table, const ContentValues &values) {
    CassStatement *statement = cass_statement_new(getInsertClause(table, values).c_str(), values.size());
    bindParams(statement, values);

    CassFuture *future = cass_session_execute(session, statement);
    cass_future_wait(future);

    CassError rc = CASS_OK;
    rc = cass_future_error_code(future);
    if (rc != CASS_OK) {
        return returnError(rc, statement, future);
    }

    freeQuery(statement, future);

    return {};
}

optional<DriverError> CassDriverWrapper::update(const string &table,
                                                const ContentValues &where,
                                                const ContentValues &values) {
    string query = getUpdateClause(table, where, values);
    CassStatement *statement = cass_statement_new(query.c_str(), where.size() + values.size());

    bindParams(statement, values, 0);
    bindParams(statement, where, values.size());

    CassFuture *future = cass_session_execute(session, statement);
    cass_future_wait(future);

    CassError rc = CASS_OK;
    rc = cass_future_error_code(future);
    if (rc != CASS_OK) {
        return returnError(rc, statement, future);
    }

    freeQuery(statement, future);

    return {};
}

optional<DriverError> CassDriverWrapper::del(const string &table, const ContentValues &where) {
    CassStatement *statement = cass_statement_new(getDeleteClause(table, where).c_str(), where.size());
    bindParams(statement, where);

    CassFuture *future = cass_session_execute(session, statement);
    cass_future_wait(future);

    CassError rc = CASS_OK;
    rc = cass_future_error_code(future);
    if (rc != CASS_OK) {
        return returnError(rc, statement, future);
    }

    freeQuery(statement, future);

    return {};
}

/////////////////////////////////////////
//static
/////////////////////////////////////////

string CassDriverWrapper::getSelectClause(const string &table, const ContentValues &values) {
    ostringstream os;

    os << "SELECT * FROM " << table;
    if (values.empty()) {
        return os.str();
    }

    os << " WHERE ";

    for (auto it = begin(values); it != end(values); ++it) {
        os << it->first << " = ?";

        if (next(it) != end(values)) {
            os << " AND ";
        }
    }

    return os.str();
}

string CassDriverWrapper::getInsertClause(const string &table, const ContentValues &values) {
    ostringstream os;
    ostringstream osValues;

    os << "INSERT INTO " << table << " (";

    for (auto it = begin(values); it != end(values); ++it) {
        os << it->first;
        osValues << "?";

        if (next(it) != end(values)) {
            os << ", ";
            osValues << ", ";
        }
    }

    os << ") VALUES (" << osValues.str() << ")";

    return os.str();
}

string CassDriverWrapper::getUpdateClause(const string &table,
                                          const ContentValues &where,
                                          const ContentValues &values) {
    ostringstream os;
    os << "UPDATE " << table << " SET ";

    for (auto it = begin(values); it != end(values); ++it) {
        os << it->first << " = ?";
        if (next(it) != end(values)) {
            os << ", ";
        }
    }

    os << " WHERE ";

    for (auto it = begin(where); it != end(where); ++it) {
        os << it->first << " = ?";
        if (next(it) != end(where)) {
            os << " AND ";
        }
    }

    return os.str();
}

string CassDriverWrapper::getDeleteClause(const string &table, const ContentValues &values) {
    ostringstream os;

    os << "DELETE FROM " << table;
    if (values.empty()) {
        return os.str();
    }

    os << " WHERE ";

    for (auto it = begin(values); it != end(values); ++it) {
        os << it->first << " = ?";

        if (next(it) != end(values)) {
            os << " AND ";
        }
    }

    return os.str();
}


/////////////////////////////////////////
//private
/////////////////////////////////////////

CassCluster *CassDriverWrapper::createCluster() {
    CassCluster *cassCluster = cass_cluster_new();
    cass_cluster_set_contact_points(cassCluster, ip.c_str());
    return cassCluster;
}

optional<DriverError> CassDriverWrapper::connectSession() {
    CassError rc = CASS_OK;
    session = cass_session_new();
    CassFuture *future = cass_session_connect_keyspace(session, cluster, keyspace.c_str());

    cass_future_wait(future);
    rc = cass_future_error_code(future);
    optional<DriverError> result{};

    if (rc != CASS_OK) {
        result = {rc, getError(future)};
    }

    cass_future_free(future);

    return result;
}

string CassDriverWrapper::getError(CassFuture *future) {
    const char *message;
    size_t messageLength;
    cass_future_error_message(future, &message, &messageLength);
    return string(message, messageLength);
}

void CassDriverWrapper::bindParams(CassStatement *statement, const ContentValues &values, int startIndex) {
    for (const auto&[attr, val] : values) {
        if (holds_alternative<string>(val)) {
            cass_statement_bind_string(statement, startIndex, get<string>(val).c_str());
        } else if (holds_alternative<int>(val)) {
            cass_statement_bind_int32(statement, startIndex, get<int>(val));
        } else if (holds_alternative<float>(val)) {
            cass_statement_bind_float(statement, startIndex, get<float>(val));
        }
        startIndex++;
    }
}

ContentValues CassDriverWrapper::getRow(const CassRow *row, const ContentMappings &attrs) {
    ContentValues resultRow;

    for (const auto&[name, type] : attrs) {
        if (type == typeid(string)) {
            const char *strVal = nullptr;
            size_t len = 0;
            cass_value_get_string(cass_row_get_column_by_name(row, name.c_str()), &strVal, &len);
            resultRow[name] = move(string(strVal, len));
        } else if (type == typeid(int)) {
            cass_int32_t intVal = 0;
            cass_value_get_int32(cass_row_get_column_by_name(row, name.c_str()), &intVal);
            resultRow[name] = intVal;
        } else if (type == typeid(float)) {
            cass_float_t floatVal = 0;
            cass_value_get_float(cass_row_get_column_by_name(row, name.c_str()), &floatVal);
            resultRow[name] = floatVal;
        }
    }

    return resultRow;
}

void CassDriverWrapper::freeQuery(CassStatement *statement, CassFuture *future) {
    cass_future_free(future);
    cass_statement_free(statement);
}

DriverError CassDriverWrapper::returnError(CassError rc, CassStatement *statement, CassFuture *future) {
    DriverError driverError{rc, getError(future)};
    freeQuery(statement, future);
    return driverError;
}

