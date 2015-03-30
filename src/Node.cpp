//
// Created by karim on 3/27/15.
//
#include "Node.h"
#include "nvwa/debug_new.h"

InitialTreeNode * const InitialTreeNode::getChild(const std::string &name) const{
    auto child = children.find(name);

    return child != children.end() ? child->second: nullptr;
}

InitialTreeNode *InitialTreeNode::getChild(const std::string &name) {

    auto child = children.find(name);

    return child != children.end() ? child->second: nullptr;


}

void ConditionalTreeNode::setParent(BaseTreeNode *node) {
    parent = (ConditionalTreeNode*)(node);

}

ConditionalTreeNode::~ConditionalTreeNode() {

    if(original)
        original->setCopyNode(nullptr);

}
