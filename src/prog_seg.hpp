#pragma once


#include <vector>
namespace details {

class ProgSegParser {

public:
  ProgSegParser();

  void Reset();

  void Parse(const std::vector<std::string> &clean_input);

public:
  std::vector<uint32_t> GenerateOutput() const;

private:

};

}
