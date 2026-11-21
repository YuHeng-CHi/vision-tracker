/*
 * @FilePath: include/targettracker/targetmanager/TargetManager.hpp
 * @Description: 目标管理器：持有所有目标，逐帧处理 onFrame。
 */

#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include <algorithm>
#include "datatype/data_types.hpp"
#include "targettracker/targetcheck/TargetBase.hpp"
#include "targettracker/targetfactory/TargetFactory.hpp"
#include "targettracker/targetfactory/FrameValidator.hpp"
#include "targettracker/targetfactory/ObservationValidator.hpp"
#include "targettracker/targetfactory/ResultBuilder.hpp"
// 当前为占位实现（见 src/targettracker/targetmanager/TargetManager.cpp）：
// 尚未实现任何跟踪逻辑，只保证 main 可以编译运行。
// 正式实现时保持 onFrame 签名不变，在 TargetManager.cpp 中补全逻辑即可，main 不需要改动。
// 开始时目标管理器内无目标；收到此前未创建过的编号时创建目标，main 不参与。

class TargetManager
{
    std::vector<std::unique_ptr<TargetBase>> targets_;
    FrameValidator frame_validator_;
    ObservationValidator observation_validator_;
public:
    //check if the target with the given id exists, if so return a pointer to it, otherwise return nullptr
    TargetBase* findTarget(int id);
    //if the target with the given id does not exist, create a new target with the given observation and return a pointer to it, otherwise return nullptr
    TargetBase* createTarget(const rm::Observation& obs);
    //upadte time stamp of the target with the given id, if it exists
    void updateTargetTimestamp(const rm::Observation& obs, std::int64_t timestamp_ns , std::int64_t frame_dt);
    //handle unobserved targets, if the target with the given id does not exist, do nothing
    void handleUnobservedTargets(const std::vector<int> &observed_ids, std::int64_t frame_dt);
    //commit the current state of all targets to the result, and return the result
    void commitTargets(const rm::InputFrame& frame);
    //build the result for the current frame, including all targets and their current state, and return the result
    rm::Result buildResult(int frame_id, bool accepted);
    //process a new input frame, update the state of all targets, and return the result
    rm::Result onFrame(const rm::InputFrame &frame);
};
