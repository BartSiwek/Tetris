/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_PERFORMANCE_FRAMESTATISTICS_H_
#define ELG_PERFORMANCE_FRAMESTATISTICS_H_

#include "Elg/Utilities/Types.h"
#include "Elg/Clock/GameClock.h"

namespace Elg {
namespace Performance {

class FrameStatistics {
 public:
  static void RegisterFrameTime(Elg::Clock::GameClock::Duration frame_time);

  static uint32 GetCurrentFrameTime();
  static uint32 GetAvgFrameTime();
  static uint32 GetCurrentFps();
  static uint32 GetAvgFps();

 private:
  typedef Elg::Clock::Nanoseconds Duration;

  static const Duration RESET_INTERVAL;
  static const uint32 OLD_FPS_WEIGHT_NUM;
  static const uint32 NEW_FPS_WEIGHT_NUM;
  static const uint32 FPS_WEIGHT_DEN;
  static const uint32 OLD_MS_WEIGHT_NUM;
  static const uint32 NEW_MS_WEIGHT_NUM;
  static const uint32 MS_WEIGHT_DEN;

  static Duration s_accumulated_frame_time_;
  static uint32 s_frame_counter_;

  static Duration s_current_frame_time_;  // Stores the frame time in ns
  static Duration s_avg_frame_time_;  // Stores the avg frame time in ns
  static uint32 s_current_fps_;  // Stores the FPS * 1000 (for accuracy to the 3rd decimal place)
  static uint32 s_avg_fps_;  // Stores the avg FPS * 1000 (for accuracy to the 3rd decimal place)
};

}  // namespace Performance
}  // namespace Elg

#endif  // ELG_PERFORMANCE_FRAMESTATISTICS_H_
