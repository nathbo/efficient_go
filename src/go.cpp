#include <iostream>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <set>

#include"move.h"
#include"board.h"
#include"colors.h"

using namespace std;

int seed = time(NULL);
// int seed = 0;

Move get_random_move(Board board)
{
    vector<int> empty_locs = board.get_available_moves();
    srand(seed);
    seed++;
    int index = rand() % empty_locs.size();
    return empty_locs[index];
}

void play_random_game(bool verbose)
{
    Board b;
    int current_player = Colors::BLACK;
    bool ended = 0;

    while(!ended){
        Move move_rand = get_random_move(b);
        ended = b.play(move_rand, current_player);
        current_player = -current_player;
        if (verbose) b.display();
    }
}

int main(int argc, char* argv[])
{
    bool verbose = false;
    int iterations = 1;
    for (int i = 0; i < argc; ++i)
    {
        if (string(argv[i]) == "-v") verbose = true;
        if (string(argv[i]) == "-N") iterations = stoi(argv[i+1]);
    }

    for(int i=0; i<iterations; i++){
        play_random_game(verbose);
    }
    return 0;
}
