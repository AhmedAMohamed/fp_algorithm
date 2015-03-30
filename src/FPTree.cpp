//
// Created by karim on 3/26/15.
//

#include <iostream>
#include <queue>

#include "nvwa/debug_new.h"

#include "FPTree.h"



FPTree::FPTree(BaseTreeNode *root){


    header = new FPHeader();
    this->root = root;
}



FPTree::FPTree(const FPTree &other):
        FPTree(nullptr) {

}

FPTree &FPTree::operator=(const FPTree &other) {

    return *this;
}


FPTree::FPTree(FPTree &&other):
    header(other.header), root(other.root)
{
    other.header = nullptr;
    other.root = nullptr;

}

FPTree &FPTree::operator=(FPTree &&other) {
    if(this != &other)
    {
        destroy();

        header = other.header;
        root = other.root;

        other.header = nullptr;
        other.root = nullptr;

    }


    return *this;
}




FPTree FPTree::createInitialTree() {
    return FPTree(new InitialTreeNode(""));

}

FPTree FPTree::createConditionalTree(BaseTreeNode *original) {
    return FPTree(new ConditionalTreeNode("",original));
}


FPTree::~FPTree() {

    destroy();
}

void FPTree::addBasket(std::vector<std::string> basket) {

    //start at the root node
    InitialTreeNode* current = dynamic_cast<InitialTreeNode*>(root);
    // then loop over basket items one by one
    for(auto& item : basket)
    {
        //search for child node with item as key
        InitialTreeNode* iter = current->getChild(item);
        //if found increase and make the found node our next current node then resume the loop
        if(iter)
        {
            iter->increment(1);
            current = iter;
            continue;
        }

        //otherwise create a new node, make the header[item] sibling point to this new node
        //and make the new node sibling point to header[item] previous sibling
        HeaderNode* head = header->getItem(item);
        InitialTreeNode* current_header_sibling = dynamic_cast<InitialTreeNode*>(head->getSibling());
        InitialTreeNode* new_node = new InitialTreeNode(item, 1,current_header_sibling, current);
        head->setSibling(new_node);

        // add the new node the the current node children
        current->addChild(item,new_node);

        //advance
        current = new_node;

    }

}


FPHeader* FPTree::getHeader() {
    return header;
}




void FPTree::destroy() {
    // go over each header and delete all attached nodes
    if(header)
    {
        for(auto hn:header->getAscender())
        {
            BaseTreeNode* current = hn->getSibling();
            while(current)
            {
                BaseTreeNode* old = current;
                current = old->getSibling();
                delete old;

            }
        }
    }

    // then delete the root node
    delete root;
    delete header;
}

