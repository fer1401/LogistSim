#ifndef COUNTHANDLER_H
#define COUNTHANDLER_H

#include <cstdint> // for std::uint64_t
#include <osmium/io/any_input.hpp> // Allow any format of input files (XML, PBF, ...)
#include <osmium/handler.hpp> // We want to use the handler interface
#include <osmium/util/memory.hpp> // Utility class gives us access to memory usage information
#include <osmium/visitor.hpp> // For osmium::apply()
#include <osmium/geom/haversine.hpp>
#include <osmium/geom/coordinates.hpp>

#include <graph.hpp>
#include <MapPoint.h>
#include <Street.h>

// Handler derive from the osmium::handler::Handler base class. Usually you
// overwrite functions node(), way(), and relation(). Other functions are
// available, too. Read the API documentation for details.
struct CountHandler : public osmium::handler::Handler
{
    Designar::Digraph<MapPoint, Street> g;

    std::uint64_t nodes = 0;
    std::uint64_t ways = 0;
    std::uint64_t relations = 0;

    // This callback is called by osmium::apply for each node in the data.
    void node(const osmium::Node &node) noexcept
    {
        auto point = MapPoint(node.id(), node.location().lat(), node.location().lon());
        g.insert_node(point);
    }

    // This callback is called by osmium::apply for each way in the data.
    void way(const osmium::Way &way) noexcept
    {
        // Insert only if it is a road
        if (!way.tags()["highway"])
        {
            return;
        }

        const char* oneway_value = way.tags().get_value_by_key("oneway");

        auto i = way.nodes().begin();
        auto j = i + 1;
        while(i != way.nodes().end() && j != way.nodes().end())
        {
            auto src = g.filter_nodes([&i](const auto &node) { return node->get_info().getId() == i->ref(); }).get_first();
            auto tgt = g.filter_nodes([&j](const auto &node) { return node->get_info().getId() == j->ref(); }).get_first();

            auto street = Street(way.id(), "", osmium::geom::haversine::distance(osmium::geom::Coordinates(src->get_info().getLongitude(), src->get_info().getLatitude()), osmium::geom::Coordinates(tgt->get_info().getLongitude(), tgt->get_info().getLatitude())));

            if (oneway_value)
            {
                if (std::strcmp(oneway_value, "yes"))
                {
                    g.insert_arc(src, tgt, street);
                }
                else if (std::strcmp(oneway_value, "-1"))
                {
                    g.insert_arc(tgt, src, street);
                }
            }

            g.insert_arc(src, tgt, street);
            g.insert_arc(tgt, src, street);

            ++i;
            ++j;
        }
    }

}; // struct CountHandler

#endif // COUNTHANDLER_H
