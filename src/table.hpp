#pragma once
#include <map>
#include <vector>

namespace mias {

struct DmemDesc {
  std::map<std::string, std::size_t> variables;
  std::vector<uint8_t> dmem_value;
};

struct PmemDesc {
  std::map<std::string, std::size_t> flags;
  std::vector<uint32_t> binary_instrs;
};

} // namespace mias
