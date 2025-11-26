#include "RandomGenerator.h"

RandomGenerator::RandomGenerator()
    : gen(rd()),
      latDist(8.56837, 8.61000),
      lonDist(-71.16609, -71.12145),
      numProductsDist(1.5),
      quantityDist(0.7)
{
}

double RandomGenerator::randomLatitude()
{
    return latDist(gen);
}

double RandomGenerator::randomLongitude()
{
    return lonDist(gen);
}

int RandomGenerator::randomNumProducts()
{
    return numProductsDist(gen);
}

int RandomGenerator::randomQuantity()
{
    return quantityDist(gen);
}

int RandomGenerator::selectProduct(int catalogSize)
{
    if (catalogSize <= 0)
        return 0;
    std::uniform_int_distribution<int> productSelect(1, catalogSize);
    return productSelect(gen);
}
