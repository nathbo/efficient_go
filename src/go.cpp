#include <string>

#include "board.h"
#include "colors.h"
#include "move.h"

int main(int argc, char *argv[]) {
  bool verbose = false;
  int iterations = 1;

  for (int i = 0; i < argc; ++i) {
    if (std::string(argv[i]) == "-v")
      verbose = true;
    if (std::string(argv[i]) == "-N")
      iterations = stoi(argv[i + 1]);
  }

  for (int i = 0; i < iterations; i++) {
    Board b = Board();
    int current_player = Colors::BLACK;
    b.random_rollout(current_player, verbose);
  }

  return 0;
}
