
#include "serializer.h"
#include <gtest/gtest.h>

namespace s = sudoku::serializer;

TEST(DeserializeFile, SimpleTextFile) {
  std::filesystem::path p = std::filesystem::current_path().append("sudoku");
  std::unordered_map<std::string, u_int16_t> map;
  s::deserialize(p, map);
  ASSERT_EQ(map.size(), 9);
  const std::vector<uint16_t> expected{0, 2, 3};
  const std::vector<uint16_t> expected1{1, 0, 0};
  const std::vector<uint16_t> expected2{2, 0, 0};

  ASSERT_EQ(map.at("01"), 2);
  ASSERT_EQ(map.at("02"), 3);
  ASSERT_EQ(map.at("10"), 1);
  ASSERT_EQ(map.at("20"), 2);
  ASSERT_EQ(map.at("22"), 0);
}
