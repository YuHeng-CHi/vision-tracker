#pragma once

#include <memory>
#include <vector>

#include "datatype/data_types.hpp"
#include "targettracker/targetcheck/TargetBase.hpp"

class ResultBuilder
{
public:
    // accepted：本帧是否被接受；targets：管理器持有的全部目标（含 LOST）
    static rm::Result build(int frame_id,const std::vector<std::unique_ptr<TargetBase>>& targets,bool accepted);
};
