#include<iostream>
#include "utils.cpp"

/*
Algoritmo genético para o problema do caixeiro viajante
-Seleção por roleta
-Cruzamento oX
-Mutação swap simples entre duas posições aleatórias
-Manutenção da população por elitismo, retirando o pior indivíduo
e substituindo pelo melhor filho gerado
-Critério de parada: número de gerações
*/


int main(int argc, char **argv){
    //TODO:fazer com que os parametros principais sejam passados por comandline
    //taxa de mutacao
    //numero de geracoes
    //numero de individuos por geracao
    std::string inputFile = argv[1];
    int numeroDeGeracoes = std::stoi(argv[2]);
    int numeroDeIndividuos = std::stoi(argv[3]);
    double taxaDeMutacao = std::stod(argv[4]);
    std::string path = "./Input/" + inputFile;
    fileReader::Reader entrada(path);
    entrada.readFile();
    Ga::Genetic genetico(taxaDeMutacao, numeroDeGeracoes, entrada.get_cityPoints() , entrada.get_inputSize(), numeroDeIndividuos, "outputFile.txt");
    genetico.run();

    
    return 0 ;
}