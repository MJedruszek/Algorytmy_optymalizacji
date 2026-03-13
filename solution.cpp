#include "solution.h"

Solution::Solution(){
    fillTestData();
}

void Solution::fillTestData(){
    problem.clear();
    Cmax = 0;
    std::vector<int> pj;
    pj.push_back(3);
    pj.push_back(2);
    pj.push_back(4);
    
    Product p1(pj, 3);
    pj[0]=1;
    pj[1]=4;
    pj[2]=6;
    Product p2(pj, 3);
    pj[0]=3;
    pj[1]=7;
    pj[2]=1;
    Product p3(pj, 3);
    pj[0]=2;
    pj[1]=5;
    pj[2]=1;
    Product p4(pj, 3);
    problem.push_back(p1);
    problem.push_back(p2);
    problem.push_back(p3);
    problem.push_back(p4);
    m=3;
    //przykładowe uzupełnienie
    result.push_back(0);
    result.push_back(1);
    result.push_back(2);
    result.push_back(3);
    // Cmax = countCmax();
}

void Solution::readFromFile(std::string fname){
    problem.clear();
    result.clear();
    Cmax = 0;
    int size = 0;
    std::string tmp;
    std::ifstream file(fname);
    //pobierz rozmiar
    getline(file, tmp, ' ');
    size = std::stoi(tmp);
    //pobierz liczbę kroków
    getline(file, tmp);
    m = std::stoi(tmp);
    //kolejno pobierz dane z pliku i zapisz je do wektora
    for(int i = 0; i<size; i++){
        std::vector<int> pj;
        for(int j=0; j<m-1; j++){
            getline(file, tmp, ' ');
            pj.push_back(std::stoi(tmp));
        }      
        getline(file, tmp);  
        pj.push_back(std::stoi(tmp));
        Product p(pj, m);
        problem.push_back(p);
        result.push_back(i);
    }
    file.close();
}

void Solution::print(){
    std::cout<<"Cmax = "<<Cmax<<std::endl;
    for(int i = 0; i<result.size(); i++){
        std::cout<<"Produkt nr: "<<result[i]<<": ";
        problem[result[i]].print();
    }
}

int Solution::sumOfP(int i){
    int s = 0;
    if(i==0){
        for(int j = 0; j<result.size(); j++){
            s+=problem[result[j]].getProductPj(i);//zadanie j zakończy się w czasie s po dodaniu Pj
            problem[result[j]].setProductFinished(i,s); //zapisujemy, że zadanie i zostało zakónczone w chwili s
        }
    }
    else{
       
        for(int j = 0; j<result.size(); j++){
            //czas rozpoczęcia zadnia j, równy czasowi zakończenia zadania j-1 na maszynie i lub 
            //czasowi zakończenia zadania j na maszynie i-1
            s = std::max(s, problem[result[j]].getProductFinished(i-1));
            s+=problem[result[j]].getProductPj(i); //do czasu zaczęcia dodajemy czas wykonywania, mamy czas zakończenia
            problem[result[j]].setProductFinished(i,s); //zapisujemy czas zakońćzenia tego zadania na maszynie i
        }
    }
    return s;
}

int Solution::countCmax(){
    int C = 0;
    for(int i = 0; i<problem.size(); i++){
        problem[i].clearFinished();
    }
    for(int i = 0; i<m; i++){
        C = std::max(C, sumOfP(i));//wybierz, czy większe jest aktualne C, czy suma dla aktualnej maszyny
    }
    return C; //zwróć max
}

void Solution::bruteforce(){
    std::vector<int> tmp;
    int tmp_size = problem.size();
    for(int i = 0; i<tmp_size; i++){
        tmp.push_back(i);
    }
    result = tmp;
    int tmp_c = INT16_MAX; //Duża liczba, do niej przyrównamy Cmax poszukując najmniejszego
    do{
        int c = countCmax();         
        if(c<tmp_c){
            tmp_c = c;
            tmp = result;
        }
        //tmp_c = std::min(tmp_c, Cmax); //chyba niepotrzebne?
    } while(std::next_permutation(result.begin(), result.end()));
    //przepisujemy wartości zdobyte w trakcie działania do wartości s oraz Cmax
    result = tmp;
    Cmax = tmp_c;
}

std::vector<int> Solution::generateRandomNeigh(std::vector<int> curr){
    int i = std::rand()%(problem.size()); //wygeneruj losowy indeks od 0 do n
    int j = std::rand()%problem.size(); //wygeneruj drugi losowy indeks od 0 do n
    while(j==i) j = std::rand()%problem.size();
    std::vector<int> new_n = curr;
    new_n[i] = curr[j];
    new_n[j] = curr[i];
    return new_n;
}

std::vector<std::vector<int>> Solution::generateNeighbours(std::vector<int> curr, int size){
    //Tworzymy size sąsiadów dla aktualnego rozwiązania curr
    std::vector<std::vector<int>> neighs;
    for(int i = 0; i<size; i++){
        std::vector<int> neigh = generateRandomNeigh(curr);
        neighs.push_back(neigh);
    }
    //zwracamy całe wygenerowane sąsiedztwo
    return neighs;
}

std::pair<int, int> Solution::findSwapped(std::vector<int> curr, std::vector<int> neigh){
    std::pair<int,int> found = std::pair(-1,-1);
    for(int i = 0; i<curr.size(); i++){
        if(curr[i]!=neigh[i]){
            if(found.first == -1) found.first = i;
            else found.second = i;
        }
    }
    return found;
}

void Solution::QNEH(){
    std::vector<std::vector<std::pair<int,int>>> times;
    std::vector<std::pair<int, int>> first;
    first.resize(m);

    result.clear();
    std::vector<int> sorted;

    for(int i = 0; i<problem.size(); i++){
        sorted.push_back(i);
    }

    //posortuj nierosnąco po sumie pj
    std::sort(sorted.begin(), sorted.end(), [this](int p1, int p2) {
        int sum1 = 0;
        for(int i = 0; i<m; i++){
            sum1+=problem[p1].getProductPj(i);
        }
        int sum2 = 0;
        for(int i = 0; i<m; i++){
            sum2+=problem[p2].getProductPj(i);
        }
        return sum1 > sum2;
    });
    result.push_back(sorted[0]);
    //pierwsza kolumna, w przód
    
    for(int i = 0; i<m; i++){
        if(i==0){
            first[0] = (std::make_pair(problem[sorted[0]].getProductPj(0), 0));
            
        }
        else{
            first[i] = (std::make_pair(problem[sorted[0]].getProductPj(i)+first[i-1].first, 0));
        }
    }
    
    times.push_back(first);
    
    //pierwsza kolumna, w tył
    for(int i = m-1; i>=0; i--){
        if(i==m-1){
            times[0][m-1].second=problem[sorted[0]].getProductPj(m-1);
        }
        else{
            times[0][i].second=problem[sorted[0]].getProductPj(i)+times[0][i+1].second;
        }
    }
    
    //w pętli dodajemy obiekty i sprawdzamy, jakie wyjdzie Cmax
    for(int p = 1; p<problem.size(); p++){
        //inicjalizacja zmiennej pomocniczej do przechowywania Cmax
        int tmp_c = INT16_MAX;
        int pos = 0; //pozycja, na której należy umieścić obiekt o numerze p
        std::vector<std::pair<int,int>> s;
        s.resize(m);
        for(int k = 0; k<result.size()+1; k++){
            if(k==result.size()){
                std::vector<std::pair<int, int>> val;
                val.resize(m);
                //jesteśmy w ostatniej kolumnie, Cmax to max(suma pj w przód + aktualne Pj) spośród wszystkich maszyn
                int c = 0;
                for(int j = 0; j<m; j++){
                    if(j==0) {
                        val[j].first=times[k-1][j].first + problem[sorted[p]].getProductPj(j);
                    }
                    else {
                        val[j].first=std::max(times[k-1][j].first, val[j-1].first)+problem[sorted[p]].getProductPj(j);
                    }
                    //std::cout<<p<<": "<<times[k-1][j].first<<" "<<times[k-1][j].second<<std::endl;
                    c = std::max(c, val[j].first);
                } 
                //znaleźliśmy lepszą pozycję, zapisujemy
                if(c<tmp_c){
                    tmp_c = c;
                    pos = k;
                    s=val;
                    // std::cout<<k<<": "<<c<<std::endl;
                }
                
            }
            else{
                //dla każdej innej pozycji, oblicz wartość w przód
                std::vector<std::pair<int, int>> val;
                val.resize(m);
                for(int i = 0; i<m; i++){
                    if(i==0){
                        if(k==0){
                            //pierwsza kolumna
                            val[0].first = problem[sorted[p]].getProductPj(i);
                        }
                        else{
                            //w przód, na pozycji zerowej, suma tego, co było poprzednio, i aktualnego Pj
                            val[0].first = times[k-1][0].first+problem[sorted[p]].getProductPj(i); 
                            //std::cout<<p<<": "<<times[k-1][0].first<<" "<<times[k-1][0].second<<std::endl;
                        }
                    }
                    else{
                        if(k==0){
                            //pierwsza kolumna
                            val[i].first = val[i-1].first+problem[sorted[p]].getProductPj(i);
                            
                        }
                        else{
                            //w przód, na kolejnych pozycjach, suma max(było, poprzednia maszyna) i aktualnego Pj
                            val[i].first = std::max(times[k-1][i].first, val[i-1].first) + problem[sorted[p]].getProductPj(i);
                             //std::cout<<p<<": "<<times[k-1][i].first<<" "<<times[k-1][i].second<<std::endl;
                        } 
                    }
                }
                int c = 0;
                //szukamy Cmax
                
                for(int j = 0; j<m; j++){
                    //w tył dla maszyny jeden z przodu i w przód dla naszej aktualnej maszyny
                    if(k==times.size()){
                        //ostatni element
                        c=std::max(c,val[j].first);
                    }
                    else{
                        c = std::max(c, times[k][j].second + val[j].first);
                        //std::cout<<times[k][j].second<<"+"<<val[j].first <<"="<<times[k][j].second + val[j].first<<std::endl;
                    }
                    // std::cout<<k<<": "<<times[k+1][j].second<<", "<<val[j].first<<std::endl;
                }
                if(c<tmp_c){
                    tmp_c = c;
                    pos = k;
                    s=val;
                    // std::cout<<k<<": "<<c<<std::endl;
                }
                
            }
        }
        //zabieramy naszą najlepszą znalezioną pozycję ze sobą
        //zapisujemy w result i w times
        result.emplace(result.begin()+pos, sorted[p]);
        times.emplace(times.begin() + pos, s);
        //Cmax = countCmax();
        //obliczamy w przód i w tył tam, gdzie trzeba, ponownie
        //kolejność pól w times jest taka sama, jak kolejność pól w result: result pamięta wartość naszego aktualnego pola
        //w przód: produkt p ma już obliczone i zapisane, liczymy dla pozycji pos+1 i dalej, do końca result
        for(int i = pos; i<times.size(); i++){
            for (int j = 0; j < m; j++) {
                if (i == 0 && j == 0) {
                    times[i][j].first = problem[result[i]].getProductPj(j);
                } else if (i == 0) {
                    times[i][j].first = problem[result[i]].getProductPj(j) + times[i][j-1].first;
                } else if (j == 0) {
                    times[i][j].first = problem[result[i]].getProductPj(j) + times[i-1][j].first;
                } else {
                    times[i][j].first = problem[result[i]].getProductPj(j) + 
                                        std::max(times[i][j-1].first, times[i-1][j].first);
                }
            }
        }

        //w tył: produkt p nie ma obliczonego i zapisanego, więc liczymy to i tak - nie ma przypadku, gdzie nie liczymy nic
        //idziemy od tyłu, od kolumny pos do kolumny 0 włącznie
        //jeśli jesteśmy w ostatniej kolumnie, dodajemy wprost wartości z Pj   
        for(int i = pos; i>=0; i--){
            for (int j = m-1; j >= 0; j--) {
                if (i == times.size() - 1 && j == m - 1) {
                    times[i][j].second = problem[result[i]].getProductPj(j);
                } else if (i == times.size() - 1) {
                    times[i][j].second = problem[result[i]].getProductPj(j) + times[i][j+1].second;
                } else if (j == m - 1) {
                    times[i][j].second = problem[result[i]].getProductPj(j) + times[i+1][j].second;
                } else {
                    times[i][j].second = problem[result[i]].getProductPj(j) + std::max(times[i][j+1].second, times[i+1][j].second);
                }
            }
        }
    }
    
    Cmax = countCmax();
}

void Solution::simulatedAnnealing(float initial_temp, float final_temp, int max_iterations){
    QNEH();
    //neh
    std::vector<int> best_sequence = result;
    result = best_sequence;
    int best_cmax = countCmax();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    //zgodnie ze wzorem z prezentacji
    double cooling_rate = powf((final_temp/initial_temp), (1.0/max_iterations));
    // std::cout<<cooling_rate<<std::endl;
    // std::cout<<final_temp/initial_temp<<std::endl;

    std::vector<int> current_sequence = best_sequence;
    int current_cmax = best_cmax;

    float temp = initial_temp;
    int i = 0;

    while(temp>final_temp && i<max_iterations){
        
        //tworzymy losowego sąsiada
        std::vector<int> neigh = generateRandomNeigh(current_sequence);
        result = neigh;
        int tmp_c = countCmax();

        int diff = tmp_c - current_cmax;
        double r = dis(gen);

        //std::cout<<diff<<" "<<temp<<std::endl;
        double pow = (diff * 1.0)/(temp * 1.0);
        double prob = 1/(1+std::exp(pow));
        //dwa warunki: albo lepsze rozwiązanie, albo spełnia kryterium akceptacji (spełnia warunek, sytuacja o prawdopodobieństwie
        //zgodnym z kryterium Metropolisa, bo dis(gen) to liczba od 0 do 1)
        if(diff<0 || r < prob){
            current_sequence = neigh;
            current_cmax = tmp_c;
            //znaleźliśmy rozwiązanie lepsze od poprzedniego? zapisujemy
            if(current_cmax<best_cmax){
                best_cmax = current_cmax;
                best_sequence = current_sequence;
                //std::cout<<"HERE"<<std::endl;
            }
        }
        temp = temp * cooling_rate;
        i++;
    }
    Cmax = best_cmax;
    result = best_sequence;
}

void Solution::menu(){
    srand(time(0));
    std::string filename;
    std::cout<<"Wpisz nazwę pliku, który chcesz rozwiązać"<<std::endl;
    std::cin>>filename;
    readFromFile(filename);
    if(problem.size() <= 0){
        std::cout<<"Nieprawidłowa nazwa pliku lub plik pusty"<<std::endl;
        return;
    }
    char choice = '-';
    
    std::string tmp;
    //do tabu
    int max_iters = problem.size()*m*200;
    int tabu_size = problem.size();
    int neigh_size = problem.size();
    float initial_temp = 1000000.0;
    float final_temp = 0.001;

    while(choice != 'q'){
        std::cout<<"Wybierz, jakim algorytmem chcesz rozwiązać zadanie: "<<std::endl;
        std::cout<<"0 - QNEH"<<std::endl;
        std::cout<<"1 - bruteforce, czyli pszeszukanie wszystkich permutacji"<<std::endl;
        std::cout<<"2 - symulowane wyżarzanie"<<std::endl;
        std::cout<<"Aby wyświetlić ostatnie zapisane rozwiązanie, wpisz p"<<std::endl;
        std::cout<<"Aby wyjść, wpisz q"<<std::endl;
        std::cin>>choice;
        auto t1 = std::chrono::high_resolution_clock::now();
        auto t2 = std::chrono::high_resolution_clock::now();
        auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        switch(choice){
            case '0':
                std::cout<<"-------------------------"<<std::endl;
                t1 = std::chrono::high_resolution_clock::now();
                QNEH();
                t2 = std::chrono::high_resolution_clock::now();
                ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
                std::cout<<ms_int.count()<<" w milisekundach"<<std::endl;
                std::cout<<"Rozwiązano algorytmem QNEH, Cmax = "<<Cmax<<std::endl;
                std::cout<<"-------------------------"<<std::endl;
                break;
            case '1':
                std::cout<<"-------------------------"<<std::endl;
                t1 = std::chrono::high_resolution_clock::now();
                bruteforce();
                t2 = std::chrono::high_resolution_clock::now();
                ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
                std::cout<<ms_int.count()<<" w milisekundach"<<std::endl;
                std::cout<<"Rozwiązano metodą bruteforce, Cmax = "<<Cmax<<std::endl;
                std::cout<<"-------------------------"<<std::endl;
                break;
            case '2':
                std::cout<<"-------------------------"<<std::endl;
                t1 = std::chrono::high_resolution_clock::now();
                simulatedAnnealing(initial_temp, final_temp, max_iters);
                t2 = std::chrono::high_resolution_clock::now();
                ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
                std::cout<<ms_int.count()<<" w milisekundach"<<std::endl;
                std::cout<<"Rozwiązano algorytmem symulowanego wyżarzania, Cmax = "<<Cmax<<std::endl;
                std::cout<<"-------------------------"<<std::endl;
                break;
            case 'p':
                print();
                break;
            case 'q':
                std::cout<<"Wybrano wyjście z programu"<<std::endl;
                return;
            default:
                std::cout<<"Nieprawidłowa opcja, spróbuj ponownie"<<std::endl;
                break;
        }
    }
}

void Solution::annealingTest(std::string filename){
    srand(time(0));
    readFromFile(filename);

    //pierwsze: dla różnych max_iters
    int max_iters = problem.size()*problem.size()*m*10;
    float max_temp = problem.size()*problem.size()*10;
    float min_temp = problem.size()/m;
    auto t1 = std::chrono::high_resolution_clock::now();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout<<"Different max iters"<<std::endl;
    std::cout<<"MAX_ITERS = n*n*m"<<std::endl;
    for(int i = 0; i<10; i++){
        t1 = std::chrono::high_resolution_clock::now();
        simulatedAnnealing(max_temp, min_temp, max_iters);
        t2 = std::chrono::high_resolution_clock::now();
        ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        std::cout<<ms_int.count()<<" w milisekundach"<<std::endl;
        std::cout<<"Rozwiązano algorytmem symulowanego wyżarzania, Cmax = "<<Cmax<<std::endl;
    }
    max_iters *= 2;
    std::cout<<"MAX_ITERS = n*n*m*2"<<std::endl;
    for(int i = 0; i<10; i++){
        t1 = std::chrono::high_resolution_clock::now();
        simulatedAnnealing(max_temp, min_temp, max_iters);
        t2 = std::chrono::high_resolution_clock::now();
        ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        std::cout<<ms_int.count()<<" w milisekundach"<<std::endl;
        std::cout<<"Rozwiązano algorytmem symulowanego wyżarzania, Cmax = "<<Cmax<<std::endl;
    }

    max_iters *= 0.25;
    std::cout<<"MAX_ITERS = n*n*m/2"<<std::endl;
    for(int i = 0; i<10; i++){
        t1 = std::chrono::high_resolution_clock::now();
        simulatedAnnealing(max_temp, min_temp, max_iters);
        t2 = std::chrono::high_resolution_clock::now();
        ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        std::cout<<ms_int.count()<<" w milisekundach"<<std::endl;
        std::cout<<"Rozwiązano algorytmem symulowanego wyżarzania, Cmax = "<<Cmax<<std::endl;
    }
    max_iters = problem.size()*problem.size()*m*10;
    std::cout<<"Different max temps"<<std::endl;
    max_temp *= 2;
    std::cout<<"MAX_TEMP = n*n*2"<<std::endl;
    for(int i = 0; i<10; i++){
        t1 = std::chrono::high_resolution_clock::now();
        simulatedAnnealing(max_temp, min_temp, max_iters);
        t2 = std::chrono::high_resolution_clock::now();
        ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        std::cout<<ms_int.count()<<" w milisekundach"<<std::endl;
        std::cout<<"Rozwiązano algorytmem symulowanego wyżarzania, Cmax = "<<Cmax<<std::endl;
    }
    max_temp *= 2;
    std::cout<<"MAX_TEMP = n*n*4"<<std::endl;
    for(int i = 0; i<10; i++){
        t1 = std::chrono::high_resolution_clock::now();
        simulatedAnnealing(max_temp, min_temp, max_iters);
        t2 = std::chrono::high_resolution_clock::now();
        ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        std::cout<<ms_int.count()<<" w milisekundach"<<std::endl;
        std::cout<<"Rozwiązano algorytmem symulowanego wyżarzania, Cmax = "<<Cmax<<std::endl;
    }
    max_temp = problem.size()*problem.size()*10;
    std::cout<<"Different final temps"<<std::endl;
    min_temp *= 2;
    std::cout<<"MIN_TEMP = 1/n*2"<<std::endl;
    for(int i = 0; i<10; i++){
        t1 = std::chrono::high_resolution_clock::now();
        simulatedAnnealing(max_temp, min_temp, max_iters);
        t2 = std::chrono::high_resolution_clock::now();
        ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        std::cout<<ms_int.count()<<" w milisekundach"<<std::endl;
        std::cout<<"Rozwiązano algorytmem symulowanego wyżarzania, Cmax = "<<Cmax<<std::endl;
    }
    min_temp *= 2;
    std::cout<<"MIN_TEMP = 1/n*4"<<std::endl;
    for(int i = 0; i<10; i++){
        t1 = std::chrono::high_resolution_clock::now();
        simulatedAnnealing(max_temp, min_temp, max_iters);
        t2 = std::chrono::high_resolution_clock::now();
        ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        std::cout<<ms_int.count()<<" w milisekundach"<<std::endl;
        std::cout<<"Rozwiązano algorytmem symulowanego wyżarzania, Cmax = "<<Cmax<<std::endl;
    }
    std::cout<<"done"<<std::endl;
}

int Solution::noWaitCmax()
{
    int C;

    for (int i = 0; i < result.size() - 1; i++) {
        C += calculateDelay(result[i], result[i + 1]); //obliczmy delay dla każdej pary zadań z wyniku i dodajemy do C
    }

    //doajemy do cmaxa czasy wykonywania się ostatniego zadania
    int lastJob = result.back();
    for (int k = 0; k < m; k++) {
        C += problem[lastJob].getProductPj(k);
    }

    return C;
}

int Solution::calculateDelay(int job1, int job2){
    //zmienna m -> ile maszyn
    int maxDelay = 0; //największy znaleziony potrzebny delay pomiędzy Job1 i Job2
    int sumJob1 = 0;
    int sumJob2 = 0;

    //bierzemy czasy dla zadan i obliczmy dla każdej maszynuy jaki będzie delay pomiędzy maszyną k dla Job1 a k-1 Job2
    //zadania wykonuja sie bezprzerwy (po prostu sumy czasow), obliczamy delay i patrzymy gdzie jest najwiekszy

    for (int k = 0; k < m; ++k) {
        sumJob1 += problem[job1].getProductPj(k); // suma czasow wykonywania do maszyny k dla job1
        
        if (k > 0) {
            sumJob2 += problem[job2].getProductPj(k-1); // suma czasow wykonywania do maszyny k-1 dla job2
        }
        
        int currentDelay = sumJob1 - sumJob2; //obecny delay to roznica w wykonywaniu czasu dla job1 a job2 bez przerw

        maxDelay = std::max(currentDelay, maxDelay);
    }
    return maxDelay;
}

void Solution::noWaitCmaxTest(){
    //kolejność i dane z fill test data -> result = 0,1,2,3
    int testCmax = noWaitCmax();
    std::cout<<"Cmax: "<<testCmax<<std::endl;
}