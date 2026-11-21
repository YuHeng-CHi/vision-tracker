#include "targettracker/targetfactory/ObservationValidator.hpp"
#include <vector>
#include <algorithm>

bool ObservationValidator::validate(const rm::InputFrame &frame) const
{   
    std::vector<int> ids;    
    //check for zero or negative IDs and duplicate IDs
    for (const auto& obs : frame.observations){
        if (obs.id < 0 || obs.type != rm::TargetType::ARMOR && obs.type != rm::TargetType::SPINNING){
            return false; // Invalid observation: negative ID or invalid type
        }
        ids.push_back(obs.id);
    }
    //check for duplicate IDs
    std::sort(ids.begin(), ids.end());
    for (size_t i = 1; i < ids.size(); ++i){
        if (ids[i] == ids[i - 1]){
            return false; // Invalid observation: duplicate ID
        }
    }
    return true; // All observations are valid
}