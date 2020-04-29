//
// Created by Artsiom Kaliaha on 4/29/20.
//

#ifndef WEBSERVER_HELPER_CPP
#define WEBSERVER_HELPER_CPP

#include <string>
#include <fstream>
#include <functional>

#include <mstch/mstch.hpp>

using namespace std;

template<typename TModel>
string processTemplate(const string &fileName,
                       const string &allItems,
                       const string &singleItem,
                       const string &itemView,
                       const vector<TModel> items,
                       function<mstch::map(const TModel &)> modelMapper) {
    ifstream file("../html/" + fileName + ".html");
    string html((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    mstch::array list;
    for (const TModel &item : items) {
        list.push_back(modelMapper(item));
    }
    mstch::map context{{allItems, list}};

    return mstch::render(html, context, {{singleItem, itemView}});
}

#endif //WEBSERVER_HELPER_CPP
