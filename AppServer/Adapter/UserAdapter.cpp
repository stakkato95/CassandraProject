//
// Created by Artsiom Kaliaha on 4/28/20.
//

#include "UserAdapter.h"

using namespace std;

User UserAdapter::getModel(const ContentValues &value) {
    return {
            get<string>(value.at("lastname")),
            get<int>(value.at("age")),
            get<string>(value.at("city")),
            get<string>(value.at("email")),
            get<string>(value.at("firstname")),
    };
}

ContentValues UserAdapter::getContentValues(const User &model) {
    return {
            {"lastname",  model.lastname},
            {"age",       model.age},
            {"city",      model.city},
            {"email",     model.email},
            {"firstname", model.firstname}
    };
}

ContentMappings UserAdapter::getAttrs() {
    return {
            {"lastname",  type_index(typeid(string))},
            {"age",       type_index(typeid(int))},
            {"city",      type_index(typeid(string))},
            {"email",     type_index(typeid(string))},
            {"firstname", type_index(typeid(string))}
    };
}