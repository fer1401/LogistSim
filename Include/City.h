#ifndef CITY_H
#define CITY_H

#include <graph.hpp>
#include "CountHandler.h"

class City
{
private:

    Designar::Digraph<MapPoint, Street> graph;

public:

    City();
};

#endif // CITY_H
