# C++ Go engine
During my work on my [Data-Innovation Lab university
project](https://github.com/nathbo/GO_DILab), we developed smart agents to play
the game of [Go](https://en.wikipedia.org/wiki/Go_(game)). Many modern methods
use some kind of Monte-Carlo play to evaluate board positions, but due to
performance issues we could not directly try that in Python.

This is my approach to create a small but fast Go engine in C++ to randomly play
Go until the game finishes.

Future work would be about importing this to Python in order to quickly simulate
a random playoff for a given board.

# Usage
Create the executable with:
```make```

You can then watch random playoffs with
```./a.out -v -N 1```
