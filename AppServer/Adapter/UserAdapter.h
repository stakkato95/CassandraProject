//
// Created by Artsiom Kaliaha on 4/28/20.
//

#ifndef APPSERVER_USERADAPTER_H
#define APPSERVER_USERADAPTER_H

#include "Adapter.h"
#include "../Model/User.h"

#include <string>

class UserAdapter : public Adapter<User> {
public:
    User getModel(const ContentValues &value) override;

    ContentValues getContentValues(const User &model) override;

    ContentMappings getAttrs() override;
};


#endif //APPSERVER_USERADAPTER_H
