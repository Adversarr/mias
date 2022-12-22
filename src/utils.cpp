#include "utils.hpp"
std::string strtolower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return tolower(c); });
  return s;
}

std::size_t find_first(const std::string &s, char c, size_t start) {
  if (start >= s.length()) {
    return start;
  }
  while (s[start] != c) {
    start += 1;
    if (start == s.length()) {
      break;
    }
  }

  return start;
}

std::size_t find_first_ws(const std::string &s, size_t start) {
  if (start >= s.length()) {
    return start;
  }
  while (!std::isspace(s[start])) {
    start += 1;
    if (start == s.length()) {
      break;
    }
  }

  return start;
}

std::size_t find_first_nws(const std::string &s, size_t start) {
  if (start >= s.length()) {
    return start;
  }
  while (std::isspace(s[start])) {
    start += 1;
    if (start == s.length()) {
      break;
    }
  }

  return start;
}
