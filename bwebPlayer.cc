#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Board
{
private:
  string lastMove;
  vector<string> boardState;
public:
  Board(string initialString);
  Board(Board *board, string newMove);
  ~Board();

  vector<string> getBoardString(){ return boardState;}

  vector<string> getNextMoves(){

  }
};

Board::Board(string initialString){
    std::string delimiter = "[";

  //https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
  vector<string> rows;
  size_t pos = 0;
  string token;
  while ((pos = initialString.find(delimiter)) != string::npos) { //while can still find "["
      token = initialString.substr(0, pos-1); //substr from [ to ], don't include ]
      rows.push_back(token); //append to vector
      initialString.erase(0, pos + delimiter.length());
  }
  //getting last move:
  pos = initialString.find("]");
  token = initialString.substr(0, pos);
  rows.push_back(token);
  initialString.erase(0, pos + 1);
  //strip "LastMove:" from front of last line
  string lastMove = initialString.substr(9, initialString.length());
}
Board::Board(Board *oldBoard, string newMove){
  this->boardState = oldBoard->boardState;
  string parsedMove = newMove.substr(1);
  int pos = parsedMove.find(',');
  int color = stoi(parsedMove.substr(0, pos));
}



int main(int argc, char* argv[])
{
  // print to stderr for debugging purposes
  // remove all debugging statements before submitting your code
  std::cerr << "Given board "  << argv[1] << " thinking...\n" <<  std::flush;
  

  string inpt = argv[1];

  Board *startingBoard = new Board(inpt);
  // parse the input string, i.e., argv[1]
 
  // perform intelligent search to determine the next move

  // print to stdout for AtroposGame
  std::cout << "(1,2,2,2)";
  // As you can see Zeek's algorithm is not very intelligent. He 
  // will be disqualified.

  return 0;
}
