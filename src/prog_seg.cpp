#include "prog_seg.hpp"
#include "defs.hpp"
#include "spdlog/spdlog.h"

namespace mias {

enum class InstrAccessType { kNone, kGoto, kDmem };

static InstrAccessType get_iat(InstrDesc) { return InstrAccessType::kNone; }

void ProgSegParser::Parse(const std::vector<std::string> &clean_input) {
  input_str_ = clean_input;
  MarkAllFlags();
  GenerateBinary();
}

void ProgSegParser::SetDmemDesc(const DmemDesc &desc) { dmem_desc_ = desc; }

void ProgSegParser::MarkAllFlags() {
  for (const auto &instr : input_str_) {
    if (instr.find(':') != std::string::npos) {
      // instr is flag
      auto pos = instr.find(':');
      auto flag = instr.substr(0, pos);
      AddFlag(flag);
    } else {
      instr_str_.push_back(instr);
      spdlog::debug("[P] Add Instr: {}", instr);
    }
  }
}

void ProgSegParser::AddFlag(std::string name) {
  if (flags_.find(name) == flags_.end()) {
    spdlog::debug("[P] Add Flag {}, global offset = {}", name,
                  instr_str_.size());
    flags_.emplace(name, instr_str_.size());
  } else {
    spdlog::error("Prog Seg: Flags redefinition.");
    throw std::runtime_error("Prog Seg Error");
  }
}

void ProgSegParser::GenerateBinary() {
  for (auto instr : instr_str_) {
    auto idesc = from_string(instr);
    uint32_t binary = 0;
    auto iat = get_iat(idesc);
    if (iat == InstrAccessType::kDmem) {
      // Accessing Data Memory
      if (idesc.flag) {
        // has flag, -> find in dmem

        if (auto it = dmem_desc_.variables.find(idesc.flag.value());
            it != dmem_desc_.variables.end()) {
          // found.
          spdlog::info("[P] Found dmem flag: {} : {}", it->first, it->second);
          idesc.immediate = static_cast<uint32_t>(it->second);
        } else {
          spdlog::error("Prog Seg: Unknown Flag {}", idesc.flag.value());
          throw std::runtime_error("Prog Seg Error");
        }
      }
    } else if (iat == InstrAccessType::kGoto) {
      // Accessing Program Counter
    }
    binary = to_binary(idesc);
  }
}

} // namespace mias
