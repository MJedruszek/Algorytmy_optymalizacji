#include "product.h"
#include <limits.h>
#include <stack>

enum Version{
    NORMAL,
    GREEDY,
    REHEAT,
    BOTH
};

class Solution{
    private:
        std::vector<Product> problem;
        int Cmax;
        std::vector<int> result; //przechowujemy kombinację, która oznacza kolejność w rozwiązaniu
        int m; //ile kroków ma jeden produkt/ile maszyn należy wykorzystać
        std::vector<std::vector<int>> delays; //macierz opoznien pomiedzy Jobi i Jobj -> delays[i][j]
        std::vector<int> totalP; //tab dla czasow wykonywania dla job na wszystkich maszynach
        int sumOfP(int i); //oblicza czas dla maszyny i
        int countCmax();
        void fillTestData();
        void print();
        void bruteforce();
        void readFromFile(std::string fname);
        //sąsiedztwo typu swap, dwie różne liczby
        std::vector<int> generateRandomNeigh(std::vector<int> curr);
        std::vector<std::vector<int>> generateNeighbours(std::vector<int> curr, int size);
        std::vector<int> generateBestNeighbour(std::vector<int> curr, int size);
        std::pair<int, int> findSwapped(std::vector<int> curr, std::vector<int> neigh);
        void QNEH();
        //wyżarzanie
        void simulatedAnnealing(float initial_temp, float final_temp, int max_iterations, Version v, int n);
        //B&B
        struct Node {
            std::vector<bool> scheduled; //czy zadanie i jest przypisane?
            std::vector<int> path; //uszeregowane zadania
            int lowerBound;
            int lvl;
            
            // Do priority queue
            bool operator>(const Node& other) const {
                return lowerBound > other.lowerBound;
            }

            Node(int n): scheduled(n, false), lvl(0), lowerBound(0){};
        };
        int calculateLowerBound(const Node& node);
        void branchAndBound();

        //LICZENIE CMAX DLA NO WAIT FLOW SHOP
        int noWaitCmax(); //liczenie Cmaxa klasycznie
        int calculateDelay(int job1, int job2);
        void calculateDelayMatrix();
        int calculateSwap(int pos1, int pos2);
        int cmaxDelays();
    public:
        Solution();
        void menu();
        void annealingTest(std::string filename);
        void noWaitCmaxTest();
};