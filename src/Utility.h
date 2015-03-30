//
// Created by karim on 3/26/15.
//

#ifndef _FP_GROWTH_UTILITY_H_
#define _FP_GROWTH_UTILITY_H_
#include <string>
#include <vector>
#include <functional>
namespace Utility{

    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems,
            std::function<bool(std::string&)> pred);
    std::string trim(const std::string& str, const std::string& whitespace = " \t");

};



#endif //_FP_GROWTH_UTILITY_H_
