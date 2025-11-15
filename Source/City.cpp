#include "City.h"
#include <iostream>

City::City()
{
    try
    {
        const osmium::io::File input_file{"qrc:/map.osm"};
        osmium::io::Reader reader{input_file};

        // Create an instance of our own CountHandler and push the data from the
        // input file through it.
        CountHandler handler;
        osmium::apply(reader, handler);

        // You do not have to close the Reader explicitly, but because the
        // destructor can't throw, you will not see any errors otherwise.
        reader.close();

        // Print graph
        for(const auto& arc : handler.g.arcs())
        {
            std::cout << arc->get_info().getId() << ": " << arc->get_src_node()->get_info().getId() << " -> " << arc->get_tgt_node()->get_info().getId() << "\n";
        }

        // Because of the huge amount of OSM data, some Osmium-based programs
        // (though not this one) can use huge amounts of data. So checking actual
        // memore usage is often useful and can be done easily with this class.
        // (Currently only works on Linux, not macOS and Windows.)
        const osmium::MemoryUsage memory;

        std::cout << "\nMemory used: " << memory.peak() << " MBytes\n";
    }
    catch (const std::exception &e)
    {
        // All exceptions used by the Osmium library derive from std::exception.
        std::cerr << e.what() << '\n';
    }
}
