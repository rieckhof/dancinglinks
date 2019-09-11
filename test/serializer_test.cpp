#include "board.h"
#include "serializer.h"
#include <gtest/gtest.h>

namespace s = sudoku::serializer;
using Board = sudoku::datastruct::Board;
using matrix_value_depth = Board::matrix_value_depth;

std::vector<matrix_value_depth> fill_values(std::vector<uint16_t> &&input) {
  std::vector<matrix_value_depth> result(9);
  assert(input.size() == result.size());

  uint16_t index{0};
  for (matrix_value_depth &r : result) {
    r.insert(input.at(index));
    ++index;
  }
  return result;
}

TEST(DeserializeFile, SimpleTextFile){
    std::filesystem::path p = std::filesystem::current_path().append("sudoku");
    sudoku::serializer::TheBoard b;
    std::unordered_map<std::string, u_int16_t> map;
    s::deserialize(p,b,map);
    ASSERT_EQ(b.size(),3);
    const std::vector<uint16_t> expected  {0,2,3};
    const std::vector<uint16_t> expected1 {1,0,0};
    const std::vector<uint16_t> expected2 {2,0,0};

     ASSERT_EQ(expected, b.at(0));
     ASSERT_EQ(expected1, b.at(1));
     ASSERT_EQ(expected2, b.at(2));
     ASSERT_EQ(map.at("01"),2);
     ASSERT_EQ(map.at("02"),3);
     ASSERT_EQ(map.at("10"),1);
     ASSERT_EQ(map.at("20"),2);
     ASSERT_EQ(map.at("22"),0);
}


//TEST(Serializershoudl, serializeBoardtoJson) {
//  Board b{};
//  b.init_all_empty();
//  std::vector<matrix_value_depth> v2 =
//      fill_values(std::vector<uint16_t>{9, 8, 7, 6, 5, 4, 3, 2, 1});
//  std::vector<matrix_value_depth> v3 =
//      fill_values(std::vector<uint16_t>{1, 2, 3, 4, 5, 6, 7, 8, 9});
//  std::vector<matrix_value_depth> v4 =
//      fill_values(std::vector<uint16_t>{1, 2, 3, 4, 5, 9, 8, 7, 6});
//  b.set_column(2, v2);
//  b.set_column(3, v3);
//  b.set_column(4, v4);

//  std::string expected{"{\"0\":[[],[],[9],[1],[1],[],[],[],[]],"
//                       "\"1\":[[],[],[8],[2],[2],[],[],[],[]],"
//                       "\"2\":[[],[],[7],[3],[3],[],[],[],[]],"
//                       "\"3\":[[],[],[6],[4],[4],[],[],[],[]],"
//                       "\"4\":[[],[],[5],[5],[5],[],[],[],[]],"
//                       "\"5\":[[],[],[4],[6],[9],[],[],[],[]],"
//                       "\"6\":[[],[],[3],[7],[8],[],[],[],[]],"
//                       "\"7\":[[],[],[2],[8],[7],[],[],[],[]],"
//                       "\"8\":[[],[],[1],[9],[6],[],[],[],[]]}"};

//  std::string result{s::serialize(b)};
//  EXPECT_EQ(result, expected);
//}
//void create_result(Board const &result, std::vector<matrix_value_depth> const &expected,
//           uint16_t line) {
//  std::vector<matrix_value_depth> real;
//  result.get_column(line, real);
//  EXPECT_EQ(real, expected);
//}

//TEST(SerializerShould, deserializeJsonToBoard) {
//  std::vector<matrix_value_depth> expected2 =
//      fill_values(std::vector<uint16_t>{9, 8, 7, 6, 5, 4, 3, 2, 1});
//  std::vector<matrix_value_depth> expected3 =
//      fill_values(std::vector<uint16_t>{1, 2, 3, 4, 5, 6, 7, 8, 9});
//  std::vector<matrix_value_depth> expected4 =
//      fill_values(std::vector<uint16_t>{1, 2, 3, 4, 5, 9, 8, 7, 6});

//  std::string input{"{\"0\":[[],[],[9],[1],[1],[],[],[],[]],"
//                    "\"1\":[[],[],[8],[2],[2],[],[],[],[]],"
//                    "\"2\":[[],[],[7],[3],[3],[],[],[],[]],"
//                    "\"3\":[[],[],[6],[4],[4],[],[],[],[]],"
//                    "\"4\":[[],[],[5],[5],[5],[],[],[],[]],"
//                    "\"5\":[[],[],[4],[6],[9],[],[],[],[]],"
//                    "\"6\":[[],[],[3],[7],[8],[],[],[],[]],"
//                    "\"7\":[[],[],[2],[8],[7],[],[],[],[]],"
//                    "\"8\":[[],[],[1],[9],[6],[],[],[],[]]}"};

//  Board result{s::deserialze(input)};
//  create_result(result, expected2, 2);
//  create_result(result, expected3, 3);
//  create_result(result, expected4, 4);
//}

//TEST(SerializerShould, beASymetricFunction) {
//  std::string easy_input{"{\"0\":[[],[],[6],[4],[],[9],[5],[],[]],"
//                         "\"1\":[[],[],[3],[],[2],[],[1],[],[]],"
//                         "\"2\":[[9],[1],[],[],[7],[],[],[2],[8]],"
//                         "\"3\":[[6],[],[],[7],[],[3],[],[],[1]],"
//                         "\"4\":[[],[4],[7],[],[8],[],[3],[5],[]],"
//                         "\"5\":[[5],[],[],[2],[],[1],[],[],[9]],"
//                         "\"6\":[[4],[2],[],[],[9],[],[],[3],[6]],"
//                         "\"7\":[[],[],[1],[],[3],[],[9],[],[]],"
//                         "\"8\":[[],[],[9],[8],[],[4],[2],[],[]]}"};

//  sudoku::datastruct::Board b{sudoku::serializer::deserialze(easy_input)};
//  std::string result{sudoku::serializer::serialize(b)};

//  EXPECT_EQ(easy_input, result);
//}
