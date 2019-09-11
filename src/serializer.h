#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "board.h"
#include <string>
#include <map>
#include <vector>
#include <filesystem>

namespace sudoku {
namespace serializer {

using Board = datastruct::Board;
using TheBoard = std::vector<std::vector<uint16_t>>;

//std::string serialize(Board const &board);
//Board deserialze(std::string const &json);

void deserialize(std::filesystem::path p, TheBoard& b,std::unordered_map<std::string, u_int16_t>& map);


} // namespace serializer
} // namespace sudoku

#endif // SERIALIZER_H
