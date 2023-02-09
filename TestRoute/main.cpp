#include <math.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

double pitagoras(float x1, float y1, float x2, float y2)
{
    float x = x2 - x1;
    float y = y2 - y1;
    float raiz = (x * x) + (y * y);
    return sqrt(raiz);
}

float pathValue(std::vector<int> path, std::vector<std::vector<float>> cityPoints)
{

    float value = 0.0;

    for (int i = 0; i < path.size() - 2; i++)
    {
        value += pitagoras(
            cityPoints[path[i] - 1][0],
            cityPoints[path[i] - 1][1],
            cityPoints[path[i + 1] - 1][0],
            cityPoints[path[i + 1] - 1][1]);
    }

    return value;
}

std::vector<std::vector<float>> readFileCity(std::string filePath)
{
    std::ifstream arquivo;
    arquivo.open(filePath);
    std::vector<std::vector<float>> cityPoints;
    int counter = 0;
    std::string line;
    std::string buffer;
    float n, x, y;

    if (arquivo.is_open())
    {
        while (getline(arquivo, line))
        {
            if (counter > 5 and line != "EOF")
            {
                std::stringstream read(line);
                read >> n >> x >> y;
                std::vector<float> points = {0, 0};
                points[0] = x;
                points[1] = y;
                cityPoints.push_back(points);
            }
            counter += 1;
        }
    }
    arquivo.close();
    return cityPoints;
}

std::vector<int> readFilePath(std::string filePath)
{
    std::ifstream arquivo;
    arquivo.open(filePath);
    std::vector<int> route;
    std::string line;
    std::string buffer;
    float n, x, y;

    if (arquivo.is_open())
    {
        while (getline(arquivo, line))
        {

            std::stringstream read(line);
            read >> x;
            route.push_back(x);
        }
    }
    arquivo.close();
    return route;
}

bool validatePath(std::vector<int> path)
{
    std::vector<bool> pathCopy(path.size(), false);
    for (int i = 0; i < path.size(); i++)
    {
        if (pathCopy[path[i]] == false)
        {
            pathCopy[path[i]] = true;
        }
        else
        {
            return false;
        }
    }
    return true;
}

int main()
{
    std::string cityPointsFile = "att48.tsp";
    std::string pathFile = "att48Path.txt";
    std::vector<std::vector<float>> cityPoints = readFileCity("./Input/" + cityPointsFile);
    std::vector<int> route = readFilePath("./Input/" + pathFile);
    if (validatePath(route))
    {
        std::cout << "Path is valid" << std::endl;
        std::cout << pathValue(route, cityPoints) << std::endl;
    }
    else
    {
        std::cout << "Path is invalid" << std::endl;
    }

    std::ofstream arquivo;
    arquivo.open("SAIDA.txt");
    for (int i = 0; i < route.size(); i++)
    {
        arquivo << cityPoints[route[i] - 1][0] << " " << cityPoints[route[i] - 1][1] << std::endl;
    }

    return 0;
}