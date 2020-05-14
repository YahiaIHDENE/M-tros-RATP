#include "Generic_mapper.hpp"

using namespace std;
using namespace travel;

class Lecture : public Generic_mapper  
{
public:
    void read_stations(const std::string &_filename) override;
    //uint64_t string_to_id(string arret);

    void read_connections(const std::string &_filename) override;

    std::vector<std::pair<uint64_t, uint64_t>> compute_travel(uint64_t _start, uint64_t _end) override;

    std::vector<std::pair<uint64_t, uint64_t>> compute_and_display_travel(uint64_t _start, uint64_t _end) override;
};