#include "data_seg.hpp"
#include <regex>
#include <spdlog/spdlog.h>
#include <map>

namespace details {

void DataSegParser::Parse(const std::vector<std::string> &clean_input) {
  Reset();
  input_str_ = clean_input;

  auto cmd_type = GetNextCommandType();
  while (cmd_type != CommandType::kEnd) {
    switch (cmd_type) {
      case CommandType::kAlign:
        RunAlignCommand();
        break;
      case CommandType::kSpace:
        RunSpaceCommand();
        break;
      case CommandType::kVar:
        RunVarDefinitionCommand();
        break;
      case CommandType::kData:
        RunDataDefinitionCommand();
        break;
      case CommandType::kEmpty:
      case CommandType::kEnd:
        break;
    }

    index_ += 1;
  }
}


void DataSegParser::RunVarDefinitionCommand() {
  auto cmd = input_str_[index_];
  // remove all the spaces.
  int i = 0;
  while (std::isspace(cmd[i])) {
    i += 1;
    if (i >= cmd.length()) {
      throw std::runtime_error("Expect: Varname");
    }
  }

  auto vname_begin = i;
  while (cmd[i] != ':') {
    i += 1;
    if (i >= cmd.length()) {
      throw std::runtime_error("Expect: ':'");
    }
  }
  auto vname_end = i;
  std::string vname = cmd.substr(vname_begin, i - vname_begin);
  spdlog::info("[Data] Variable : {}", vname);

  if (addr_name_.find(vname) != addr_name_.end()) {
    throw std::runtime_error("Variable has already defined.");
  }

  spdlog::info("[Data] Place Dmem = {}", dmem_usage_);
  addr_name_.insert({vname, dmem_usage_});
}

void DataSegParser::RunDataDefinitionCommand() {
  auto cmd = input_str_[index_];
  int i = 0;
  while (cmd[i] != '.') {
    i += 1;
    if (i >= cmd.length()) {
      throw std::runtime_error("Except: '.'");
    }
  }

  auto inst_begin = i;
  while (std::isspace(cmd[i])) {
    i += 1;
    if (i >= cmd.length()) {
      throw std::runtime_error("Except: '\\s'");
    }
  }

  auto instname = cmd.substr(inst_begin, i - inst_begin);
}

void DataSegParser::RunAlignCommand() {
  auto cmd = input_str_[index_];
  int i = 0;
  while (cmd[i] != '.') {
    i += 1;
    if (i >= cmd.length()) {
      throw std::runtime_error("Except: '.'");
    }
  }

  if (cmd.substr(i + 1, 5) != "align") {
    throw std::
  }

  

  
  
}


}
