#include "serializer.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <exception>
#include <vector>

namespace sudoku {
namespace serializer {

using matrix_value_depth = ::sudoku::datastruct::Board::matrix_value_depth;
using namespace rapidjson;

void insert(Writer<StringBuffer> &writer,
            const std::vector<matrix_value_depth> &result) {
  writer.StartArray();
  for (auto &values_for_column : result) {
    writer.StartArray();
    for (auto value : values_for_column) {
      writer.Uint(value);
    }
    writer.EndArray();
  }
  writer.EndArray();
}

std::string serialize(Board const &board) {
  StringBuffer sb;
  Writer<StringBuffer> writer(sb);
  std::vector<uint16_t> line_indexes{0, 1, 2, 3, 4, 5, 6, 7, 8};

  writer.StartObject();
  for (auto line_i : line_indexes) {
    std::vector<matrix_value_depth> result;
    board.get_line(line_i, result);
    writer.Key(std::to_string(line_i).c_str());
    insert(writer, result);
  }
  writer.EndObject();
  return sb.GetString();
}

std::vector<matrix_value_depth> complete_line(const Value &vector) {
  std::vector<matrix_value_depth> result;
  for (SizeType i = 0; i < vector.Size(); i++) {
    const Value &set = vector[i];
    assert(set.IsArray());
    matrix_value_depth mvd;
    for (SizeType j = 0; j < set.Size(); j++) {
      assert(set[j].IsUint());
      mvd.insert(set[j].GetUint());
      assert(j < 9);
    }
    result.push_back(mvd);
  }
  return result;
}

Board deserialze(std::string const &json) {
  Board result;
  result.init_all_empty();

  Document doc;
  assert(!doc.Parse(json.c_str()).HasParseError());
  std::vector<uint16_t> line_indexes{0, 1, 2, 3, 4, 5, 6, 7, 8};

  for (uint16_t index : line_indexes) {
    const Value &vector = doc[std::to_string(index).c_str()];
    assert(vector.IsArray());
    result.set_line(index, complete_line(vector));
  }
  return result;
}

} // namespace serializer
} // namespace sudoku
