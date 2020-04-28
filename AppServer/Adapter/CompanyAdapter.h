//
// Created by Artsiom Kaliaha on 4/28/20.
//

#ifndef APPSERVER_COMPANYADAPTER_H
#define APPSERVER_COMPANYADAPTER_H

#include "Adapter.h"
#include "../../Model/Company.h"

#include <string>

class CompanyAdapter : public Adapter<Company> {
public:
    Company getModel(const ContentValues &value) override;

    ContentValues getContentValues(const Company &model) override;

    ContentMappings getAttrs() override;
};


#endif //APPSERVER_COMPANYADAPTER_H
