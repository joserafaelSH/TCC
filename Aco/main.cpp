#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <math.h>
#include <sstream>
#include <fstream>


namespace fileReader
{
    class Reader{
        private:
            int inputSize = 0;
            std::string fileName;
            std::vector<std::vector<float>> cityPoints;

        public:
            Reader(){
            }
            
            Reader(std::string name){
                this->fileName = name;
            }

            void readFile(){
                std::ifstream arquivo;
                arquivo.open(this->fileName);
                
                int counter = 0;
                std::string line;
                std::string buffer;
                float n ,x, y ;
           

                if(arquivo.is_open()){
                    while(getline(arquivo, line)){
                        if(counter == 3){

                            std::stringstream read(line);
                            read >> buffer >> buffer >> this->inputSize;
                        }
                        if(counter > 5 and line != "EOF"){
                            std::stringstream read(line);
                            read >> n >> x >> y;
                            std::vector<float> points = {0,0};
                            points[0] = x ;
                            points[1] = y ;
                            this->cityPoints.push_back(points) ;     
            
                        }
                        counter+=1;
                    }
                }
                arquivo.close();
            }

            void showCityPoints(){
                for(auto city : this->cityPoints)
                    std::cout<<city[0] << " "<<city[1]<<std::endl;
                  
            }

            int get_inputSize(){
                return this->inputSize;
            }
    
            std::vector<std::vector<float>> get_cityPoints(){
                return this->cityPoints;
            }

    };
    
}; // namespace LeituraArquivo

typedef struct Rotas{
    float p1;
    float p2;
    float distancia;
    float Tp1p2;
}Rota;

typedef struct Ants{
    std::vector<int> tour;
    double tourLength;
}Ant;

double dist(float ax,float ay, float bx, float by){
    float dx = ax - bx;
    float dy = ay - by;
    return sqrt(dx * dx + dy * dy);
}

float calcularDistancia(std::vector<int> tour, std::vector<std::vector<float>> pontos){
    float distancia = 0;
    for(int i = 0; i < tour.size()-1; i++){
        distancia += dist(pontos[tour[i]][0], pontos[tour[i]][1], pontos[tour[i+1]][0], pontos[tour[i+1]][1]);
    }
    return distancia;
}

void inicializarRotas(std::vector<std::vector<float>> pontos, std::vector<Rota> &rotas){
    
    for(int i = 0; i < pontos.size(); i++){
        for(int j = 0; j < pontos.size(); j++){
            if(i != j){
                Rota r;
                r.p1 = i;
                r.p2 = j;
                r.distancia = dist(pontos[i][0], pontos[i][1], pontos[j][0], pontos[j][1]);
                r.Tp1p2 = 1/r.distancia;
                rotas.push_back(r);
            }
        }
    }
}

void calcularTauPorFeromonio(std::vector<float> &tauPorFeromonio, std::vector<float> feromonios, std::vector<Rota> rotas ){
    for(int i = 0; i < rotas.size(); i++){
        tauPorFeromonio[i] = rotas[i].Tp1p2 * feromonios[i];
    }
}

void calcularProb(std::vector<float> &prob, std::vector<float> tauPorFeromonio, std::vector<Rota> rotas){
    for(int i = 0; i < rotas.size(); i++){
        float soma = 0;
        for(int j = 0; j < rotas.size(); j++){
            if(rotas[i].p1 == rotas[j].p1){
                soma += tauPorFeromonio[j];
            }
        }
        prob[i] = tauPorFeromonio[i]/soma;
    }
}

void debugPrint(std::vector<Rota> rotas, std::vector<float> feromonios, std::vector<float> tauPorFeromonio, std::vector<float> prob ){
    float soma = 0;
    for(int i = 0; i < rotas.size(); i++){
        soma += prob[i];
        std::cout<<"Rota: "<<rotas[i].p1<<" "<<rotas[i].p2<<" Distancia: "<<rotas[i].distancia<<" Feromonio: "<<feromonios[i]<<" TauRota: "<<rotas[i].Tp1p2<<" TauFeromonio: "<<tauPorFeromonio[i]<<" ProbDec: "<<prob[i]<<" Prob%: "<<(prob[i]*100.0)<<std::endl;
    }
    std::cout<<"P: "<<soma<<std::endl;
}

void inicializarRotasFormigas(std::vector<Ant> &ants, std::vector<Rota> rotas, std::vector<std::vector<float>> pontos, int NUMERO_FORMIGAS, std::vector<float> prob){
    for(int i = 0; i < NUMERO_FORMIGAS; i++){
        std::vector<int> visitados;
        int pontoAtual = 0;
        visitados.push_back(pontoAtual);

        while(visitados.size() < pontos.size()){
            float r = (float)rand()/(float)RAND_MAX;
            float soma = 0;
            for(int j = 0; j < rotas.size(); j++){
                if(rotas[j].p1 == pontoAtual){
                    soma += prob[j];
                    if(soma >= r){
                        pontoAtual = rotas[j].p2;
                        visitados.push_back(pontoAtual);
                        break;
                    }
                }
            }
        }
        ants[i].tour = visitados;
        ants[i].tourLength = calcularDistancia(visitados, pontos);
    }
}

void tabelaDeAtualizacao(std::vector<std::vector<float>> &tabelaAtt, std::vector<float> tauPorFeromonio ,std::vector<Ant> ants, std::vector<Rota> rotas,  std::vector<float> &feromonios, float COEFICIENTE_EVAPORACAO, int Q){
    
    int idxValorPosEvaporacao = ants.size();
    int idxValorTotal = idxValorPosEvaporacao+1;
    for(int i = 0; i < rotas.size(); i++){
        tabelaAtt[i][idxValorPosEvaporacao] = (1-COEFICIENTE_EVAPORACAO) * tauPorFeromonio[i] ;
        for(int j = 0; j < ants.size(); j++){
            float feromonioDepositado = tabelaAtt[i][idxValorPosEvaporacao];
            for(int k = 0; k < ants[j].tour.size()-1; k++){
                if((rotas[i].p1 == ants[j].tour[k] || rotas[i].p2 == ants[j].tour[k]) && (rotas[i].p2 == ants[j].tour[k+1] || rotas[i].p1 == ants[j].tour[k+1])){
        
                    feromonioDepositado = Q/ants[j].tourLength;
                    tabelaAtt[i][idxValorPosEvaporacao] = feromonioDepositado;
                    tabelaAtt[i][idxValorTotal] += feromonioDepositado;

                }
            }
            tabelaAtt[i][j]=feromonioDepositado;
            feromonios[i] = tabelaAtt[i][idxValorTotal];
        }
    }
    
}

int main(int argc, char **argv){
    srand(time(0)); 
    //./main att48.txt 0.1 5 0.01 100 10
    std::string inputFile = argv[1];
    std::string path = "./Input/" + inputFile;
    fileReader::Reader entrada(path);
    entrada.readFile();
    double FEROMONIO_INICIAL = std::stod(argv[2]);
    int NUMERO_FORMIGAS = std::atoi(argv[3]);
    float COEFICIENTE_EVAPORACAO = std::stof(argv[4]);
    int NUM_GERACOES = std::atoi(argv[5]);
    int Q = std::atoi(argv[6]);
        
    std::vector<std::vector<float>> pontos = entrada.get_cityPoints();
    std::vector<Rota> rotas;
    inicializarRotas(pontos, rotas);
    std::vector<float> feromonios(rotas.size(), FEROMONIO_INICIAL);
    std::vector<float> tauPorFeromonio(rotas.size(), 0);
    std::vector<float> prob(rotas.size(), 0);

    calcularTauPorFeromonio(tauPorFeromonio, feromonios, rotas);
    calcularProb(prob, tauPorFeromonio, rotas);
    
    int iteracoes = 0;

    std::vector<Ant> ants;
    for(int i = 0; i < NUMERO_FORMIGAS; i++){
        Ant a;
        a.tourLength = 0;
        ants.push_back(a);
    }
    inicializarRotasFormigas(ants, rotas, pontos, NUMERO_FORMIGAS, prob );
    for(int i = 0; i < NUMERO_FORMIGAS; i++){
        std::cout<<ants[i].tour.size()<<std::endl;
        std::cout<<ants[i].tourLength<<std::endl;
        for(int j = 0; j < ants[i].tour.size(); j++){
            std::cout<<ants[i].tour[j]<<" ";
        }
        std::cout<<std::endl;
    }

    for(int i = 0; i <NUM_GERACOES; i++){
        std::vector<std::vector<float>> tabelaAtt(rotas.size(), std::vector<float>(NUMERO_FORMIGAS+2, 0));
        tabelaDeAtualizacao(tabelaAtt, tauPorFeromonio, ants, rotas, feromonios, COEFICIENTE_EVAPORACAO, Q);
        iteracoes++;
        for(int i = 0; i < NUMERO_FORMIGAS; i++){
            ants[i].tour.clear();
            ants[i].tourLength = 0;
        }
        inicializarRotasFormigas(ants, rotas, pontos, NUMERO_FORMIGAS, prob );
    }

    


    for(int i = 0; i < NUMERO_FORMIGAS; i++){
        std::cout<<ants[i].tour.size()<<std::endl;
        std::cout<<ants[i].tourLength<<std::endl;
        for(int j = 0; j < ants[i].tour.size(); j++){
            std::cout<<ants[i].tour[j]<<" ";
        }
        std::cout<<std::endl;
    }
    
    // for(int i = 0; i < rotas.size(); i++){
    //     for(int j = 0; j < tabelaAtt[i].size(); j++){
    //         std::cout<<tabelaAtt[i][j]<<" ";
    //     }
    //     std::cout<<std::endl;
    // }

    return 0;
}