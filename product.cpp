#include "product.h"


Product::Product(std::vector<int> p, int s){
    for(int i = 0; i<s; i++){
        pj.push_back(p[i]);
        finished.push_back(0);//zapełnimy zerami, żeby potem łatwo można było przypisać czas zakończenia zadani
    }
    size = s;
}

void Product::print(){
    for(int i = 0; i<size; i++){
        std::cout<<"P"<<i<<": "<<pj[i]<<", ";
    }
    std::cout<<std::endl;
}

int Product::getProductPj(int i){
    return pj[i];
}

void Product::setProductPj(int i, int p){
    pj[i]=p;
}

void Product::clearFinished(){
    for(int i = 0; i<size; i++){
        finished[i]=0;
    }
}