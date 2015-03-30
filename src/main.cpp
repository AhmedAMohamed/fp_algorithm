#include <iostream>


#include "nvwa/debug_new.h"
#include "FPAlgorithm.h"


int main(int argc, char**argv) {

    if(argc < 3)
    {
        std::cout<<"Error: "<<"data file & minimum support are required as arguments.\n";
        return 1;
    }

    //check parameter
    double minsup;
    if(sscanf(argv[2],"%lf",&minsup) != 1)
    {
        std::cout<<"Error: "<<"second argument should be a floating point(minimum support argument).\n";
        return 1;
    }

    //initialzie algorithm
    FPAlgorithm fp(argv[1], minsup);

    
    try{
        //run the algorithm
        auto out = fp.run();

        //output the results
        for(auto& kv:out)
        {
            std::cout<<kv.first<< ": ";
            for(auto& _kv:kv.second) {
                std::cout<<_kv.first<< "("<<_kv.second<<"), ";
            }
            std::cout<<"\n";
        }
    }
    catch(const std::exception& e)
    {
        std::cout<<"Error: "<<"\n"<<e.what()<<"\n";
        return 1;
    }

    



    return 0;

}