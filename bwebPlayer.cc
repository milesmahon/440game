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
  int getBoardSize(){return boardState.size();}
  int* getLastMove(){return lastMove;}

  vector<string> getBoardString(){ return boardState;}

  int getColorAt(int height, int leftDistance, int rightDistance);

  vector<int*> getNextMoves();

  vector<int*> getNearby();
  
  vector<int*> getNearby2();
};

Board::Board(string initialString){
  std::string delimiter = "[";
  vector<string> boardStateCopy; //"Copy" bc will be flipped later in function at *1

  //https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
  size_t pos = 0;
  string token;
  initialString.erase(0, pos + delimiter.length());

  while ((pos = initialString.find(delimiter)) != string::npos) { //while can still find "["
      token = initialString.substr(0, pos-1); //substr from [ to ], don't include ]
      boardStateCopy.push_back(token); // append to list
      //cerr << token << endl;
      initialString.erase(0, pos + delimiter.length());
  }
  //getting last move:
  pos = initialString.find("]");
  token = initialString.substr(0, pos);
  boardStateCopy.push_back(token);
  initialString.erase(0, pos + 1);
  //strip "LastMove:" from front of last line

  //flip the board so index of row = height of row *1
  for (int i = boardStateCopy.size(); i >= 0; i--){
    boardState.push_back(boardStateCopy[i]);
  }

  // db:
  // for (int i = 0; i < boardState.size(); i++){
  //   cerr << boardState[i] << endl;
  // }


  cerr << "before setLastMove" <<endl;
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

//TODO: flipping board so index matches height

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
    if(this->getColorAt(tempMove[1], tempMove[2], tempMove[3]) == 0
      && tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0){
        newMoves->push_back(tempMove);
    }

    // top right
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1] + 1; // height
    tempMove[2] = lastMove[2]; // left distance
    tempMove[3] = lastMove[3] - 1; // right distance
    if(this->getColorAt(tempMove[1], tempMove[2], tempMove[3]) == 0
      && tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0){
        newMoves->push_back(tempMove);
    }

    // left
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1]; // height
    tempMove[2] = lastMove[2] - 1; // left distance
    tempMove[3] = lastMove[3] + 1; // right distance
    if(this->getColorAt(tempMove[1], tempMove[2], tempMove[3]) == 0
      && tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0){
        newMoves->push_back(tempMove);
    }

    // right
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1]; // height
    tempMove[2] = lastMove[2] + 1; // left distance
    tempMove[3] = lastMove[3] - 1; // right distance
    if(this->getColorAt(tempMove[1], tempMove[2], tempMove[3]) == 0
      && tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0){
        newMoves->push_back(tempMove);
    }

    // bottom left
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1] - 1; // height
    tempMove[2] = lastMove[2]; // left distance
    tempMove[3] = lastMove[3] + 1; // right distance
    if(this->getColorAt(tempMove[1], tempMove[2], tempMove[3]) == 0
      && tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0){
        newMoves->push_back(tempMove);
    }

    // bottom right
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1] - 1; // height
    tempMove[2] = lastMove[2] + 1; // left distance
    tempMove[3] = lastMove[3]; // right distance
    if(this->getColorAt(tempMove[1], tempMove[2], tempMove[3]) == 0
      && tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0){
        newMoves->push_back(tempMove);
    }

  }

  return *newMoves;
}

vector<int*> Board::getNearby(){
  vector<int*> *newMoves = new vector<int*>();
  int tempMove[4];
  for(int color = 1; color <= 3; color++){
    // top left
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1] + 1; // height
    tempMove[2] = lastMove[2] - 1; // left distance
    tempMove[3] = lastMove[3]; // right distance
	if (tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0) {
		newMoves->push_back(tempMove);
	}

    // top right
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1] + 1; // height
    tempMove[2] = lastMove[2]; // left distance
    tempMove[3] = lastMove[3] - 1; // right distance
	if (tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0) {
		newMoves->push_back(tempMove);
	}

    // left
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1]; // height
    tempMove[2] = lastMove[2] - 1; // left distance
    tempMove[3] = lastMove[3] + 1; // right distance
	if (tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0) {
		newMoves->push_back(tempMove);
	}

    // right
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1]; // height
    tempMove[2] = lastMove[2] + 1; // left distance
    tempMove[3] = lastMove[3] - 1; // right distance
	if (tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0) {
		newMoves->push_back(tempMove);
	}

    // bottom left
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1] - 1; // height
    tempMove[2] = lastMove[2]; // left distance
	if (tempMove[1] == 1) tempMove[2] -= 1;
    tempMove[3] = lastMove[3] + 1; // right distance
	if (tempMove[1] == 1) tempMove[3] -= 1;
	if (tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0) {
		newMoves->push_back(tempMove);
	}

    // bottom right
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1] - 1; // height
    tempMove[2] = lastMove[2] + 1; // left distance
	if (tempMove[1] == 1) tempMove[2] -= 1;
    tempMove[3] = lastMove[3]; // right distance
	if (tempMove[1] == 1) tempMove[3] -= 1;
	if (tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0) {
		newMoves->push_back(tempMove);
	}

  }

  return *newMoves;
}

vector<int*> Board::getNearby2(){
  vector<int*> *newMoves = new vector<int*>();
  int tempMove1[4];
  int tempMove2[4];
  int tempMove3[4];
  int tempMove4[4];
  int tempMove5[4];
  int tempMove6[4];
  for(int color = 1; color <= 3; color++){
    // top left
    tempMove1[0] = color; // color
    tempMove1[1] = lastMove[1] + 1; // height
    tempMove1[2] = lastMove[2] - 1; // left distance
    tempMove1[3] = lastMove[3]; // right distance
	if (tempMove1[1] > 0 && tempMove1[2] > 0 && tempMove1[3] > 0) {
		newMoves->push_back(tempMove1);
	}

    // top right
    tempMove2[0] = color; // color
    tempMove2[1] = lastMove[1] + 1; // height
    tempMove2[2] = lastMove[2]; // left distance
    tempMove2[3] = lastMove[3] - 1; // right distance
	if (tempMove2[1] > 0 && tempMove2[2] > 0 && tempMove2[3] > 0) {
		newMoves->push_back(tempMove2);
	}

    // left
    tempMove3[0] = color; // color
    tempMove3[1] = lastMove[1]; // height
    tempMove3[2] = lastMove[2] - 1; // left distance
    tempMove3[3] = lastMove[3] + 1; // right distance
	if (tempMove3[1] > 0 && tempMove3[2] > 0 && tempMove3[3] > 0) {
		newMoves->push_back(tempMove3);
	}

    // right
    tempMove4[0] = color; // color
    tempMove4[1] = lastMove[1]; // height
    tempMove4[2] = lastMove[2] + 1; // left distance
    tempMove4[3] = lastMove[3] - 1; // right distance
	if (tempMove4[1] > 0 && tempMove4[2] > 0 && tempMove4[3] > 0) {
		newMoves->push_back(tempMove4);
	}

    // bottom left
    tempMove5[0] = color; // color
    tempMove5[1] = lastMove[1] - 1; // height
    tempMove5[2] = lastMove[2]; // left distance
	if (tempMove5[1] == 1) tempMove5[2] -= 1;
    tempMove5[3] = lastMove[3] + 1; // right distance
	if (tempMove5[1] == 1) tempMove5[3] -= 1;
	if (tempMove5[1] > 0 && tempMove5[2] > 0 && tempMove5[3] > 0) {
		newMoves->push_back(tempMove5);
	}

    // bottom right
    tempMove6[0] = color; // color
    tempMove6[1] = lastMove[1] - 1; // height
    tempMove6[2] = lastMove[2] + 1; // left distance
	if (tempMove6[1] == 1) tempMove6[2] -= 1;
    tempMove6[3] = lastMove[3]; // right distance
	if (tempMove6[1] == 1) tempMove6[3] -= 1;
	if (tempMove6[1] > 0 && tempMove6[2] > 0 && tempMove6[3] > 0) {
		newMoves->push_back(tempMove6);
	}

  }

  return *newMoves;
}

int Board::getColorAt(int height, int leftDistance, int rightDistance){
  string row = this->boardState[height];
  char color = row[leftDistance];
  return color - '0';
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
      this->top = color;
    } else if (left == -1){
      this->left = color;
    } else if (right == -1){
      this->right = color;
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
          //TODO: test w getColor
          tri->add(board.getColorAt(i, j, k)); //pass the color to the trianlge
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


int* minimax(){

}

int eval(Board* board, int* lastMove) {
	int height = board->getBoardString().size();
	int numMoves = 0;
	int color = lastMove[0];
	int mh = lastMove[1];
	int ml = lastMove[2];
	for (int i = 0; i < board->getBoardString().size(); i++) {
		std::cout << "string: " << board->getBoardString()[i] << "\n";
	}
	
	vector<int*> *v = new vector<int*>();
	
	int tempMove[4] = {5, 2, 3, 1};
	v->push_back(tempMove);
	tempMove[0] = 0;
	tempMove[1] = 1;
	tempMove[2] = 2;
	tempMove[3] = 3;
	v->push_back(tempMove);
	
	
	vector<int*> result = board->getNearby2();
	for (int i = 0; i < result.size(); i++) {
		std::cout << "move: [";
		for (int j = 0; j < 4; j++) {
			std::cout << result[i][j] << ", ";
		}
		std::cout << "] \n";
		std::cout << "size: " << sizeof(result[0]) << "\n";
	}
	
	return result.size();
}

void testGiuliano(char* argv[]) {
  string inpt = argv[1];
  if (inpt == "board1") {
	  inpt = "[13][302][1003][31002][100003][3000002][121212]LastPlay:(1,3,1,3)";
  }
  Board *startingBoard = new Board(inpt);

  int move[] = {2, 2, 3, 2};
  std::cout << "eval: " << eval(startingBoard, move) << "\n";
}

int main(int argc, char* argv[])
{
  // print to stderr for debugging purposes
  // remove all debugging statements before submitting your code
  std::cerr << "Given board "  << argv[1] << " thinking...\n" <<  std::flush;


  string inpt = argv[1];
  if (inpt == "board1") {
	  inpt = "[13][302][1003][31002][100003][3000002][121212]LastPlay:(1,3,1,3)";
  }
  Board *startingBoard = new Board(inpt);

  //giuliano testing space
  //testGiuliano(argv);

  /*
  if (isWin(*startingBoard)){
    cerr << "debug: WIN!" << endl;
  }
  */
  // parse the input string, i.e., argv[1]

  // perform intelligent search to determine the next move

  // print to stdout for AtroposGame
  //std::cout << "(1,2,2,2)";
  // As you can see Zeek's algorithm is not very intelligent. He
  // will be disqualified.

  return 0;
}
