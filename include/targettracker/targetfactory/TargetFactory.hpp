#pragma once

#include <memory>
#include "datatype/data_types.hpp"

class TargetBase; // 前向声明即可（unique_ptr 的完整类型由调用处/实现处保证）

class TargetFactory
{
public:
    // 按 obs.type 创建具体目标；未知类型返回 nullptr
    static std::unique_ptr<TargetBase> create(const rm::Observation &obs);
};
