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
    std::vector<std::string> args(argv, argv + argc);
    int i = 0;
    int numeroDeGeracoes = std::atoi(argv[8]);
    //std::cout << numeroDeGeracoes << std::endl;
    int numeroDeIndividuos = std::atoi(argv[10]);
    //std::cout << numeroDeIndividuos << std::endl;
    double taxaDeMutacao = std::atof(argv[12]);
    //std::cout << taxaDeMutacao << std::endl;
    fileReader::Reader entrada(args[4]);
    entrada.readFile();
    //std::cout << entrada.get_inputSize() << std::endl;
    Ga::Genetic genetico(taxaDeMutacao, numeroDeGeracoes, entrada.get_cityPoints() , entrada.get_inputSize(), numeroDeIndividuos, "outputFile.txt");
    genetico.run();

    return 0 ;
}
