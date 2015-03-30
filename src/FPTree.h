//
// Created by karim on 3/26/15.
//

#ifndef _FP_GROWTH_FPTREE_H_
#define _FP_GROWTH_FPTREE_H_
#include <unordered_map>
#include <string>
#include "FPHeader.h"
#include "Node.h"
#include <vector>

class BaseTreeNode;
class HeaderNode;
class ConditionalTreeNode;


class FPTree {


private:


    FPHeader* header;
    BaseTreeNode* root;

    FPTree(BaseTreeNode* root);
public:

    FPTree(const FPTree& other);
    //assign operator
    FPTree& operator=(const FPTree& other);

    //move constructor
    FPTree(FPTree&& other);
    //move assign operator
    FPTree& operator=(FPTree&& other);


    ~FPTree();

    static FPTree createInitialTree();

    static FPTree createConditionalTree(BaseTreeNode *original);

    void addBasket(std::vector<std::string> basket);


    FPHeader* getHeader();

    BaseTreeNode* getRoot(){return root;}



private:
    void destroy();

};


#endif //_FP_GROWTH_FPTREE_H_
