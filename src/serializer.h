#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "board.h"
#include <string>

namespace sudoku {
namespace serializer {

using Board = datastruct::Board;

std::string serialize(Board const &board);
Board deserialze(std::string const &json);

} // namespace serializer
} // namespace sudoku

#endif // SERIALIZER_H
