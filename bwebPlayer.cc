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
  for (int i = boardStateCopy.size()-1; i >= 0; i--){
    boardState.push_back(boardStateCopy[i]);
  }

  // db:
  // for (int i = 0; i < boardState.size(); i++){
  //   cerr << boardState[i] << endl;
  // }


  //cerr << "before setLastMove" <<endl;
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
  //db:
  operator std::string() const {return (to_string(top) + "," + to_string(left) + "," + to_string(right));};

  void add(int color){
    //cerr << "adding::" + to_string(color) << endl;
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

  bool isValid(){
    //cerr << "colors::" + to_string(this->top) + "," + to_string(this->left) + "," + to_string(this->right) << endl;
    if (this->top != -1 && this->right != -1 && this->left != -1)
      return true;
    return false;

  }

  void clear(){
    this->top = -1;
    this->right = -1;
    this->left = -1;

  }

};


Triangle::Triangle(){
  top = -1;
  right = -1;
  left = -1;
}

Triangle::~Triangle(){
  //no need anymore, since top,left,right are ints, not int*/int[]
    // delete[] top;
    // delete[] right;
    // delete[] left;
}

//ported from AtroposCircle.java
bool isValid(int* move, int size){
  if (move[0] >= size || move[1] >= size || move[2] >= size){
    return false;
  }

  return (move[0] + move[1] + move[2] == size);
}

bool isValid(int i, int j, int k, int size){
  //cerr << "testing::" + to_string(i) + "," + to_string(j) + "," + to_string(k) << endl;
  if (i >= size || j >= size || k >= size){
    //cerr << "invalid::" + to_string(i) + "," + to_string(j) + "," + to_string(k) << endl;
    return false;
  }

  //cerr << "checking ijk add to ::" + to_string(size) + ":" + to_string(i) + "," + to_string(j) + "," + to_string(k) << endl;
  return (i + j + k == size);
}


  //db: print the vector contents
  //from stackoverflow
  // for (std::vector<string>::const_iterator i = rows.begin(); i != rows.end(); ++i)
  //   cerr << *i << endl;
  // cerr << lastMove << endl;

//TODO: working, not well tested
bool isWin(Board board){
  vector<string> boardState = board.getBoardString();
  int *lastMove = board.getLastMove();
  int color = lastMove[0];
  int height = lastMove[1];
  int left = lastMove[2];
  int right = lastMove[3];
  //cerr << left << endl;

  // Hacks
  int i, j, k; // temps
  int c1 = -1;
  int c2 = -1;
  int c3 = -1;
  int c4 = -1;
  int c5 = -1;
  int c6 = -1;

  vector<Triangle> triangles;
  Triangle *tri = new Triangle();

  // top left
  i = height + 1; // height
  j = left - 1; // left distance
  k = right; // right distance
  //int move [] = {i, j, k};
  if (isValid(i, j, k, board.getBoardSize())){
    //cerr << "valid::" + to_string(i) + "," + to_string(j) + "," + to_string(k) << endl;
    c1 = board.getColorAt(i, j, k);
    //cerr << "color::" + to_string(c1) << endl;
  }

  // top right
  i = height + 1; // height
  j = left; // left distance
  k = right-1; // right distance
  //move = {i, j, k};
  if (isValid(i, j, k, board.getBoardSize())){
    //cerr << "valid::" + to_string(i) + "," + to_string(j) + "," + to_string(k) << endl;
    c2 = board.getColorAt(i, j, k);
  }

  tri->add(c1);
  tri->add(c2);
  tri->add(color);
  if (tri->isValid()){
    triangles.push_back(*tri);
  }
  tri->clear();

  // left
  i = height; // height
  j = left - 1; // left distance
  k = right + 1; // right distance
  if (isValid(i, j, k, board.getBoardSize()))
    c3 = board.getColorAt(i, j, k);

  tri->add(c2);
  tri->add(c3);
  tri->add(color);
  if (tri->isValid()){
    triangles.push_back(*tri);
  }
  tri->clear();

  // right
  i = height; // height
  j = left + 1; // left distance
  k = right - 1; // right distance
  //int move [] = {i, j, k};
  if (isValid(i, j, k, board.getBoardSize()))
    c4 = board.getColorAt(i, j, k);

  tri->add(c3);
  tri->add(c4);
  tri->add(color);
  if (tri->isValid()){
    triangles.push_back(*tri);
  }
  tri->clear();

  // bottom left
  i = height - 1; // height
  j = left; // left distance
  k = right + 1; // right distance
  //int move [] = {i, j, k};
  if (isValid(i, j, k, board.getBoardSize()))
    c5 = board.getColorAt(i, j, k);

  tri->add(c4);
  tri->add(c5);
  tri->add(color);
  if (tri->isValid()){
    triangles.push_back(*tri);
  }
  tri->clear();

  // bottom right
  i = height - 1; // height
  j = left + 1; // left distance
  k = right; // right distance
  //int move [] = {i, j, k};
  if (isValid(i, j, k, board.getBoardSize()))
    c6 = board.getColorAt(i, j, k);

  tri->add(c5);
  tri->add(c6);
  tri->add(color);
  if (tri->isValid()){
    triangles.push_back(*tri);
  }
  tri->clear();


  //wrap around!

  tri->add(c1);
  tri->add(c6);
  tri->add(color);
  if (tri->isValid()){
    triangles.push_back(*tri);
  }
  tri->clear();




  // for (int i = height-1; i <= height + 1; i++){
  //   for (int j = left - 1; j <= left + 1; j++){
  //     Triangle *tri = new Triangle();
  //     for (int k = right - 1; k <= right + 1; k++){
  //
  //       int move[3] = {i, j, k};
  //       //cerr << (to_string(i) + "," + to_string(j) + "," + to_string(k)) << endl;
  //       if (isValid(move, board.getBoardSize())){ //need size of board
  //
  //         cerr << ("valid move:: " + to_string(i) + "," + to_string(j) + "," + to_string(k)) << endl;
  //         tri->add(board.getColorAt(i, j, k)); //pass the color to the trianlge
  //       } else {
  //         cerr << ("invalid move:: " + to_string(i) + "," + to_string(j) + "," + to_string(k)) << endl;
  //       }
  //     }
  //
  //     cerr << "triangle::" + string(*tri) << endl;
  //     if (tri->isValid()) //Triangle also has an isValid func
  //       triangles.push_back(*tri);
  //   }
  // }

  for (int x = 0; x < triangles.size(); x++){
    //db
    //cerr << "tri::::" + string(triangles[x]) << endl;
    if (triangles[x].isWin()){
      return true;
    }
  }
  return false;

}


int* minimax(){
  return 0;
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
	std::cout << "size: " << board->getBoardString().size() << "\n";



	return 0;
}

void testGiuliano(char* argv[]) {
  string inpt = argv[1];
  if (inpt == "board1") {
	  inpt = "[13][302][1003][31002][100003][3000002][121212]LastPlay:(1,3,1,3)";
  }
  Board *startingBoard = new Board(inpt);

  int move[] = {2, 2, 3, 2};
  eval(startingBoard, move);
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
  // Triangle *tri = new Triangle();
  // tri->add(1);
  // tri->add(2);
  // tri->add(3);
  //cerr << "tri::::" + string(*tri) << endl;


  // if (isWin(*startingBoard)){
  //   cerr << "debug: WIN!" << endl;
  // } else {
  //
  //   cerr << "debug: not win" << endl;
  // }

  //giuliano testing space
  testGiuliano(argv);




  // parse the input string, i.e., argv[1]

  // perform intelligent search to determine the next move

  // print to stdout for AtroposGame
  //std::cout << "(1,2,2,2)";
  // As you can see Zeek's algorithm is not very intelligent. He
  // will be disqualified.

  return 0;
}
