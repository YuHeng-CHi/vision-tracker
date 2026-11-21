#include "targettracker/targetfactory/ResultBuilder.hpp"

#include "targettracker/targetcheck/TargetBase.hpp"

rm::Result ResultBuilder::build(int frame_id,const std::vector<std::unique_ptr<TargetBase>>& targets,bool accepted){
    rm::Result result{};
    result.accepted = accepted;
    result.frame_id = frame_id;

    //create malloc vector to avoid multiple reallocations
    result.targets.reserve(targets.size());
    for (const auto &target : targets){
        if (!target){
            continue;
        }

        const rm::TargetSnapshot snapshot = target->snapshot();
        result.targets.push_back(snapshot);

        //summary statistics: TRACK / LOST
        if (snapshot.status == rm::TargetStatus::TRACK){
            ++result.tracking_target_count;
        }
        else{
            ++result.lost_target_count;
        }

        //summary statistics: ARMOR / SPINNING, LOST targets also counted
        if (snapshot.type == rm::TargetType::ARMOR){
            ++result.armor_target_count;
        }
        else{
            ++result.spinning_target_count;
        }
    }

    return result;
}
