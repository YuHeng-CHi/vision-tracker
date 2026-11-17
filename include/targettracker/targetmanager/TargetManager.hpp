/*
 * @FilePath: include/targettracker/targetmanager/TargetManager.hpp
 * @Description: 目标管理器：持有所有目标，逐帧处理 onFrame。
 */

#pragma once

#include "datatype/data_types.hpp"

// 当前为占位实现（见 src/targettracker/targetmanager/TargetManager.cpp）：
// 尚未实现任何跟踪逻辑，只保证 main 可以编译运行。
// 正式实现时保持 onFrame 签名不变，在 TargetManager.cpp 中补全逻辑即可，main 不需要改动。
// 开始时目标管理器内无目标；收到此前未创建过的编号时创建目标，main 不参与。

class TargetManager
{
public:
    rm::Result onFrame(const rm::InputFrame &frame);
};
