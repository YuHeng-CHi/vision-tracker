#include "TargetBase.hpp"

class SpinningTarget : public TargetBase {
public:
    SpinningTarget(const rm::Observation &obs) : TargetBase(obs) {}
    virtual ~SpinningTarget() {}

    // 旋转装甲的追踪函数：传观察数据、纳秒时间戳、与上一成功帧的纳秒间隔
    bool onObserved(const rm::Observation& obs, std::int64_t timespec_ns, std::int64_t frame_dt) override;
    // 旋转装甲的丢失函数：传入与上一成功帧的纳秒间隔
    bool onMissed(std::int64_t frame_dt) override;
};