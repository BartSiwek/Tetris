/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include "Elg/Performance/FrameStatistics.h"

#include "Elg/Clock/Duration.h"

namespace Elg {
namespace Performance {

const FrameStatistics::Duration FrameStatistics::RESET_INTERVAL = Elg::Clock::Seconds(1);
const uint32 FrameStatistics::OLD_FPS_WEIGHT_NUM = 9;
const uint32 FrameStatistics::NEW_FPS_WEIGHT_NUM = 1;
const uint32 FrameStatistics::FPS_WEIGHT_DEN = FrameStatistics::OLD_FPS_WEIGHT_NUM +
                                               FrameStatistics::NEW_FPS_WEIGHT_NUM;
const uint32 FrameStatistics::OLD_MS_WEIGHT_NUM = 95;
const uint32 FrameStatistics::NEW_MS_WEIGHT_NUM = 5;
const uint32 FrameStatistics::MS_WEIGHT_DEN = FrameStatistics::OLD_MS_WEIGHT_NUM +
                                              FrameStatistics::NEW_MS_WEIGHT_NUM;

FrameStatistics::Duration FrameStatistics::s_accumulated_frame_time_ = FrameStatistics::Duration(0);
uint32 FrameStatistics::s_frame_counter_ = 0;

FrameStatistics::Duration FrameStatistics::s_current_frame_time_ = FrameStatistics::Duration(0);
FrameStatistics::Duration FrameStatistics::s_avg_frame_time_ = FrameStatistics::Duration(0);

uint32 FrameStatistics::s_current_fps_ = 0;
uint32 FrameStatistics::s_avg_fps_ = 0;

void FrameStatistics::RegisterFrameTime(Elg::Clock::GameClock::Duration frame_time) {
  // Assert that frame time makes sense
  ASSERT(frame_time.Count() >= 0, "The frame time should be positive");

  // Accumulate time and call count
  s_accumulated_frame_time_ += Elg::Clock::DurationCast<Duration>(frame_time);
  ++s_frame_counter_;

  if (s_accumulated_frame_time_ > RESET_INTERVAL) {
    // Calculate ms statistics
    s_current_frame_time_ = s_accumulated_frame_time_ / s_frame_counter_;
    s_avg_frame_time_ = (OLD_MS_WEIGHT_NUM * s_avg_frame_time_ +
                         NEW_MS_WEIGHT_NUM * s_current_frame_time_) / MS_WEIGHT_DEN;

    // Account for the fact we store time in ns (10^9 accounts for ns 10^3 accounts for storing 1000 * FPS)
    uint64 frame_count_64bit = static_cast<uint64>(1000000000) * static_cast<uint64>(1000 * s_frame_counter_);
    s_current_fps_ = static_cast<uint32>(frame_count_64bit / s_accumulated_frame_time_.Count());

    s_avg_fps_ = (OLD_FPS_WEIGHT_NUM * s_avg_fps_ + NEW_FPS_WEIGHT_NUM * s_current_fps_) / FPS_WEIGHT_DEN;

    // Clear the accumulators
    s_accumulated_frame_time_ = Duration(0);
    s_frame_counter_ = 0;
  }
}

uint32 FrameStatistics::GetCurrentFrameTime() {
  return static_cast<uint32>(Elg::Clock::DurationCast<Elg::Clock::Microseconds>(s_current_frame_time_).Count());
}

uint32 FrameStatistics::GetAvgFrameTime() {
  return static_cast<uint32>(Elg::Clock::DurationCast<Elg::Clock::Microseconds>(s_avg_frame_time_).Count());
}

uint32 FrameStatistics::GetCurrentFps() {
  return s_current_fps_;
}

uint32 FrameStatistics::GetAvgFps() {
  return s_avg_fps_;
}

}  // namespace Performance
}  // namespace Elg
