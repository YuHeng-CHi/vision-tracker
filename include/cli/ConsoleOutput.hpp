/*
 * @FilePath: include/cli/ConsoleOutput.hpp
 * @Description: 命令行输出：按作业规定的格式打印一帧结果（Result）与其中每个目标。
 */

#pragma once

#include "datatype/data_types.hpp"

// 打印一个目标的信息
void printTarget(const rm::TargetSnapshot &target);

// 打印一帧的结果
void printResult(const rm::Result &result);
