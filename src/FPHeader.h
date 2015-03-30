//
// Created by karim on 3/26/15.
//

#ifndef _FP_GROWTH_FPHEADER_H_
#define _FP_GROWTH_FPHEADER_H_



#include <set>
#include <unordered_map>
#include <functional>


class BaseTreeNode;

class HeaderNode {

public:
    struct PTR_COMPARE_ON_SUPPORT_ASC{
        bool operator()(const HeaderNode* a, const HeaderNode* b)
        {
            if(a->getCount() == b->getCount())
                return a->getName() < b->getName();

            return a->getCount() < b->getCount();
        }
    };

protected:
    std::string name;
    int count;
    BaseTreeNode* sibling;

public:

    HeaderNode()
            :name(""), count(0), sibling(nullptr)
    {}
    explicit HeaderNode(const std::string& name, const int count=0,
            BaseTreeNode *sibling= nullptr)
            : name(name), count(count), sibling(sibling) {}


    bool operator==(const HeaderNode& other){return other.name == this->name;}

    const std::string getName() const {return name;}

    void setName(std::string name) {this->name = name;}


    const int getCount() const {return count;}

    BaseTreeNode *  const getSibling() const {return sibling;}
    BaseTreeNode *   getSibling()  {return sibling;}

    void setSibling(BaseTreeNode* node){sibling = node;}

    void increment(const int count = 1){this->count += count;}

};


class FPHeader {
public:
    using ASCENDER = std::set<HeaderNode*, HeaderNode::PTR_COMPARE_ON_SUPPORT_ASC>;
    
private:
    bool freeze;
    std::unordered_map<std::string,HeaderNode> header;
    ASCENDER invHeader;

public:

    FPHeader():freeze(false)
    {}

    HeaderNode* getItem(std::string name);

    int getItemCount(std::string name);

    void removeItem(std::string name);

    HeaderNode* incrementItem(std::string, int count = 1);


    FPHeader::ASCENDER getAscender();



private:
    void createInverseTable();
    void checkFreeze();
};



#endif //_FP_GROWTH_FPHEADER_H_
