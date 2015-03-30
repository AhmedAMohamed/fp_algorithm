//
// Created by karim on 3/26/15.
//

#ifndef _FP_GROWTH_FPALGORITHM_H_
#define _FP_GROWTH_FPALGORITHM_H_


#include <string>
#include <vector>
#include <functional>
#include "FPTree.h"
#include <map>

class FPHeader;
class FPAlgorithm {
public:

    struct RESULT_COMPARER{
        bool operator()(const std::string& a, const std::string& b)
        {
            if(a.size() == b.size())
                return a>b;

            return a.size()<b.size();
        }
    };

    using RESULT = std::map<std::string, std::map<std::string,int, RESULT_COMPARER>>;




private:
    std::string file_name;
    double minSup;
    int total_size;
    int minSupN;


public:

    FPAlgorithm(std::string file_name, double minSup);
    RESULT run();


private:
    void getFrequentOneItemSet(FPHeader& header);


    void forEachBasketInFile(std::function<void(std::vector<std::string>&)> func,
            std::function<bool(std::string&)> item_pred = std::function<bool(std::string&)>());


    void initializeTree(FPTree & tree);

    void mine(FPTree *tree, std::string suffix, RESULT &map);

    FPTree getConditionalTree(FPTree *tree, HeaderNode *node);

    void prune(FPTree *tree, FPHeader *conditional_header, HeaderNode *head);
};


#endif //_FP_GROWTH_FPALGORITHM_H_
