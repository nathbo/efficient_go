#include <array>
#include <move.h>

Move::Move(int x) { option = x; };
Move::Move(int row, int column) { option = 9 * row + column; };
int Move::to_flatid() { return option; }

std::array<int, 2> Move::to_matrix_id() {
  std::array<int, 2> out;
  int column = option % 9;
  int row = option / 9;
  out = {row, column};
  return out;
}

bool Move::operator==(const int &m) { return (option == m); }
