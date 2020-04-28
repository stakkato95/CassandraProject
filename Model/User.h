//
// Created by Artsiom Kaliaha on 4/28/20.
//

#ifndef APPSERVER_USER_CPP
#define APPSERVER_USER_CPP

#include <iostream>
#include <string>

struct User {
    std::string lastname;
    int age;
    std::string city;
    std::string email;
    std::string firstname;
};

//std::ostream &operator<<(std::ostream &os, const User &user) {
//    os << "["
//       << user.firstname << ", "
//       << user.age << ", "
//       << user.city << ", "
//       << user.email << ", "
//       << user.firstname;
//
//    return os;
//}

#endif //APPSERVER_USER_CPP
