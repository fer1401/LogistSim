#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <random>

class RandomGenerator
{
public:
    RandomGenerator();

    double randomLatitude();
    double randomLongitude();

    int randomNumProducts();
    int randomQuantity();
    int selectProduct(int catalogSize);

private:
    std::random_device rd;  // seed source for the RNG
    std::mt19937 gen; // mersenne_twister_engine seeded with rd()

    std::uniform_real_distribution<double> latDist;
    std::uniform_real_distribution<double> lonDist;
    std::poisson_distribution<int> numProductsDist;
    std::geometric_distribution<int> quantityDist;
};

#endif // RANDOMGENERATOR_H
