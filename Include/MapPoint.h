#include <string>

// Clase para guardar la información de un nodo
class MapPoint
{
private:
    long long id;
    double latitude;
    double longitude;
    std::string name;

public:
    MapPoint();
    MapPoint(long long nodeId, double lat, double lon, const std::string& nodeName = "");

    long long getId() const;
    double getLatitude() const;
    double getLongitude() const;
    std::string getName() const;

    void setId(long long newId);
    void setLatitude(double newLat);
    void setLongitude(double newLon);
    void setName(const std::string& newName);
};

