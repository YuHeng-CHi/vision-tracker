#pragma once
#include "datatype/data_types.hpp"

class ObservationValidator
{
public:
//check if the observation is valid, return true if valid, false otherwise
    bool validate(const rm::InputFrame &frame) const;
};