#include <array>
#include <set>
#include <vector>

#include "colors.h"
#include "move.h"

using namespace std;

class Board {
  array<int, 81> board = {Colors::EMPTY};
  Move last_move = -1;
  // int current_player = Colors::BLACK;
public:
  Board(){};
  Board(array<int, 81>, Move);
  int play(Move, int);
  void display();
  vector<int> get_empty_locations();
  vector<Move> get_available_moves(int);
  set<int> get_group(int);
  bool check_dead(set<int>);
  int random_rollout(int, bool);
};
