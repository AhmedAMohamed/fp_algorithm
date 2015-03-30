//
// Created by karim on 3/27/15.
//

#ifndef _FP_GROWTH_NODE_H_
#define _FP_GROWTH_NODE_H_


#include <string>
#include <unordered_map>
#include "FPTree.h"


class ConditionalTreeNode;
class InitialTreeNode;

class BaseTreeNode{
protected:
    std::string name;
    int count;

protected:
    ConditionalTreeNode* resource;

    explicit BaseTreeNode(const std::string& name, const int count=0)
            : name(name), count(count), resource(nullptr){}

public:

    virtual ~BaseTreeNode(){}

    const std::string getName() const {return name;}

    const int getCount() const {return count;}

    void increment(const int count = 1){this->count += count;}

    virtual BaseTreeNode* const getSibling()const=0;
    virtual BaseTreeNode*  getSibling()=0;

    virtual BaseTreeNode* const getParent()const=0;
    virtual BaseTreeNode*  getParent()=0;

    virtual void setParent( BaseTreeNode* node) = 0;

    ConditionalTreeNode * const getCopyNode() const {return resource;}
    ConditionalTreeNode *getCopyNode()  {return resource;}

    void setCopyNode(ConditionalTreeNode *resource) {this->resource = resource;}


};



class ConditionalTreeNode: public BaseTreeNode {
protected:
    ConditionalTreeNode* parent;
    ConditionalTreeNode* sibling;
    BaseTreeNode* original;

public:
    explicit ConditionalTreeNode(const std::string& name,BaseTreeNode * original, const int count=0)
        :BaseTreeNode(name, count),original(original)
    {parent = nullptr;}

    ~ConditionalTreeNode();
    ConditionalTreeNode * const  getParent() const {return parent;}
    ConditionalTreeNode *   getParent()  {return parent;}

    void setParent( BaseTreeNode* node);

    ConditionalTreeNode * const  getSibling() const {return sibling;}
    ConditionalTreeNode *   getSibling()  {return sibling;}

    void setSibling( ConditionalTreeNode* node){sibling = node;}


    BaseTreeNode * const  getOriginal() const {return original;}
    BaseTreeNode *   getOriginal()  {return original;}

    void setOriginal( BaseTreeNode* node){original = node;}







};

class InitialTreeNode: public BaseTreeNode {
protected:
    InitialTreeNode *parent;
    InitialTreeNode *sibling;
    std::unordered_map<std::string, InitialTreeNode*> children;


public:
    explicit InitialTreeNode(const std::string &name, const int count = 0,
             InitialTreeNode *sibling = nullptr,  InitialTreeNode* parent = nullptr)
            : BaseTreeNode(name, count), parent(parent), sibling(sibling) {

    }

    InitialTreeNode * const getParent() const {return parent;}
    InitialTreeNode *  getParent()  {return parent;}

    void setParent( BaseTreeNode *node) {parent = dynamic_cast<InitialTreeNode*>(node);}

    InitialTreeNode * const getSibling() const {return sibling;}
    InitialTreeNode *  getSibling()  {return sibling;}

    void setSibling( InitialTreeNode *node) {sibling = node;}



    void addChild(const std::string& name,  InitialTreeNode* node) {children[name] = node;}

    InitialTreeNode*  const getChild(const std::string& name) const;
    InitialTreeNode* getChild(const std::string& name);

};

#endif //_FP_GROWTH_NODE_H_
