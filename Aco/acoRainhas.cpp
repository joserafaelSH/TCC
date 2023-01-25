#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>
using namespace std;

const int N = 8; // número de rainhas
int chessboard[N][N]; // tabuleiro de xadrez
double pheromone[N][N]; // matriz de feromônio
double alpha = 1.0; // parâmetro de importância do feromônio
double BETA = 1.0; // parâmetro de importância da qualidade da solução
double evaporation = 0.1; // taxa de evaporação do feromônio
int numAnts = 10; // número de formigas
int maxIterations = 5000; // número máximo de iterações

// função para avaliar a qualidade de uma solução
int evaluate(int queens[]) {
    int conflicts = 0;
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (queens[i] == queens[j]) {
                conflicts++; // rainhas na mesma linha
            } else if (abs(queens[i] - queens[j]) == abs(i - j)) {
                conflicts++; // rainhas na mesma diagonal
            }
        }
    }
    return conflicts;
}

// função para selecionar a próxima posição da rainha com base na probabilidade de feromônio
int selectNextPosition(int queens[], int currentQueen) {
    double probabilities[N];
    double total = 0.0;
    for (int i = 0; i < N; i++) {
        if (i != queens[currentQueen]) {
            probabilities[i] = pow(pheromone[currentQueen][i], alpha) * pow(1.0 / evaluate(queens), BETA);
            total += probabilities[i];
        } else {
            probabilities[i] = 0.0;
        }
    }
    for (int i = 0; i < N; i++) {
        probabilities[i] /= total;
    }
    double randomValue = (double) rand() / RAND_MAX;
    double cumProbability = 0.0;
    for (int i = 0; i < N; i++) {
        cumProbability += probabilities[i];
        if (randomValue <= cumProbability) {
            return i;
        }
    }
    return 0;
}

void updatePheromone(int queens[], int conflicts) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            pheromone[i][j] *= (1.0 - evaporation);
        }
    }

    double deltaPheromone = 1.0 / conflicts;
    for (int i = 0; i < N; i++) {
        pheromone[i][queens[i]] += deltaPheromone;
    }
}

void solveNQueens() {
    
    int bestQueens[N];
    int bestConflicts = N * (N - 1) / 2; // número máximo de conflitos possíveis
    int currentQueens[N];
    int currentConflicts;
    for (int i = 0; i < maxIterations; i++) {
        for (int j = 0; j < numAnts; j++) {
            for (int k = 0; k < N; k++) {
                currentQueens[k] = selectNextPosition(currentQueens, k);
            }
            currentConflicts = evaluate(currentQueens);
            if (currentConflicts < bestConflicts) {
                bestConflicts = currentConflicts;
                for (int k = 0; k < N; k++) {
                    bestQueens[k] = currentQueens[k];
                    
                }
                if (bestConflicts == 0) {
                    break;
                }
            }
            updatePheromone(currentQueens, currentConflicts);
        }
        if (bestConflicts == 0) {
            break;
        }
        cout << "Solução Final:" << endl;
        for(int i = 0; i < N; i++) {
            std::cout<<bestQueens[i] <<" - ";
        }
    }
}

int main() {
    srand(time(NULL));
    solveNQueens();
    return 0;
}