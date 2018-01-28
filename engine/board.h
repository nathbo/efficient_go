using namespace std;

int EMPTY = 0;
int WHITE = -1;
int BLACK = 1;

class Board
{
    int board[81] = {0};
    int last_move = -1;
  public:
    int play(Move, int);
    void display();
    vector<int> get_empty_locations();
    vector<int> get_available_moves();
    set<int> get_group(int);
    bool check_dead(set<int>);
};
