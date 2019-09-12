#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <filesystem>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace sudoku {
namespace serializer {

void deserialize(std::filesystem::path const& p,
                 std::unordered_map<std::string, u_int16_t>& map);

}  // namespace serializer
}  // namespace sudoku

#endif  // SERIALIZER_H
