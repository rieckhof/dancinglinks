#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "board.h"
#include <string>
#include <map>
#include <vector>
#include <filesystem>

namespace sudoku {
namespace serializer {

void deserialize(std::filesystem::path p, std::unordered_map<std::string, u_int16_t>& map);


} // namespace serializer
} // namespace sudoku

#endif // SERIALIZER_H
