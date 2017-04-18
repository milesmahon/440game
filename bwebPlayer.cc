#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Board
{
private:

  int *lastMove;
  vector<string> boardState;
public:
  Board(string initialString);
  Board(Board *board, string newMove);
  ~Board();

  void setLastMove(string lastMove);

  int* getLastMove(){return lastMove;}

  vector<string> getBoardString(){ return boardState;}

  vector<string> getNextMoves();
    // This will return a vector containing the next moves possible
    // given the current board state and the last move, including both color and position of these moves
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
  cerr << "before setLastMove" <<endl;
  this->lastMove = new int[4];
  this->setLastMove(initialString.substr(9, initialString.length()));

}
Board::Board(Board *oldBoard, string newMove){
  this->boardState = oldBoard->boardState; // same as (*oldBoard).boardState
  this->lastMove = new int[4];
  this->setLastMove(newMove);
}

Board::~Board(){
  delete[] lastMove;

}

void Board::setLastMove(string move){
  string parsedMove = move.substr(1);
  int pos = parsedMove.find(',');
  lastMove[0] = stoi(parsedMove.substr(0, pos)); // color
  parsedMove = parsedMove.substr(pos + 1);
  pos = parsedMove.find(',');
  lastMove[1] = stoi(parsedMove.substr(0, pos)); // height
  parsedMove = parsedMove.substr(pos + 1);
  pos = parsedMove.find(',');
  lastMove[2] = stoi(parsedMove.substr(0, pos));  // left Distance
  parsedMove = parsedMove.substr(pos + 1);
  pos = parsedMove.find(')');
  lastMove[3] = stoi(parsedMove.substr(0, pos));  // rightDistance
  parsedMove = parsedMove.substr(pos + 1);
}

vector<string> Board::getNextMoves(){
}

//triangle holds the COLORS of an arbitrary triangle, no coordinates
class Triangle
{
public:
  int top;
  int left;
  int right;
  // Triangle(int top[4], int left[4], int right[4]);
  Triangle();
  ~Triangle();

  void add(int color){
    if (top == -1){
      this->top = move;
    } else if (left == -1){
      this->left = move;
    } else if (right == -1){
      this->right = move;
    } else {
      cout << "ERROR -- triangle full -mm" << endl;
    }
    return;
  }

  bool isWin(){
    int c1 = this->top;
    int c2 = this->left;
    int c3 = this->right;

    if (c1 != c2 && c2 != c3 && c3 != c1){
      return true;
    }
    return false;
  }
};


Triangle::Triangle(){
  top = -1;
  right = -1;
  left = -1;
}

Triangle::~Triangle(){
    // delete[] top;
    // delete[] right;
    // delete[] left;
}

//ported from AtroposCircle.java
bool isValid(int* move, int size){
  if (move[0] >= size || move[1] >= size || move[2] >= size){
    return false;
  }

  return (move[0] + move[1] + move[2] >= size);
}

  //db: print the vector contents
  //from stackoverflow
  // for (std::vector<string>::const_iterator i = rows.begin(); i != rows.end(); ++i)
  //   cerr << *i << endl;
  // cerr << lastMove << endl;
bool isWin(Board board){
  vector<string> boardState = board.getBoardString();
  int *lastMove = board.getLastMove();
  int height = lastMove[1];
  int left = lastMove[2];
  int right = lastMove[3];
  vector<Triangle> triangles;
  for (int i = left-1; i <= left + 1; i++){
    for (int j = right - 1; j <= right + 1; j++){
      Triangle *tri = new Triangle();
      for (int k = height - 1; k <= height + 1; k++){
        int move[3] = {i, j, k};
        if (isValid(move, board.getBoardString().size())){ //need size of board
          tri->getColor(i, j, k); //pass the color to the trianlge
        }
      }
      triangles.push_back(*tri);
    }
  }

  for (int x = 0; x < triangles.size(); x++){
    if (triangles[x].isWin()){
      return true;
    }
  }
  return false;

}


int main(int argc, char* argv[])
{
  // print to stderr for debugging purposes
  // remove all debugging statements before submitting your code
  std::cerr << "Given board "  << argv[1] << " thinking...\n" <<  std::flush;


  string inpt = argv[1];

  Board *startingBoard = new Board(inpt);

  isWin(*startingBoard);
  // parse the input string, i.e., argv[1]

  // perform intelligent search to determine the next move

  // print to stdout for AtroposGame
  std::cout << "(1,2,2,2)";
  // As you can see Zeek's algorithm is not very intelligent. He
  // will be disqualified.

  return 0;
}
