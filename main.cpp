/*
 * @FilePath: main.cpp
 * @Description: 程序入口：创建目标管理器、逐帧调用 onFrame 并打印结果。
 */

#include "cli/ConsoleOutput.hpp"
#include "datatype/data_types.hpp"
#include "targettracker/targetmanager/TargetManager.hpp"
#include "testdatas/test_frames.hpp"

int main()
{
    TargetManager target_manager;

    for (const auto &frame : testFrames())
    {
        rm::Result result = target_manager.onFrame(frame);
        printResult(result); // 用于输出结果的函数
    }
    return 0;
}
