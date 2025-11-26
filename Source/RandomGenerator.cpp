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

int RandomGenerator::selectProduct(std::vector<Product> catalog)
{
    if (catalog.size() <= 0)
        return 0;

    std::vector<float> productPopularities;
    for(const auto& product : catalog)
    {
        productPopularities.push_back(product.getPopularityScore());
    }

    std::discrete_distribution<int> productSelect(productPopularities.begin(), productPopularities.end());
    return productSelect(gen);
}
