//
// Created by Artsiom Kaliaha on 4/28/20.
//

#ifndef WEBSERVER_MAPPER_H
#define WEBSERVER_MAPPER_H

class IMapper {
};

template<typename TResponse, typename TModel>
class Mapper : public IMapper {
    virtual TModel getModel(const TResponse& response) = 0;
};


#endif //WEBSERVER_MAPPER_H
