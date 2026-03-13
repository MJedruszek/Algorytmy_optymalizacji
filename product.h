#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <queue>
#include <ctime>
#include <random>

class Product{
    private:
        std::vector<int> pj;
        std::vector<int> finished;
        int size;
    public:
        Product(std::vector<int> p, int s);
        void print();
        int getProductPj(int i); //zwracamy pj dla danego kroku i
        void setProductPj(int i, int p); //nadajemy odpowiedniemu krokowi i zadane pj
        int getProductFinished(int i){return finished[i];};
        void setProductFinished(int i, int t){finished[i]=t;};
        void clearFinished();
};