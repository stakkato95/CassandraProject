//
// Created by Artsiom Kaliaha on 4/28/20.
//

#include "CompanyMapper.h"

Company CompanyMapper::getModel(const databaseapp::CompanyResponse &response) const {
    return {
            response.id(),
            response.name(),
            response.address()
    };
}
