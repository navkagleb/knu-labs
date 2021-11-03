#pragma once

#include <cstdint>
#include <vector>
#include <functional>

#include "command_line_args.hpp"
#include "process_config.hpp"
#include "scheduling_algorithm.hpp"

namespace os_lab2 {

class SchedulingSystem {
 public:
  static SchedulingSystem& GetInstance();

 private:
  SchedulingSystem();

 public:
  void Init(const CommandLineArgs& args);
  void Run(SchedulingAlgorithm&& algorithm);

 private:
  std::size_t                 simulation_time_;
  std::vector<ProcessConfig>  processes_;
};

} // namespace os_lab2
