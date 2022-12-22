#pragma once

#include <string>
#include <strstream>

std::string strtolower(std::string s);
std::size_t find_first(const std::string& s, char c, size_t start = 0);
std::size_t find_first_nws(const std::string& s, size_t start = 0);
std::size_t find_first_ws(const std::string& s, size_t start = 0);


