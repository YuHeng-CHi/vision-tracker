#pragma once
#include "TargetBase.hpp"

class ArmorTarget : public TargetBase {

public:
    ArmorTarget(const rm::Observation &obs) : TargetBase(obs) {}
    virtual ~ArmorTarget() {}

    // 追踪函数：传观察数据、纳秒时间戳、与上一成功帧的纳秒间隔
    bool onObserved(const rm::Observation& obs, std::int64_t timespec_ns, std::int64_t frame_dt) override;
    // 丢失函数：传入与上一成功帧的纳秒间隔
    bool onMissed(std::int64_t frame_dt) override;

};