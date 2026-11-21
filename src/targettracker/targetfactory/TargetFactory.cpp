#include "targettracker/targetfactory/TargetFactory.hpp"

#include "targettracker/targetcheck/ArmorTarget.hpp"
#include "targettracker/targetcheck/SpinningTarget.hpp"

std::unique_ptr<TargetBase> TargetFactory::create(const rm::Observation &obs)
{
    switch (obs.type)
    {
    case rm::TargetType::ARMOR:
        return std::make_unique<ArmorTarget>(obs);
    case rm::TargetType::SPINNING:
        return std::make_unique<SpinningTarget>(obs);
    }
    return nullptr; // 枚举出现未知值时由管理器决定拒帧/跳过
}
