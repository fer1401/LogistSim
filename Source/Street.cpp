#include "Street.h"

Street::Street()
{}

Street::Street(long long arcId, std::string name, double len)
    : id(arcId), streetName(name), length(len) {}

long long Street::getId() const {
    return id;
}


std::string Street::getStreetName() const {
    return streetName;
}

double Street::getLength() const {
    return length;
}

const std::vector<CoordinatePair>& Street::getGeometry() const {
    return geometry;
}

void Street::setLength(double len) {
    length = len;
}

void Street::setStreetName(const std::string& name) {
    streetName = name;
}

void Street::addIntermediatePoint(double lat, double lon) {
    // Agrega un nuevo par de coordenadas (lat, lon) a la geometría
    geometry.push_back({lat, lon});
}
