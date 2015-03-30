//
// Created by karim on 3/26/15.
//

#include "Utility.h"
#include <sstream>

std::vector<std::string>& Utility::split(const std::string &s, char delim, std::vector<std::string> &elems,
        std::function<bool(std::string&)> pred) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        std::string tmp = trim(item);
        if(!pred || pred(tmp))
        {
            elems.push_back(tmp);
        }
    }
    return elems;
}


std::string Utility::trim(const std::string& str, const std::string& whitespace)
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}
