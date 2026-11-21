/*
 * @FilePath: src/targettracker/targetcheck/TargetBase.cpp
 * @Description: TargetBase 成员函数定义（仅函数骨架，功能待实现）。
 */

#include "targettracker/targetcheck/TargetBase.hpp"

// 识别首帧与 LOST 后的重追踪
bool TargetBase::isRestart(void) const
{
    return !this->has_observation_ || this->status_ == rm::TargetStatus::LOST;
}

// 计算两次观察的ns时间差
std::int64_t TargetBase::observationDeltaNSecond(const std::int64_t &timespec_ns) const
{
    if (!this->has_observation_)
    {
        return 0;
    }
    return timespec_ns - this->last_observed_timestamp_ns_;
}

// 首帧/重追踪速度清零；正常用位置差 / (obs_dt_ns/1e9) 算速度；预测 = 位置 + 速度 × (frame_delta_ns/1e9)
bool TargetBase::acceptLinearObservation(double obs_x, double obs_y, std::int64_t obs_dt_ns, std::int64_t frame_delta_ns)
{
    // 时间统一为纳秒，参与公式前换算成秒
    const double obs_dt_seconds = static_cast<double>(obs_dt_ns) / 1e9;
    const double frame_delta_seconds = static_cast<double>(frame_delta_ns) / 1e9;

    if (this->isRestart() || obs_dt_seconds <= 0.0){
        // 首次观测 / LOST 后重新追踪：不使用很久以前的位置计算速度
        this->velocity_x_ = 0.0;
        this->velocity_y_ = 0.0;
    }
    else{
        // 先用“上一次观测的位置”（当前 x_/y_）算速度，再覆盖
        this->velocity_x_ = (obs_x - this->x_) / obs_dt_seconds;
        this->velocity_y_ = (obs_y - this->y_) / obs_dt_seconds;
    }

    // 更新为本次观测位置
    this->x_ = obs_x;
    this->y_ = obs_y;

    // 预测下一帧位置
    this->predicted_x_ = this->x_ + this->velocity_x_ * frame_delta_seconds;
    this->predicted_y_ = this->y_ + this->velocity_y_ * frame_delta_seconds;
    return true;
}

// 规则同位置：算角速度与预测角；普通装甲不调用
bool TargetBase::acceptAngularObservation(double obs_angle, std::int64_t obs_dt_ns, std::int64_t frame_delta_ns)
{
    const double obs_dt_seconds = static_cast<double>(obs_dt_ns) / 1e9;
    const double frame_delta_seconds = static_cast<double>(frame_delta_ns) / 1e9;

    if (this->isRestart() || obs_dt_seconds <= 0.0){
        this->angular_velocity_ = 0.0;
    }else{
        // 用上一次观测的角度（当前 angle_）算角速度
        this->angular_velocity_ = (obs_angle - this->angle_) / obs_dt_seconds;
    }

    // 更新角度
    this->angle_ = obs_angle;
    this->predicted_angle_ = this->angle_ + this->angular_velocity_ * frame_delta_seconds;
    return true;
}

// 恢复 TRACK、missed_count 清零、记录最近观测时间戳
bool TargetBase::markObserved(std::int64_t timestamp_ns)
{
    status_ = rm::TargetStatus::TRACK;
    missed_count_ = 0;
    has_observation_ = true;
    last_observed_timestamp_ns_ = timestamp_ns;
    return true;
}

// 状态转 LOST，位置/速度/预测/角度/角速度全部清零（对象保留）
bool TargetBase::markLost()
{
    status_ = rm::TargetStatus::LOST;
    x_ = 0.0;
    y_ = 0.0;
    angle_ = 0.0;
    velocity_x_ = 0.0;
    velocity_y_ = 0.0;
    angular_velocity_ = 0.0;
    predicted_x_ = 0.0;
    predicted_y_ = 0.0;
    predicted_angle_ = 0.0;
    return true;
}

// missed_count +1；未达阈值推进预测；达阈值调用 markLost 并返回 false
bool TargetBase::missStep(std::int64_t frame_delta_ns, size_t lost_threshold)
{
    // 已经 LOST：不再变化，让 missed_count 冻结在阈值处
    if (status_ == rm::TargetStatus::LOST){
        return false;
    }

    ++missed_count_;
    
    if (static_cast<size_t>(missed_count_) >= lost_threshold){
        markLost();
        return false;
    }

    const double frame_delta_seconds = static_cast<double>(frame_delta_ns) / 1e9;
    predicted_x_ += velocity_x_ * frame_delta_seconds;
    predicted_y_ += velocity_y_ * frame_delta_seconds;
    return true;
}

// 未观测但 TRACK 时：predicted_angle += angular_velocity × (frame_delta_ns/1e9)
bool TargetBase::advanceAngularPrediction(std::int64_t frame_delta_ns)
{
    if(status_ != rm::TargetStatus::TRACK)
    {
        return false;
    }
    predicted_angle_ += angular_velocity_ * (static_cast<double>(frame_delta_ns) / 1e9);
    return true;
}

// 所有字段就绪后最后写；把公共状态导出给 ResultBuilder / 打印函数
rm::TargetSnapshot TargetBase::snapshot() const
{
    return rm::TargetSnapshot{
        id_,
        type_,
        status_,
        x_,
        y_,
        velocity_x_,
        velocity_y_,
        predicted_x_,
        predicted_y_,
        angle_,
        angular_velocity_,
        predicted_angle_,
        missed_count_
    };
}
