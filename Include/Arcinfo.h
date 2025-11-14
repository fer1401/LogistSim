#include <string>
#include <vector>
#include <utility> // Para std::pair
#include "Nodeinfo.h" // Necesitas referenciar los nodos de inicio/fin

//para almacenar una coordenada (Latitud, Longitud)
using CoordinatePair = std::pair<double, double>;

class ArcInfo {
private:
    long long id;
    NodeInfo* startNode;
    NodeInfo* endNode;
    std::string streetName;
    double length;

    // Lista de coordenadas intermedias que dibujan la curva de la calle
    std::vector<CoordinatePair> geometry;

public:
    ArcInfo(long long arcId, NodeInfo* start, NodeInfo* end,
            const std::string& name = "", double len = 0.0);

    long long getId() const;
    NodeInfo* getStartNode() const;
    NodeInfo* getEndNode() const;
    std::string getStreetName() const;
    double getLength() const;
    const std::vector<CoordinatePair>& getGeometry() const;

    void setLength(double len);
    void setStreetName(const std::string& name);
    void addIntermediatePoint(double lat, double lon); // Para añadir puntos a la geometría
};
