#include <array>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "board.h"
#include "colors.h"
#include "move.h"

using namespace std;

Board::Board(array<int, 81> b, Move m) {
  board = b;
  last_move = m;
}

void Board::display() {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (board[i * 9 + j] == Colors::EMPTY)
        cout << "+"
             << " ";
      else if (board[i * 9 + j] == Colors::WHITE)
        cout << "O"
             << " ";
      else if (board[i * 9 + j] == Colors::BLACK)
        cout << "X"
             << " ";
    }
    cout << "\n";
  }
  cout << "\n";
}

vector<int> Board::get_empty_locations() {
  vector<int> locations;
  for (int loc = 0; loc < 81; loc++) {
    if (board[loc] == Colors::EMPTY) {
      Move move(loc);
      locations.push_back(move.to_flatid());
    }
  }
  return locations;
}

vector<Move> Board::get_available_moves(int player_color) {
  // Player color should be valid
  if (!(player_color == Colors::BLACK || player_color == Colors::WHITE))
    throw invalid_argument("Invalid player color");

  vector<Move> moves;
  // Passing is always an option
  moves.push_back(Move(81));

  // Otherwise, empty locations which do not lead to suicide
  vector<int> empty_locs = get_empty_locations();
  for (auto loc : empty_locs) {
    board[loc] = player_color;

    set<int> group = get_group(loc);
    if (!check_dead(group))
      moves.push_back(loc);
    board[loc] = Colors::EMPTY;
  }

  return moves;
}

vector<int> get_neighbor_locations(int loc) {
  vector<int> out;
  if (loc % 9 > 0)
    out.push_back(loc - 1);
  if (loc % 9 < 8)
    out.push_back(loc + 1);
  if (loc / 9 > 0)
    out.push_back(loc - 9);
  if (loc / 9 < 8)
    out.push_back(loc + 9);
  return out;
}

set<int> Board::get_group(int location) {
  int color = board[location];
  set<int> group;
  vector<int> to_check;

  group.insert(location);
  vector<int> neighbors = get_neighbor_locations(location);
  for (auto n : neighbors)
    to_check.push_back(n);

  while (to_check.size() > 0) {
    int current = to_check.back();
    to_check.pop_back();
    if ((board[current] == color) && !(group.count(current))) {
      group.insert(current);
      vector<int> current_neighbors = get_neighbor_locations(current);
      for (auto n : current_neighbors)
        to_check.push_back(n);
    }
  }
  return group;
}

bool Board::check_dead(set<int> group) {
  for (auto loc : group) {
    auto neighbors = get_neighbor_locations(loc);
    for (auto n : neighbors)
      if (board[n] == Colors::EMPTY)
        return 0;
  }
  return 1;
}

int Board::play(Move move, int color) {
  if (move.is_pass()) {
    if (last_move == 81)
      return 1;
  } else {
    int loc = move.to_flatid();
    board[loc] = color;

    vector<int> neighbors = get_neighbor_locations(loc);
    for (auto n : neighbors) {
      set<int> group = get_group(n);
      if (check_dead(group))
        for (auto loc : group)
          board[loc] = Colors::EMPTY;
    }
  }

  last_move = move.to_flatid();
  return 0;
}

template <class T> T random_select(vector<T> vect) {
  srand(time(NULL));
  int index = rand() % vect.size();
  return vect[index];
}

int Board::random_rollout(int current_player, bool verbose) {
  int ended = 0;
  while (!ended) {
    vector<Move> avail_moves = get_available_moves(current_player);
    Move random_move = random_select(avail_moves);
    ended = play(random_move, current_player);
    current_player = -current_player;
    if (verbose)
      display();
  }
  return ended;
}
