#pragma once
#include "datatype/data_types.hpp"
/*
对一帧下的n个目标中的一个进行检测
*/

class TargetBase {
protected:
    int id_;
    rm::TargetType type_;
    rm::TargetStatus status_;
    double x_;     
    double y_;
    double angle_;
    double velocity_x_;
    double velocity_y_;
    double angular_velocity_;
    double predicted_x_;
    double predicted_y_;
    double predicted_angle_;
    int missed_count_;
    bool has_observation_;
    std::int64_t last_observed_timestamp_ns_;


// 功能函数
    // 识别首帧与Lost后的重追踪。
    bool isRestart(void) const;
    // 计算两次观察的时间差，单位：纳秒
    std::int64_t observationDeltaNSecond(const std::int64_t &timespec_ns) const;
    // 首帧/重追踪速度清零；正常用 位置差 / (obs_dt_ns/1e9) 算速度；预测 = 位置 + 速度 × (frame_delta_ns/1e9)
    bool acceptLinearObservation(double obs_x, double obs_y, std::int64_t obs_dt_ns, std::int64_t frame_delta_ns);
    // 规则同位置：算角速度与预测角；普通装甲不调用
    bool acceptAngularObservation(double obs_angle, std::int64_t obs_dt_ns, std::int64_t frame_delta_ns);
    // 恢复 TRACK、missed_count 清零、记录最近观测时间戳
    bool markObserved(std::int64_t timestamp_ns);
    // 状态转 LOST，位置/速度/预测/角度/角速度全部清零（对象保留）
    bool markLost();
    // missed_count +1；未达阈值推进预测；达阈值调用 markLost 并返回 false
    bool missStep(std::int64_t frame_delta_ns, size_t lost_threshold);
    // 未观测但 TRACK 时：predicted_angle += angular_velocity × (frame_delta_ns/1e9)
    bool advanceAngularPrediction(std::int64_t frame_delta_ns);

public:
//初始化对象
    TargetBase(const rm::Observation &obs) : id_{obs.id},
                                             type_{obs.type},
                                             status_{rm::TargetStatus::TRACK},
                                             x_{obs.x},
                                             y_{obs.y},
                                             angle_{obs.angle},
                                             velocity_x_{0.0},
                                             velocity_y_{0.0},
                                             angular_velocity_{0.0},
                                             predicted_x_{obs.x},
                                             predicted_y_{obs.y},
                                             predicted_angle_{obs.angle},
                                             missed_count_{0},
                                             has_observation_{false},
                                             last_observed_timestamp_ns_{0}
    {
        //log::write << "TargetBase has been constructed"
    }
    virtual ~TargetBase() { 
        // log::write << "TargetBase has been destructed"
    };
//接口函数：将普通装甲和旋转装甲的实现分开
    //追踪函数：传观察数据、纳秒时间戳、与上一成功帧的纳秒间隔
    virtual bool onObserved(const rm::Observation& obs, std::int64_t timespec_ns, std::int64_t frame_dt) = 0;
    //丢失函数：传入与上一成功帧的纳秒间隔
    virtual bool onMissed(std::int64_t frame_dt) = 0;
// 只读访问器
    int getTargetID(void) const { return id_; };
    rm::TargetType getTargetType(void) const { return type_; };
    rm::TargetStatus getTargetStatus(void) const { return status_; };
    int getTargetMissedCount(void) const { return missed_count_; };
// 所有字段就绪后最后写；把公共状态导出给 ResultBuilder / 打印函数
    rm::TargetSnapshot snapshot() const;
};