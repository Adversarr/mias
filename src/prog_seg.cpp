#include "prog_seg.hpp"
#include "defs.hpp"
#include "spdlog/spdlog.h"
#include "utils.hpp"

namespace mias {

uint32_t to_binary(InstrDesc desc) {
  desc.opcode &= 0b111111;
  desc.rs &= 0b11111;
  desc.rt &= 0b11111;
  desc.rd &= 0b11111;
  desc.immediate &= 0xFFFF;
  desc.shamt &= 0b11111;
  desc.func &= 0b111111;

  switch (desc.type) {
  case Instr::kAdd:
  case Instr::kAddu:
  case Instr::kSub:
  case Instr::kSubu:
  case Instr::kAnd:
  case Instr::kOr:
  case Instr::kXor:
  case Instr::kNor:
  case Instr::kSlt:
  case Instr::kSltu:
  case Instr::kSllv:
  case Instr::kSrlv:
  case Instr::kSrav:
    return (desc.opcode << 26) | (desc.rs << 21) | (desc.rt << 16) |
           (desc.rd << 11) | (desc.shamt << 6) | desc.func;

  case Instr::kAddi:
  case Instr::kAddiu:
  case Instr::kAndi:
  case Instr::kOri:
  case Instr::kXori:
  case Instr::kSlti:
  case Instr::kSltiu:
    return desc.opcode << 26 | desc.rs << 21 | desc.rt << 16 | desc.immediate;
  case Instr::kMult:
  case Instr::kMultu:
  case Instr::kDiv:
  case Instr::kDivu:
  case Instr::kMfc0:
  case Instr::kMtc0:
  case Instr::kJalr:
    return (desc.opcode << 26) | (desc.rs << 21) | (desc.rt << 16) |
           (desc.rd << 11) | (desc.shamt << 6) | desc.func;
  case Instr::kBgez:
  case Instr::kBgtz:
  case Instr::kBlez:
  case Instr::kBltz:
  case Instr::kBgezal:
  case Instr::kBltzal:
    return (desc.opcode << 26) | (desc.rs << 21) | (desc.rt << 16) |
           desc.immediate;

  case Instr::kLb:
  case Instr::kLbu:
  case Instr::kLh:
  case Instr::kLhu:
  case Instr::kSb:
  case Instr::kSh:
  case Instr::kLw:
  case Instr::kSw:
    return (desc.opcode << 26) | (desc.rs << 21) | (desc.rt << 16) |
           desc.immediate;

  case Instr::kJ:
  case Instr::kJal:
    return (desc.opcode << 26) | desc.immediate;

  case Instr::kSyscall:
  case Instr::kEret:
    return (desc.opcode << 26) | desc.code;

  case Instr::kSll:
  case Instr::kSrl:
  case Instr::kSra:
  case Instr::kBne:
  case Instr::kBeq:
  case Instr::kLui:
  case Instr::kMfhi:
  case Instr::kMflo:
  case Instr::kMthi:
  case Instr::kMtlo:
  case Instr::kJr:
  case Instr::kBreak:
  case Instr::kJl:
  case Instr::kJle:
  case Instr::kJg:
  case Instr::kJge:
  case Instr::kNop:
    return (desc.opcode << 26) | (desc.rs << 21) | (desc.rt << 16) |
           (desc.rd << 11) | (desc.shamt << 6) | desc.func;

  default:
    spdlog::error("Not Implemented Error");
    throw std::runtime_error("Not Implemented Error");
  };
}

uint32_t interprete_register_or_immediate(std::string str) {
  // TODO:
  // $0 -> 0
  // $s0 -> ?
  // ...
  // 0x12 -> 18
  // 0xFF -> 255
  return 0;
}

InstrDesc from_string(std::string str) {
  auto space = find_first_ws(str);
  auto instr_name = strtolower(str.substr(0, space));

  spdlog::info("[P] Got Instr: {}, Full Instr = {}", instr_name, str);
  // TODO: Impl for each instruction! Parse the instr string here.
  // You can suppose the instr has:
  // 1. no spaces before the instr
  // 2. no comment after the instr.
  return InstrDesc::Nop();
}

enum class InstrAccessType { kNone, kGoto, kDmem };

static InstrAccessType get_iat(InstrDesc desc) {
  switch (desc.type) {
  case Instr::kLb:
  case Instr::kLbu:
  case Instr::kLh:
  case Instr::kLhu:
  case Instr::kSb:
  case Instr::kSh:
  case Instr::kLw:
  case Instr::kSw:
    return InstrAccessType::kDmem;
  case Instr::kBne:
  case Instr::kBeq:
  case Instr::kBgez:
  case Instr::kBgtz:
  case Instr::kBlez:
  case Instr::kBltz:
  case Instr::kBgezal:
  case Instr::kBltzal:
    return InstrAccessType::kGoto;
  default:
    return InstrAccessType::kNone;
  }
}

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
      uint32_t offset;
      if (idesc.flag) {
        if (auto it = flags_.find(idesc.flag.value()); it != flags_.end()) {
          offset = it->second - static_cast<uint32_t>(instr_binary_.size()) - 1;
          spdlog::info("[P] Found flag {}, global index = {}, offset = {}",
                       it->first, it->second, offset);
        } else {
          spdlog::error("Prog Seg: Flag {} not defined.", idesc.flag.value());
          throw std::runtime_error("Prog Seg Error");
        }
      }
      if (idesc.type == Instr::kJ || idesc.type == Instr::kJal) {
        idesc.address = offset;
      } else {
        idesc.immediate = offset;
      }
    }
    binary = to_binary(idesc);
    instr_binary_.push_back(binary);
  }
}

} // namespace mias
