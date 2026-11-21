#include "targettracker/targetfactory/FrameValidator.hpp"

bool FrameValidator::validate(const rm::InputFrame& frame) const
{
    if (frame.frame_id <= last_frame_id_)
    {
        std::cerr << "FrameValidator: frame_id " << frame.frame_id << " is not greater than last_frame_id " << last_frame_id_ << std::endl;
        return false;
    }
    if (frame.timestamp_ns <= last_timestamp_ns_)
    {
        std::cerr << "FrameValidator: timestamp_ns " << frame.timestamp_ns << " is not greater than last_timestamp_ns " << last_timestamp_ns_ << std::endl;
        return false;
    }
    return true;
}

void FrameValidator::commit(const rm::InputFrame& frame)
{
    last_frame_id_ = frame.frame_id;
    last_timestamp_ns_ = frame.timestamp_ns;
}