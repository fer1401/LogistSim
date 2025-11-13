#include "Arcinfo.h"

ArcInfo::ArcInfo(long long arcId, NodeInfo* start, NodeInfo* end,
                 const std::string& name, double len)
    : id(arcId), startNode(start), endNode(end), streetName(name), length(len) {}

long long ArcInfo::getId() const {
    return id;
}

NodeInfo* ArcInfo::getStartNode() const {
    return startNode;
}

NodeInfo* ArcInfo::getEndNode() const {
    return endNode;
}

std::string ArcInfo::getStreetName() const {
    return streetName;
}

double ArcInfo::getLength() const {
    return length;
}

const std::vector<CoordinatePair>& ArcInfo::getGeometry() const {
    return geometry;
}

void ArcInfo::setLength(double len) {
    length = len;
}

void ArcInfo::setStreetName(const std::string& name) {
    streetName = name;
}

void ArcInfo::addIntermediatePoint(double lat, double lon) {
    // Agrega un nuevo par de coordenadas (lat, lon) a la geometría
    geometry.push_back({lat, lon});
}
