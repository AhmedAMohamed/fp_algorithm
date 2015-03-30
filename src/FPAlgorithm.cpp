//
// Created by karim on 3/26/15.
//

#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>


#include "nvwa/debug_new.h"

#include "FPAlgorithm.h"
#include "Node.h"
#include "Utility.h"

FPAlgorithm::FPAlgorithm(std::string file_name, double minSup) {
    this->file_name = file_name;
    this->minSup = minSup/100;
    total_size = 0;
    minSupN = 0;
}


FPAlgorithm::RESULT FPAlgorithm::run() {
    // create a tree instance
    FPTree tree = FPTree::createInitialTree();
    // get the tree header where 1 frequent items sets will be stored too
    FPHeader* header = tree.getHeader();

    // do phase 1 : get first FI
    getFrequentOneItemSet(*header);

    //initialize the tree[using second pass on our data]
    initializeTree(tree);

    RESULT result;

    mine(&tree, "", result);
    return result;
}

void FPAlgorithm::getFrequentOneItemSet(FPHeader& header) {
    std::unordered_map<std::string, int> counter;

    // For each basket, for each item in basket add to hash and increment count
    forEachBasketInFile(
            [&counter,this] (std::vector<std::string>& basket) {
                for(auto& item : basket)
                {
                    auto iter = counter.find(item);
                    if(iter != counter.end())
                        iter->second++;
                    else
                        counter[item] = 1;
                }
                total_size++;

            }
    );

    minSupN = minSup * total_size;

    // Now create a binary tree with frequent item sets
    // this will be the header for the initial fp tree
    for(auto& kv : counter)
    {
        if(kv.second >= minSupN)
        {
            header.incrementItem(kv.first, kv.second);
        }
    }



}

void FPAlgorithm::initializeTree(FPTree &tree) {
    // pass on baskets ignoring basket items that are not frequent
    forEachBasketInFile(
            [&tree](std::vector<std::string>& basket)
            {
                //sort basket according to item's support
                std::sort(basket.begin(), basket.end(),[&tree](std::string& a, std::string& b){
                    FPHeader* header = tree.getHeader();
                    return header->getItemCount(a) > header->getItemCount(b); }
                );

                //add the basket to the tree
                tree.addBasket(basket);

            },
            //predicate function to ignore non freq items when constructing basket from file
            [&tree](std::string& token) -> bool
            {
                return tree.getHeader()->getItemCount(token) > -1;
            }
    );

}


void FPAlgorithm::forEachBasketInFile(std::function<void(std::vector<std::string>&)> func,
        std::function<bool(std::string&)> item_pred) {

    std::ifstream infile(file_name);
    if(!infile.is_open())
    {
        throw std::runtime_error("Couldn't open file " + file_name + ".");
    }
    std::string line;
    //loop each line
    while (std::getline(infile, line))
    {
        std::vector<std::string> basket;
        // extract tokens from each line and put in token vector according to some predicate
        Utility::split(line, ',', basket, item_pred);
        // call user function on the tokens vector
        func(basket);

    }
    infile.close();
}


void FPAlgorithm::mine(FPTree *tree, std::string suffix, FPAlgorithm::RESULT &map) {
    for(auto hn : tree->getHeader()->getAscender())
    {
        auto& values = map[std::string(1,(hn->getName()+suffix).back())];
        values.insert(RESULT::mapped_type::value_type(hn->getName() + suffix, hn->getCount()));
        FPTree new_tree = getPrefixTree(tree, hn);
        mine(&new_tree,hn->getName()+suffix, map);
    }
}

FPTree FPAlgorithm::getPrefixTree(FPTree *tree, HeaderNode *header) {

    //create the conditional tree and fetch its root
    FPTree conditional_tree = FPTree::createConditionalTree(nullptr);
    ConditionalTreeNode* conditional_tree_root = (ConditionalTreeNode*)conditional_tree.getRoot();

    //get the original tree root and make its resource point to our conditional tree root
    tree->getRoot()->setCopyNode(conditional_tree_root);

    // get the first linked node to the header
    BaseTreeNode*condition_node = header->getSibling();

    //loop over siblings of current header node
    //(each loop represent going over an itemset in reverse order)
    while(condition_node)
    {
        //set the increment to be the count of the current condition node
        //so if we have a duplicate basket, we manage to calculate the new count in one pass
        int inc_value = condition_node->getCount();

        //start ascending from the node just before the condition node
        //(we dont want to create the condition node in the conditional tree
        //as we already know it is frequent)
        BaseTreeNode* current = condition_node->getParent();
        //hold a prev node to update the parent of the conditonal tree nodes correctly
        BaseTreeNode* prev = nullptr;

        //ascend the tree and create copies in copy nodes
        //our condition is we stop if the current node is the parent node
        while(current->getParent())
        {
            ConditionalTreeNode* copied = nullptr;
            //we now check if the current copy node is not created for this tree
            if(!current->getCopyNode()){
                //Create the new node, setting its count to increment and its original node to current
                copied = new ConditionalTreeNode(current->getName(),current, inc_value);
                //set the copy node to the new node
                current->setCopyNode(copied);

                //add/increment header
                HeaderNode* cond_header = conditional_tree.getHeader()->incrementItem(copied->getName(), inc_value);

                //now put the new node in the begining of the linked list after header
                BaseTreeNode* prev_linked = cond_header->getSibling();
                cond_header->setSibling(copied);
                copied->setSibling( (ConditionalTreeNode*)(prev_linked));

            }
            // otherwise just increment its count & increment header
            else
            {
                copied = current->getCopyNode();
                copied->increment(inc_value);
                conditional_tree.getHeader()->incrementItem(copied->getName(),inc_value);
            }

            //adjust the previous node parent
            if(prev)
                prev->setParent(copied);

            current = current->getParent();
            prev = copied;
        }
        //we always get out when we are at root node
        //so we need to check if there is a prev node with parent not set
        //adjust the prev node to point to the conditional root & set it to the root
        if(prev)
            prev->setParent(conditional_tree_root);


        condition_node = condition_node->getSibling();
    }

    //remove items with low support and reset resource pointers to null to detach conditional tree from current tree
    prune(&conditional_tree, conditional_tree.getHeader(), header);
    return conditional_tree;
}

void FPAlgorithm::prune(FPTree *tree, FPHeader *conditional_header, HeaderNode *head) {
    //Here we loop over the whole created conditional tree again and check each node see if its support is low
    //from the tree header if so remove it and adjust the child parent by keeping a previous pointer throughout
    //the traversal


    // get first linked condition node
    BaseTreeNode*condition_node = head->getSibling();

    //pass over the conditional tree from the original tree
    while(condition_node)
    {
        //start ascending from the node just before the condition node
        BaseTreeNode* current = condition_node->getParent();
        //hold a prev node to update the parent of the conditonal tree nodes correctly when deleting current node
        BaseTreeNode* prev = nullptr;

        bool prev_in_search_for_parent = false;

        //our condition is we stop if the current node is the parent node
        while(current->getParent())
        {
            //get the conditional tree node
            ConditionalTreeNode* copied = current->getCopyNode();


            //check that a copy exists (a copy does not exist if it was deleted in a previous itemset traversal)
            //wee need to check that because if it is null this mean we are in another itemset
            //traversal and the previous node used to have the deleted copy node as its parent, so we start a search
            //for parent state for the previous node in this traverse
            if(!copied)
            {
                //start searching for parent
                prev_in_search_for_parent = true;
            }
            else {
                //we have found the copy node

                //check if the previous node had an in search for a parent state
                if(prev_in_search_for_parent)
                {
                    //set its parent to the current copy node
                    prev->setParent(copied);
                    //and reset the search for parent
                    prev_in_search_for_parent = false;
                }

                //check if the node in the copy node has support less than the minimum support
                HeaderNode *cond_head = conditional_header->getItem(copied->getName());
                if (cond_head->getCount() < minSupN) {
                    //low support we need to remove it

                    //decrement current head count
                    cond_head->increment(-1);

                    //check if head for this copy node reached 0 this means no more nodes of this type in the current
                    //conditional tree, so we can remove the header safely
                    if (cond_head->getCount() == 0) {
                        conditional_header->removeItem(copied->getName());
                    }

                    //adjust the child node ptr if needed
                    if (prev) {
                        prev->setParent(copied->getParent());
                    }

                    //delete node
                    delete copied;
                    copied = nullptr;

                }
            }

            //ascend from the original tree
            current = current->getParent();

            //update previous only if copied exist
            if(copied)
            {
               prev = copied;
            }

        }
        //if we ended the current itemset traverse and there is a node still in search for parent this means all of its
        //parents till the root were deleted previously so we need to set its parent to the root node
        if(prev_in_search_for_parent)
        {
            prev->setParent(tree->getRoot());

        }

        condition_node = condition_node->getSibling();
    }


}
