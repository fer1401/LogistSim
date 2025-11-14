#include "Nodeinfo.h"

NodeInfo::NodeInfo(long long nodeId, double lat, double lon, const std::string& nodeName)
    : id(nodeId), latitude(lat), longitude(lon), name(nodeName) {}

long long NodeInfo::getId() const {
    return id;
}

double NodeInfo::getLatitude() const {
    return latitude;
}

double NodeInfo::getLongitude() const {
    return longitude;
}

std::string NodeInfo::getName() const {
    return name;
}

void NodeInfo::setId(long long newId) {
    id = newId;
}

void NodeInfo::setLatitude(double newLat) {
    latitude = newLat;
}

void NodeInfo::setLongitude(double newLon) {
    longitude = newLon;
}

void NodeInfo::setName(const std::string& newName) {
    name = newName;
}
