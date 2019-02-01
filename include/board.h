#include <vector>
#include <set>

#include"move.h"
#include"colors.h"

using namespace std;

class Board
{
  int board[81] = {Colors::EMPTY};
  int last_move = -1;
  public:
  int play(Move, int);
  void display();
  vector<int> get_empty_locations();
  vector<int> get_available_moves();
  set<int> get_group(int);
  bool check_dead(set<int>);
};
