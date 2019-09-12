# Dancing Links / Sudoku solver
This is an implementation of the Dancing Links algorithm from Donald E. Knuth found [here](https://www-cs-faculty.stanford.edu/~knuth/news.html). There is also an adapter for using Dancing Links for solving sudokus from size NxN. Maybe in the future I could have more adapters to solve other problems N queens.

The implementation is using C++17. I compiled it in a Ubuntu 18.04 with gcc 9.1.

## How to build from Console:
* Create a build folder inside the checkout repository
* From inside build do `cmake ..`
* Call `make`
* The binary is in build/src/s_solver


## How to build from QtCreator:
* Open the project by importing the CMakeLists.txt from the top of the project and build with ctr + b

## How to run
* For it to run you have to pass a file with the sudoku puzzle
* The format is plain text separated by empty spaces for cols and each row is separated by line a break. See examples in the test folder
* The file must be on the same folder where you are calling the binary
* The solver can solve puzzles from a any size as long as it
has the same amount of rows and columns. I have only tested until 9x9 but in theory it could be higher.
* The solution will be output to console
