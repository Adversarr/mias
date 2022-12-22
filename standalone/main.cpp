#include "data_seg.hpp"
#include <cxxopts.hpp>
#include <iomanip>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// TODO:
// preprocess:
//  1. no space front.
//  2. no comment messages
//  3. seperate : with definition.
//

int main(int argc, char *argv[]) {
  auto default_logger = spdlog::stderr_color_st("el");
  spdlog::set_default_logger(default_logger);
  spdlog::set_level(spdlog::level::debug);
  return 0;
}
