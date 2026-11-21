/*
 * @FilePath: src/targettracker/targetmanager/TargetManager.cpp
 * @Description: 目标管理器的占位实现：内容自原 src/main.cpp 原样搬移，未改动逻辑。
 */

#include "targettracker/targetmanager/TargetManager.hpp"

TargetBase* TargetManager::findTarget(int id)
{
    for(const auto& target : targets_)
    {
        if(target->getTargetID() == id)
        {
            return target.get();
        }
    }
    return nullptr;
}

TargetBase* TargetManager::createTarget(const rm::Observation& obs)
{
    if(findTarget(obs.id) == nullptr)
    {
        auto temptr = TargetFactory::create(obs);
        targets_.push_back(std::move(temptr));
        return targets_.back().get();
    }
    return nullptr;
}

void TargetManager::updateTargetTimestamp(const rm::Observation& obs, std::int64_t timestamp_ns , std::int64_t frame_dt)
{
    TargetBase* target = findTarget(obs.id);
    if(target != nullptr)
    {
        target->onObserved(obs, timestamp_ns,frame_dt);
    }
}

void TargetManager::handleUnobservedTargets(const std::vector<int> &observed_ids,std::int64_t frame_dt)
{
    for (const auto &target : targets_)
    {
        const int id = target->getTargetID();
        if (std::find(observed_ids.begin(), observed_ids.end(), id) == observed_ids.end())
        {
            target->onMissed(frame_dt);
        }
    }
}

void TargetManager::commitTargets(const rm::InputFrame& frame)
{
    frame_validator_.commit(frame);
}

rm::Result TargetManager::buildResult(int frame_id, bool accepted)
{
    return ResultBuilder::build(frame_id, targets_, accepted);
}

rm::Result TargetManager::onFrame(const rm::InputFrame &frame)
{
    // 1. 帧级 + 观测级校验（只读，不修改任何状态）
    if (!frame_validator_.validate(frame) ||
        !observation_validator_.validate(frame))
    {
        return buildResult(frame.frame_id, false);
    }

    // 2. 类型一致性检查：已存在目标的类型不能变
    //    必须在修改任何目标之前完成，才能保证“整帧拒绝”
    for (const auto &obs : frame.observations)
    {
        TargetBase *target = findTarget(obs.id);
        if (target != nullptr && target->getTargetType() != obs.type)
        {
            return buildResult(frame.frame_id, false);
        }
    }

    // 3. 帧间隔：相对“上一成功帧”；首帧没有上一帧，取 0
    std::int64_t frame_dt_ns = 0;
    if (frame_validator_.lastFrameId() >= 0)
    {
        frame_dt_ns = frame.timestamp_ns - frame_validator_.lastTimestampNs();
    }

    // 4. 逐观测更新/创建
    std::vector<int> observed_ids;
    observed_ids.reserve(frame.observations.size());
    for (const auto &obs : frame.observations)
    {
        TargetBase *target = findTarget(obs.id);
        if (target == nullptr)
        {
            target = createTarget(obs); // 内部用 TargetFactory
            if (target == nullptr)
            {
                return buildResult(frame.frame_id, false); // 原子性：直接拒绝
            }
            target->onObserved(obs, frame.timestamp_ns, frame_dt_ns);
        }
        else
        {
            updateTargetTimestamp(obs, frame.timestamp_ns, frame_dt_ns);
        }
        observed_ids.push_back(obs.id);
    }

    // 5. 本帧没观测到的目标：丢失计数 + 预测推进
    handleUnobservedTargets(observed_ids, frame_dt_ns);

    // 6. 整帧成功后才提交基准帧号/时间戳
    commitTargets(frame);

    return buildResult(frame.frame_id, true);
}
