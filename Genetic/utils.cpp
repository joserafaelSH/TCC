#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <sstream>
#include <fstream>
#include <map>

namespace Ga
{
    class Genetic
    {
    private:
        float mutationRate;
        int nGenerations;
        int popSize;
        int inputSize;
        std::vector<int> bestSolution;
        std::vector<std::vector<int>> population;
        std::vector<float> fitness;
        std::vector<std::vector<float>> cityPoints;
        std::vector<int> result;
        std::string outputFile;
        int op;
        int t;

        double pitagoras(float x1, float y1, float x2, float y2)
        {
            float x = x2 - x1;
            float y = y2 - y1;
            float raiz = (x * x) + (y * y);
            return sqrt(raiz);
        }

        int getIndex(std::vector<int> v, int K)
        {
            auto it = std::find(v.begin(), v.end(), K);
            if (it != v.end())
            {
                int index = it - v.begin();
                return index;
            }
            else
            {
                return -1;
            }
        }

        int findLowestFit()
        {
            int idx = 0;
            float val = this->fitness[0];

            for (int i = 0; i < this->popSize; i++)
            {
                if (val < this->fitness[i])
                {
                    val = this->fitness[i];
                    idx = i;
                }
            }

            return idx;
        }

        int findBestFit()
        {
            int idx = 0;
            float val = this->fitness[0];

            for (int i = 0; i < this->popSize; i++)
            {
                if (val < this->fitness[i])
                {
                    val = this->fitness[i];
                    idx = i;
                }
            }

            return idx;
        }

        bool findElem(std::vector<int> v, int K)
        {
            for (int i = 0; i < v.size(); i++)
            {
                if (v[i] == K)
                    return true;
            }
            return false;
        }

        void createInitialPopulation()
        {

            std::random_device dev;
            std::mt19937 rng(dev());

            for (int i = 0; i < this->popSize; i++)
            {
                std::vector<int> pop(this->inputSize);
                std::iota(pop.begin(), pop.end(), 1);
                shuffle(pop.begin(), pop.end(), rng);

                this->population.push_back(pop);
            }
        }

        std::vector<std::vector<int>> &get_population()
        {
            return this->population;
        }

        std::vector<float> get_fitness()
        {
            return this->fitness;
        }

        float pathValue(std::vector<int> path)
        {

            float value = 0.0;

            for (int i = 0; i < path.size() - 2; i++)
            {
                value += this->pitagoras(
                    this->cityPoints[path[i] - 1][0],
                    this->cityPoints[path[i] - 1][1],
                    this->cityPoints[path[i + 1] - 1][0],
                    this->cityPoints[path[i + 1] - 1][1]);
            }

            return value;
        }

        void calculateFitness()
        {
            for (int i = 0; i < this->popSize; i++)
            {
                this->fitness[i] = (1.0 / this->pathValue(this->population[i]));
            }
        }

        bool validatePath(std::vector<int> path)
        {
            std::vector<bool> visited(this->inputSize, false);
            for (int i = 0; i < path.size(); i++)
            {
                if (visited[path[i] - 1] == true)
                {
                    return false;
                }
                visited[path[i] - 1] = true;
            }
            return true;
        }

        void showPath(std::vector<int> path)
        {
            for (auto &c : path)
                std::cout << c << " ";

            std::cout << "\n";
        }

        int rouletteSelection()
        {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<double> dist(0.0, 1.0);
            float total = std::accumulate(this->fitness.begin(), this->fitness.end(), 0.0);
            std::vector<float> probability;
            for (auto fit : this->fitness)
                probability.push_back(fit / total);

            float offset = 0.0;
            int pick = 0;
            int c = 0;
            float rng = dist(mt);

            while (c < this->popSize)
            {
                offset += probability[c];
                if (rng < offset)
                {
                    return c;
                }
                c += 1;
            }
            return pick;
        }

        void mutation(std::vector<int> &path)
        {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> dist(0, this->inputSize - 1);
            int pos1 = dist(mt);
            int pos2 = dist(mt);
            int aux = path[pos1];
            path[pos1] = path[pos2];
            path[pos2] = aux;
        }

        void orderCrossover(const std::vector<int> &p1, const std::vector<int> &p2)
        {
            int n = p1.size();
            std::mt19937 rng(std::random_device{}());
            std::uniform_int_distribution<int> dist(0, n - 1);
            int a = dist(rng), b = dist(rng);
            if (a > b)
                std::swap(a, b);

            std::vector<int> c1(p1.begin() + a, p1.begin() + b + 1);
            std::vector<int> c2(p2.begin() + a, p2.begin() + b + 1);

            for (int i : p2)
            {
                if (std::find(c1.begin(), c1.end(), i) == c1.end())
                {
                    c1.push_back(i);
                }
            }

            for (int i : p1)
            {
                if (std::find(c2.begin(), c2.end(), i) == c2.end())
                {
                    c2.push_back(i);
                }
            }

            if (this->pathValue(c1) < this->pathValue(c2))
            {
                this->result = c1;
            }
            else
            {
                this->result = c2;
            }
        }

        void populationMaintenance()
        {
            int idx = this->findLowestFit();
            if (this->pathValue(this->result) < this->pathValue(this->population[idx]) and this->validatePath(this->result))
            {

                this->population[idx] = this->result;
            }
        }

        void mostrarPopulacao()
        {
            for (auto &row : this->population)
            {
                int res = this->pathValue(std::vector<int>(row));
                for (auto &col : row)
                {
                    std::cout << col << " ";
                }
                std::cout << " " << res;
                std::cout << "\n";
            }
        }

    public:
        Genetic()
        {
        }

        Genetic(float mutationRate, int nGenerations, std::vector<std::vector<float>> cityPoints, int inputSize, int popSize, std::string outputFile)
        {
            this->mutationRate = mutationRate;
            this->nGenerations = nGenerations;
            this->cityPoints = cityPoints;
            this->popSize = popSize;
            this->inputSize = inputSize;
            this->outputFile = outputFile;
            this->fitness = std::vector<float>(popSize, 0.0);
        }

        void run()
        {

            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<double> dist(0.0, 1.0);
            int gen = 0;
            this->createInitialPopulation();
            this->calculateFitness();
            while (gen < this->nGenerations)
            {
                int best = this->findBestFit();
                // std::cout<<this->pathValue(this->population[best])<<std::endl;
                std::vector<int> bestPath = this->population[best];
                std::vector<std::vector<int>> newPopulation;
                newPopulation.push_back(bestPath);

                while (newPopulation.size() < this->popSize)
                {
                    int pathIdx1 = this->rouletteSelection();
                    int pathIdx2 = this->rouletteSelection();
                    while (pathIdx1 == pathIdx2)
                    {
                        pathIdx2 = this->rouletteSelection();
                    }
                    // std::cout<<pathIdx1<<" "<<pathIdx2<<std::endl;
                    this->orderCrossover(this->population[pathIdx1], this->population[pathIdx2]);
                    float mut = dist(mt);
                    if (mut <= this->mutationRate)
                        this->mutation(this->result);

                    if (this->validatePath(this->result))
                    {
                        newPopulation.push_back(this->result);
                    }
                }
                this->population = newPopulation;

                this->calculateFitness();
                gen += 1;
            }
            int best = this->findBestFit();
            std::cout << this->pathValue(this->population[best]) << std::endl;
            std::ofstream arquivo;
            arquivo.open(this->outputFile);
            for (auto &col : this->population[best])
            {
                arquivo << col << std::endl;
            }
        }
    };

};

// namespace LeituraArquivo
namespace fileReader
{
    class Reader
    {
    private:
        int inputSize = 0;
        std::string fileName;
        std::vector<std::vector<float>> cityPoints;

    public:
        Reader()
        {
        }

        Reader(std::string name)
        {
            this->fileName = name;
        }

        void readFile()
        {
            std::ifstream arquivo;
            arquivo.open(this->fileName);

            int counter = 0;
            std::string line;
            std::string buffer;
            float n, x, y;

            if (arquivo.is_open())
            {
                while (getline(arquivo, line))
                {
                    if (counter == 3)
                    {

                        std::stringstream read(line);
                        read >> buffer >> buffer >> this->inputSize;
                    }
                    if (counter > 5 and line != "EOF")
                    {
                        std::stringstream read(line);
                        read >> n >> x >> y;
                        std::vector<float> points = {0, 0};
                        points[0] = x;
                        points[1] = y;
                        this->cityPoints.push_back(points);
                    }
                    counter += 1;
                }
            }
            arquivo.close();
        }

        void showCityPoints()
        {
            for (auto city : this->cityPoints)
                std::cout << city[0] << " " << city[1] << std::endl;
        }

        int get_inputSize()
        {
            return this->inputSize;
        }

        std::vector<std::vector<float>> get_cityPoints()
        {
            return this->cityPoints;
        }
    };

}; // namespace LeituraArquivo