//
// Created by Artsiom Kaliaha on 4/29/20.
//

#ifndef WEBSERVER_HELPER_CPP
#define WEBSERVER_HELPER_CPP

#include <string>
#include <fstream>
#include <functional>

#include <mstch/mstch.hpp>

#define NUM_SECONDS_PER_DAY (24U * 60U * 60U)
#define CASS_DATE_EPOCH 2147483648U // 2^31
#define CASS_TIME_NANOSECONDS_PER_SECOND 1000000000LL

static std::string readHtmlFile(const std::string &fileName) {
    std::ifstream file("../html/" + fileName + ".html");
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

template<typename TModel>
static std::string processTemplate(const std::string &fileName,
                                   const std::string &allItems,
                                   const std::string &singleItem,
                                   const std::string &itemViewFileName,
                                   const std::vector<TModel> items,
                                   std::function<mstch::map(const TModel &)> modelMapper,
                                   const mstch::map &additionalItems = {}) {
    std::string html = readHtmlFile(fileName);

    mstch::array list;
    for (const TModel &item : items) {
        list.push_back(modelMapper(item));
    }
    mstch::map context{{allItems, list}};

    for (const auto &pair : additionalItems) {
        context.insert(pair);
    }

    std::string itemViewHtml = readHtmlFile(itemViewFileName);
    return mstch::render(html, context, {{singleItem, itemViewHtml}});
}

static std::string cassandraTimeToString(uint32_t date, int64_t time) {
    time_t t = (static_cast<uint64_t>(date) - CASS_DATE_EPOCH) * NUM_SECONDS_PER_DAY +
               time / CASS_TIME_NANOSECONDS_PER_SECOND;
    return asctime(gmtime(&t));
}

#endif //WEBSERVER_HELPER_CPP
