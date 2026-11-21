#include "targettracker/targetcheck/ArmorTarget.hpp"

bool ArmorTarget::onObserved(const rm::Observation& obs, std::int64_t timespec_ns, std::int64_t frame_dt)
{
    // 线性位置观测
    acceptLinearObservation(obs.x, obs.y, observationDeltaNSecond(timespec_ns), frame_dt);
    // 标记观测成功
    markObserved(timespec_ns);
    return true;
}

bool ArmorTarget::onMissed(std::int64_t frame_dt)
{
    return missStep(frame_dt, 3);
}


