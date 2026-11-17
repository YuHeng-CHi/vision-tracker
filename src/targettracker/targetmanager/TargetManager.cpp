/*
 * @FilePath: src/targettracker/targetmanager/TargetManager.cpp
 * @Description: 目标管理器的占位实现：内容自原 src/main.cpp 原样搬移，未改动逻辑。
 */

#include "targettracker/targetmanager/TargetManager.hpp"

rm::Result TargetManager::onFrame(const rm::InputFrame &frame)
{
    // 占位行为：不做处理，返回"本帧未接受"
    return rm::Result{false, frame.frame_id, {}, 0, 0, 0, 0};
}
