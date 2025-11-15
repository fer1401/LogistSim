#include <string>
#include <vector>
#include <utility> // Para std::pair

//para almacenar una coordenada (Latitud, Longitud)
using CoordinatePair = std::pair<double, double>;

class Street
{
private:
    long long id;
    std::string streetName;
    double length;

    // Lista de coordenadas intermedias que dibujan la curva de la calle
    std::vector<CoordinatePair> geometry;

public:
    Street();
    Street(long long arcId, std::string name, double len = 0.0);

    long long getId() const;
    std::string getStreetName() const;
    double getLength() const;
    const std::vector<CoordinatePair>& getGeometry() const;

    void setLength(double len);
    void setStreetName(const std::string& name);
    void addIntermediatePoint(double lat, double lon); // Para añadir puntos a la geometría
};
