/*
Move class

We work with a move as an object defining one of 82 possibilities the player has: 9*9+1.
*/
#pragma once
#include <vector>

using namespace std;

class Move
{
  protected:
    int option;
  public:
    Move (int, int);
    Move (int);
    Move () {};
    int to_flatid();
    bool is_pass() {return option==81;};
    vector<int> to_matrix_id();
};
