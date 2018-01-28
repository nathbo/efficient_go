/*
Move class

We work with a move as an object defining one of 82 possibilities the player has: 9*9+1.
*/

using namespace std;

class Move
{
  protected:
    int option;
  public:
    Move (int, int);
    Move (int);
    int to_flatid();
    bool is_pass() {return option==81;};
    vector<int> to_matrix_id();
};
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
}