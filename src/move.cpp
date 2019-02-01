#include <move.h>

Move::Move(int x)
{
  option = x;
};
Move::Move(int row, int column)
{
  option = 9*row+column;
};
int Move::to_flatid()
{
  return option;
}

vector<int> Move::to_matrix_id()
{
  vector<int> out;
  int column = option%9;
  int row = option/9;
  out = {row, column};
  return out;
}
