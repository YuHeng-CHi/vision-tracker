#pragma once
#include "datatype/data_types.hpp"


class FrameValidator
{
protected:
    int last_frame_id_ = -1;
    std::int64_t last_timestamp_ns_ = -1;
public:
//frame_id必须大于最近一次成功处理的帧号，timestamp_ns必须严格大于最近成功帧，否则整帧拒绝
    bool validate(const rm::InputFrame& frame) const;
//record the frame_id and timestamp_ns of the last successfully processed frame
    void commit(const rm::InputFrame& frame);
//get the frame_id and timestamp_ns of the last successfully processed frame
    int lastFrameId() const { return last_frame_id_; }
    std::int64_t lastTimestampNs() const { return last_timestamp_ns_; }
};