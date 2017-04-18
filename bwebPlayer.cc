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
  Board(Board *board, int* newMove);
  ~Board();

  void setLastMove(string lastMove);

  int* getLastMove(){return lastMove;}

  vector<string> getBoardString(){ return boardState;}

  vector<int*> getNextMoves();
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
  this->lastMove = new int[4];
  this->setLastMove(initialString.substr(9, initialString.length()));

}
Board::Board(Board *oldBoard, int* newMove){
  this->boardState = oldBoard->boardState; // same as (*oldBoard).boardState
  this->lastMove = newMove;
  // this->setLastMove(newMove);
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

vector<int*> Board::getNextMoves(){
  vector<int*> *newMoves = new vector<int*>();
  int tempMove[4];
  for(int color = 1; color <= 3; color++){
    // top left
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1] + 1; // height
    tempMove[2] = lastMove[2] - 1; // left distance
    tempMove[3] = lastMove[3]; // right distance
    newMoves->push_back(tempMove);

    // top right
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1] + 1; // height
    tempMove[2] = lastMove[2]; // left distance
    tempMove[3] = lastMove[3] - 1; // right distance
    newMoves->push_back(tempMove);

    // left
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1]; // height
    tempMove[2] = lastMove[2] - 1; // left distance
    tempMove[3] = lastMove[3] + 1; // right distance
    newMoves->push_back(tempMove);

    // right
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1]; // height
    tempMove[2] = lastMove[2] + 1; // left distance
    tempMove[3] = lastMove[3] - 1; // right distance
    newMoves->push_back(tempMove);

    // bottom left
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1] - 1; // height
    tempMove[2] = lastMove[2]; // left distance
    tempMove[3] = lastMove[3] + 1; // right distance
    newMoves->push_back(tempMove);

    // bottom right
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1] - 1; // height
    tempMove[2] = lastMove[2] + 1; // left distance
    tempMove[3] = lastMove[3]; // right distance
    newMoves->push_back(tempMove);

  }   

  return *newMoves; 
}

class Triangle
{
public:
  int *top;
  int *left;
  int *right;
  // Triangle(int top[4], int left[4], int right[4]);
  Triangle();
  ~Triangle();

  void add(int move[4]){
    if (top == nullptr){
      this->top = move;
    } else if (left == nullptr){
      this->left = move;
    } else if (right == nullptr){
      this->right = move;
    } else {
      cout << "ERROR -- triangle full -mm" << endl;
    }
    return;
  }

  bool isWin(){
    int c1 = top[0];
    int c2 = left[0];
    int c3 = right[0];

    if (c1 != c2 && c2 != c3 && c3 != c1){
      return true;
    }
    return false;

  }
};


Triangle::Triangle(){

}

Triangle::~Triangle(){
  delete[] top;
  delete[] right;
  delete[] left;
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
        int move[4] = {0, i, j, k};
        tri->add(move);
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


int eval(Board* board, int* lastMove) {
	int height = board->getBoardString().size();
	int numMoves = 0;
	int color = lastMove[0];
	int mh = lastMove[1];
	int ml = lastMove[2];
	std::cout << "color: " << color << "\n";
	return 0;
}

int main(int argc, char* argv[])
{
  // print to stderr for debugging purposes
  // remove all debugging statements before submitting your code
  std::cerr << "Given board "  << argv[1] << " thinking...\n" <<  std::flush;


  string inpt = argv[1];

  Board *startingBoard = new Board(inpt);
  
  int move[] = {2, 2, 3, 2};
  //eval(startingBoard, move);
  
  // parse the input string, i.e., argv[1]

  // perform intelligent search to determine the next move

  // print to stdout for AtroposGame
  //std::cout << "(1,2,2,2)";
  // As you can see Zeek's algorithm is not very intelligent. He
  // will be disqualified.

  return 0;
}
