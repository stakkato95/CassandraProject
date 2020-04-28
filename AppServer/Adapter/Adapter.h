//
// Created by Artsiom Kaliaha on 4/28/20.
//

#ifndef APPSERVER_ADAPTER_H
#define APPSERVER_ADAPTER_H

#include "../Driver/CassDriverWrapper.h"
#include <variant>

class IAdapter {
};

template<typename T>
class Adapter : public IAdapter {
public:
    Adapter() = default;

    virtual T getModel(const ContentValues &value) = 0;

    virtual ContentValues getContentValues(const T &model) = 0;

    virtual ContentMappings getAttrs() = 0;
};


#endif //APPSERVER_ADAPTER_H
