#ifndef CPRV_DOOR_H
#define CPRV_DOOR_H

#include <string>

#include <GJGO/2D/transform2D.hpp>

struct Door
{
    GJGO::Position2D position;
    GJGO::Size2D size;
    std::string target;

    Door(const GJGO::Position2D &a_position, const GJGO::Size2D &a_size, const std::string &a_target) :
        position(a_position), size(a_size), target(a_target) {}
};

#endif
