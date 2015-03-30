//
// Created by karim on 3/26/15.
//

#include <iostream>

#include "nvwa/debug_new.h"

#include "FPHeader.h"
#include "Node.h"

HeaderNode* FPHeader::incrementItem(std::string name, int count) {
    checkFreeze();
    auto iter = header.find(name);
    if(iter != header.end())
        iter->second.increment(count);
    else
    {
        auto iterr = header.emplace(name, HeaderNode(name, count));
        iter = iterr.first;
    }

    return &(iter->second);
}

FPHeader::ASCENDER FPHeader::getAscender() {
    freeze = true;
    if (invHeader.empty()) {
        createInverseTable();
    }

    return invHeader;
}

int FPHeader::getItemCount(std::string item) {
    auto iter = header.find(item);
    if(iter != header.end())
        return iter->second.getCount();

    return -1;
}

void FPHeader::createInverseTable() {
    for (auto &kn : header) {
        invHeader.insert(&kn.second);
    }
}

HeaderNode *FPHeader::getItem(std::string item) {
    auto iter = header.find(item);
    if(iter != header.end())
        return &(iter->second);

    return nullptr;
}

void FPHeader::removeItem(std::string name) {
    checkFreeze();

    auto iter = header.find(name);
    if(iter != header.end())
    {
        int count = iter->second.getCount();
        header.erase(iter);
    }
}

void FPHeader::checkFreeze() {
    if(freeze)
        throw std::runtime_error("Attempt to modify a header in freeze state");
}
