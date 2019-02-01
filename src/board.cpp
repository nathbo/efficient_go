#include <iostream>

#include "board.h"
#include "move.h"
#include "colors.h"

using namespace std;

void Board::display()
{
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
          if(board[i*9+j] == Colors::EMPTY)
                cout<< "+" << " " ;
            else if(board[i*9+j] == Colors::WHITE)
                cout<< "O" << " " ;
            else if(board[i*9+j] == Colors::BLACK)
                cout<< "X" << " " ;
        }
        cout<<"\n";
    }
    cout<<"\n";
}

vector<int> Board::get_empty_locations()
{
    vector<int> locations;
    for(int i=0; i<81; i++)
    {
        if(board[i] == Colors::EMPTY){
            Move move (i);
            locations.push_back(move.to_flatid());
        }
    }
   return locations;
}

vector<int> Board::get_available_moves()
{
    vector<int> moves = get_empty_locations();
    moves.push_back(81);
    return moves;
}

vector<int> get_neighbor_locations(int loc)
{
    vector<int> out;
    if(loc%9 > 0){
        out.push_back(loc-1);
    }
    if(loc%9 < 8){
        out.push_back(loc+1);
    }
    if(loc/9 > 0){
        out.push_back(loc-9);
    }
    if(loc/9 < 8){
        out.push_back(loc+9);
    }
    return out;
}

void print(vector<int> vect){
  for(int i: vect){
        cout<<i<<" ";
    }
    cout<<endl;
}
void print(vector<vector<int>> vect){
  for(vector<int> i: vect){
        print(i);
    }
    cout<<endl;
}
void print(set<vector<int>> vect){
    for(auto f : vect) {
        print(f);
    }
    cout<<endl;
}

set<int> Board::get_group(int location)
{
    int color = board[location];
    set<int> chain;
    vector<int> to_check;

    chain.insert(location);
    vector<int> neighbors = get_neighbor_locations(location);
    for(auto n: neighbors) to_check.push_back(n);

    while(to_check.size() > 0){
        int current = to_check.back();
        to_check.pop_back();
        if((board[current] == color) && !(chain.count(current)))
        {
            chain.insert(current);
            vector<int> current_neighbors = get_neighbor_locations(current);
            for(auto n: current_neighbors) to_check.push_back(n);
        }
    }
    return chain;
}

bool Board::check_dead(set<int> chain)
{
    for(auto loc: chain){
        auto neighbors = get_neighbor_locations(loc);
        for(auto n: neighbors){
            if(board[n]==Colors::EMPTY) return 0;
        }
    }
    return 1;
}

int Board::play(Move move, int color)
{
    if(move.is_pass()){
        if(last_move == 81)
            return 1;
    }
    else{
        int loc = move.to_flatid();
        board[loc] = color;

        vector<int> neighbors = get_neighbor_locations(loc);
        for(auto n: neighbors){
            set<int> group = get_group(n);
            if(check_dead(group)){
                for(auto loc: group) board[loc] = Colors::EMPTY;
            }
        }
    }

    last_move = move.to_flatid();
    return 0;
}
