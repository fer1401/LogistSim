#include "MapPoint.h"

MapPoint::MapPoint(long long nodeId, double lat, double lon, const std::string& nodeName)
    : id(nodeId), latitude(lat), longitude(lon), name(nodeName) {}

long long MapPoint::getId() const {
    return id;
}

double MapPoint::getLatitude() const {
    return latitude;
}

double MapPoint::getLongitude() const {
    return longitude;
}

std::string MapPoint::getName() const {
    return name;
}

void MapPoint::setId(long long newId) {
    id = newId;
}

void MapPoint::setLatitude(double newLat) {
    latitude = newLat;
}

void MapPoint::setLongitude(double newLon) {
    longitude = newLon;
}

void MapPoint::setName(const std::string& newName) {
    name = newName;
}
