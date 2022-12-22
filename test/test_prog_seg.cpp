#include <doctest/doctest.h>
#include "prog_seg.hpp"
TEST_CASE("Prog Seg") {
  mias::ProgSegParser parser;
  std::vector<std::string> asm_in{
    "start:",
    "add $1, $2, $3",
    "end:",
    "jr $31"
  };


  parser.Parse(asm_in);
}
