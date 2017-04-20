#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define COLOR_INDEX 0
#define HEIGHT_INDEX 1
#define LDISTANCE_INDEX 2
#define RDISTANCE_INDEX 3

bool isValid(int* move, int size);
bool isValid(int i, int j, int k, int size);
string moveToString(int *move);

class Board
{
private:

  int *lastMove; //int*???????TODO or int *lastMove??????
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
  for (int i = boardStateCopy.size()-1; i >= 0; i--){
    boardState.push_back(boardStateCopy[i]);
  }

  // db:
  // for (int i = 0; i < boardState.size(); i++){
  //   cerr << boardState[i] << endl;
  // }


  //cerr << "before setLastMove" <<endl;
  this->lastMove = new int[4];
  if(initialString.substr(9) != "null")
		this->setLastMove(initialString.substr(9, initialString.length()));
	else{
		lastMove[COLOR_INDEX] = -1;
		lastMove[HEIGHT_INDEX] = -1;
		lastMove[LDISTANCE_INDEX] = -1;
		lastMove[RDISTANCE_INDEX] = -1;
	}

}
Board::Board(Board *oldBoard, int* newMove){
  this->boardState = oldBoard->boardState; // same as (*oldBoard).boardState

  this->lastMove = new int[4];
  this->lastMove[0] = newMove[0];
  this->lastMove[1] = newMove[1];
  this->lastMove[2] = newMove[2];
  this->lastMove[3] = newMove[3];
  //this->lastMove = newMove;

  string row = this->boardState[newMove[HEIGHT_INDEX]];
  row.replace(newMove[LDISTANCE_INDEX], 1, to_string(newMove[COLOR_INDEX]));
  this->boardState[newMove[HEIGHT_INDEX]] = row;
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
  vector<int*> newMoves;

  if(lastMove[0] > -1){
	  for(int color = 1; color <= 3; color++){
	    // top left
	    int *tempMove = new int[4];
	    tempMove[0] = color; // color
	    tempMove[1] = lastMove[1] + 1; // height
	    tempMove[2] = lastMove[2] - 1; // left distance
	    tempMove[3] = lastMove[3]; // right distance

	    if(isValid(tempMove, getBoardSize())
				&& this->getColorAt(tempMove[HEIGHT_INDEX], tempMove[LDISTANCE_INDEX], tempMove[RDISTANCE_INDEX]) == 0){
	        newMoves.push_back(tempMove);
	    }

	    // top right
	    tempMove = new int[4];
	    tempMove[0] = color; // color
	    tempMove[1] = lastMove[1] + 1; // height
	    tempMove[2] = lastMove[2]; // left distance
	    tempMove[3] = lastMove[3] - 1; // right distance
	    if(isValid(tempMove, getBoardSize()) && this->getColorAt(tempMove[HEIGHT_INDEX], tempMove[LDISTANCE_INDEX], tempMove[RDISTANCE_INDEX]) == 0){
	        newMoves.push_back(tempMove);
	    }

	    // left
	    tempMove = new int[4];
	    tempMove[0] = color; // color
	    tempMove[1] = lastMove[1]; // height
	    tempMove[2] = lastMove[2] - 1; // left distance
	    tempMove[3] = lastMove[3] + 1; // right distance
	    if(isValid(tempMove, getBoardSize()) && this->getColorAt(tempMove[HEIGHT_INDEX], tempMove[LDISTANCE_INDEX], tempMove[RDISTANCE_INDEX]) == 0){
	        newMoves.push_back(tempMove);
	    }

	    // right
	    tempMove = new int[4];
	    tempMove[0] = color; // color
	    tempMove[1] = lastMove[1]; // height
	    tempMove[2] = lastMove[2] + 1; // left distance
	    tempMove[3] = lastMove[3] - 1; // right distance
	    if(isValid(tempMove, getBoardSize()) && this->getColorAt(tempMove[HEIGHT_INDEX], tempMove[LDISTANCE_INDEX], tempMove[RDISTANCE_INDEX]) == 0){
	        newMoves.push_back(tempMove);
	    }

	    // bottom left
	    tempMove = new int[4];
	    tempMove[0] = color; // color
	    tempMove[1] = lastMove[1] - 1; // height
			tempMove[2] = lastMove[2]; // left distance
			if (tempMove[1] == 1) tempMove[2] -= 1;
			tempMove[3] = lastMove[3] + 1; // right distance
			if (tempMove[1] == 1) tempMove[3] -= 1;
	    if(isValid(tempMove, getBoardSize()) && this->getColorAt(tempMove[HEIGHT_INDEX], tempMove[LDISTANCE_INDEX], tempMove[RDISTANCE_INDEX]) == 0){
	        newMoves.push_back(tempMove);
	    }

	    // bottom right
	    tempMove = new int[4];
	    tempMove[0] = color; // color
	    tempMove[1] = lastMove[1] - 1; // height
			tempMove[2] = lastMove[2] + 1; // left distance
			if (tempMove[1] == 1) tempMove[2] -= 1;
			tempMove[3] = lastMove[3]; // right distance
			if (tempMove[1] == 1) tempMove[3] -= 1;
	    if(isValid(tempMove, getBoardSize()) && this->getColorAt(tempMove[HEIGHT_INDEX], tempMove[LDISTANCE_INDEX], tempMove[RDISTANCE_INDEX]) == 0){
	        newMoves.push_back(tempMove);
	    }

	  }
	}
  if(newMoves.size() == 0){
  	// cerr << "newMoves size was zero" << endl;
  	// for loop over rows
  	for(int height = 1; height < this->getBoardSize(); height++){
  		string row = this->boardState[height];
  		for(int leftDistance = 1; leftDistance < row.length() - 1; leftDistance++){
  			if(row[leftDistance] == '0'){
  				for(int color = 1; color <= 3; color++){
	  				int *tempMove = new int[4];
	  				tempMove[COLOR_INDEX] = color;
	  				tempMove[HEIGHT_INDEX] = height;
	  				tempMove[LDISTANCE_INDEX] = leftDistance;
	  				tempMove[RDISTANCE_INDEX] = this->getBoardSize() - height - leftDistance;
	  				// cerr << "In get next moves (all board): move: " << moveToString(tempMove) << endl;
	  				newMoves.push_back(tempMove);
  				}
  			}
  		}
  	}
  }

  return newMoves;
}

vector<int*> Board::getNearby(){
  vector<int*> newMoves;

  for(int color = 1; color <= 3; color++){
    // top left
  	int *tempMove = new int[4];
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1] + 1; // height
    tempMove[2] = lastMove[2] - 1; // left distance
    tempMove[3] = lastMove[3]; // right distance
	if (tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0) {
		newMoves.push_back(tempMove);
	}


    // top right
	tempMove = new int[4];
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1] + 1; // height
    tempMove[2] = lastMove[2]; // left distance
    tempMove[3] = lastMove[3] - 1; // right distance
	if (tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0) {
		newMoves.push_back(tempMove);
	}

    // left
	tempMove = new int[4];
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1]; // height
    tempMove[2] = lastMove[2] - 1; // left distance
    tempMove[3] = lastMove[3] + 1; // right distance
	if (tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0) {
		newMoves.push_back(tempMove);
	}

    // right
	tempMove = new int[4];
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1]; // height
    tempMove[2] = lastMove[2] + 1; // left distance
    tempMove[3] = lastMove[3] - 1; // right distance
	if (tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0) {
		newMoves.push_back(tempMove);
	}

    // bottom left
	tempMove = new int[4];
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1] - 1; // height
	tempMove[2] = lastMove[2]; // left distance
	if (tempMove[1] == 1) tempMove[2] -= 1;
		tempMove[3] = lastMove[3] + 1; // right distance
	if (tempMove[1] == 1) tempMove[3] -= 1;
	if (tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0) {
		newMoves.push_back(tempMove);
	}

    // bottom right
	tempMove = new int[4];
    tempMove[0] = color; // color
    tempMove[1] = lastMove[1] - 1; // height
    tempMove[2] = lastMove[2] + 1; // left distance
	if (tempMove[1] == 1) tempMove[2] -= 1;
		tempMove[3] = lastMove[3]; // right distance
	if (tempMove[1] == 1) tempMove[3] -= 1;
	if (tempMove[1] > 0 && tempMove[2] > 0 && tempMove[3] > 0) {
		newMoves.push_back(tempMove);
	}

  }

  return newMoves;
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
      cerr << "ERROR -- triangle full -mm" << endl;
    }
    return;
  }

  //an in-class isWin function for triangles,
  //since triangle has immediate access to its own colors
  bool isWin(){
    int c1 = this->top;
    int c2 = this->left;
    int c3 = this->right;

    if (c1 != c2 && c2 != c3 && c3 != c1){
      if (c1 != 0 && c2 != 0 && c3 != 0){
        return true;
      }
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

int eval(Board *board);
int eval2(Board *board);

//ported from AtroposCircle.java
bool isValid(int* move, int size){
  if (move[1] >= size || move[2] >= size || move[3] >= size){
    return false;
  }

	if (move[1] < 0 || move[2] < 0 || move[3] < 0){
    return false;
  }

  return (move[1] + move[2] + move[3] == size);
}

bool isValid(int i, int j, int k, int size){
  //cerr << "testing::" + to_string(i) + "," + to_string(j) + "," + to_string(k) << endl;
  if (i >= size || j >= size || k >= size){
    //cerr << "invalid::" + to_string(i) + "," + to_string(j) + "," + to_string(k) << endl;
    return false;
  }

		if (i < 0 || j < 0 || k < 0){
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



//TODO: why do losing moves have score 0 or 20 sometimes



//TODO: working, not well tested
// Will *not* work if lastMove is entered incorrectly.
bool isWin(Board board){
  //printBoard(board);
  vector<string> boardState = board.getBoardString();
  int* lastMove = board.getLastMove();

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
  tri = new Triangle();

  // right
  i = height; // height
  j = left + 1; // left distance
  k = right - 1; // right distance
  //int move [] = {i, j, k};
  if (isValid(i, j, k, board.getBoardSize()))
    c3 = board.getColorAt(i, j, k);

  tri->add(c2);
  tri->add(c3);
  tri->add(color);
  if (tri->isValid()){
    triangles.push_back(*tri);
  }
  tri = new Triangle();


  // bottom right
  i = height - 1; // height
  j = left + 1; // left distance
  k = right; // right distance
  //int move [] = {i, j, k};
  if (isValid(i, j, k, board.getBoardSize()))
    c4 = board.getColorAt(i, j, k);

  tri->add(c3);
  tri->add(c4);
  tri->add(color);
  if (tri->isValid()){
    triangles.push_back(*tri);
  }
  tri = new Triangle();

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
  tri = new Triangle();

  // left
  i = height; // height
  j = left - 1; // left distance
  k = right + 1; // right distance
  if (isValid(i, j, k, board.getBoardSize()))
    c6 = board.getColorAt(i, j, k);

  tri->add(c5);
  tri->add(c6);
  tri->add(color);
  if (tri->isValid()){
    triangles.push_back(*tri);
  }
  tri = new Triangle();


  //wrap around!

  tri->add(c1);
  tri->add(c6);
  tri->add(color);
  if (tri->isValid()){
    triangles.push_back(*tri);
  }
  tri = new Triangle();

  for (int x = 0; x < triangles.size(); x++){
    //db
    //cerr << "tri::::" + string(triangles[x]) << endl;
    if (triangles[x].isWin()){
      return true;
    }
  }
  return false;

}


bool isEndState(Board* board){ //wrapper because "isWin" is a misnomer
  return isWin(*board);
}

/*
 * outputs a board to stdout with formatting
 */
void printBoard(Board* board) {
	int height = board->getBoardString().size();
	vector<string> boardString = board->getBoardString();
	for (int i = 0; i < height; i++) {
		int layer = height - i;
		for (int j = 0; j < abs((height-1) - (i+1)); j++) {
			cerr << " ";
		}
		for (int j = 0; j < boardString[layer-1].length(); j++) {
			cerr << boardString[layer-1][j] << " ";
		}
		cerr << "\n";
	}
}

int minimax(Board *board, bool myTurn, int depth){ //returns maximum score possible to attain from this board

  //vector<int*> nextMoves = new vector<int*>(board->getNextMoves());
  vector<int*> nextMoves(board->getNextMoves()); //= new vector(board->getNextMoves());

  //check depth base case
  //TODO: check isWin here?
  //How does eval handle winning
  //printBoard(board);
  if (isWin(*board)){ //if isWin(board) and is myTurn, then my opponent was one who played losing move
    if (myTurn){
      return 1111; //TODO: is this best score?
    } else {
      return -1111; //TODO: is this best score?
    }
  } else if (depth == 0){
    return eval(board);
  }
  else if (nextMoves.size() == 0){
    return eval(board);
  }

  //find valid moves
  //evaluate valid moves
  //pick best move
  //call minimax again with that move done on the board
  //printBoard(board);
  //vector<int*> nextMoves = board->getNextMoves();

  if (myTurn){ //we are maximizing
    int max = -100000; //NOTE: 0 is worst possible eval score, currently
    //int* maxMove;
    for (int i = 0; i < nextMoves.size(); i++){
      Board *childBoard = new Board(board, nextMoves[i]); // applies nextMoves[i] to the board
      int score = minimax(childBoard, false, depth-1); // false bc their turn now
      if (score > max){
        max = score;
        //maxMove = nextMoves[i];
      }
    }
    return max;
  }
  else { //minimizing
    int min = 100000;
    //int* minMove;
    for (int i = 0; i < nextMoves.size(); i++){
      Board *childBoard = new Board(board, nextMoves[i]); //applies nextMoves[i] to the board
      int score = minimax(childBoard, true, depth-1); // true bc my turn now
      if (score < min){
        min = score;
        //minMove = nextMoves[i];
      }
    }
    return min;
  }
  return -1500000;
}

string moveToString(int *move) {
	string result = "[";
	for (int i = 0; i < 4; i++) {
		result += to_string(move[i]);
		result += ", ";
	}
	result += "]";
	return result;
}

int eval(Board* board) {
	vector<int*> nextMoves = board->getNextMoves();
	int result = 0;

	for (int i = 0; i < nextMoves.size(); i++) {
		if(nextMoves[i][COLOR_INDEX] > 3)
			std::cerr << "move: " << moveToString(nextMoves[i]) << endl;
		result += 1;
	}

	return result;

}

int eval2(Board* board) {
	vector<int*> nextMoves = board->getNextMoves();
	int result = 0;

	for (int i = 0; i < nextMoves.size(); i++) {
		Board *childBoard = new Board(board, nextMoves[i]);
		std::cerr << "move: " << moveToString(nextMoves[i]) << endl;
		if (isEndState(childBoard)) {
			//std::cerr << "move: " << moveToString(nextMoves[i]) << endl;
			std:cerr << "results in loss" << endl;
			result += 1;
		}
		std::cerr << "\n";
	}

	return result;

}



void testGiuliano(Board* board) {
	std::cerr << "lastMove: " << moveToString(board->getLastMove()) << endl;
	//std::cerr << "--------eval1---------" << "\n";
	//int e1 = eval(board);
	//std::cerr << "eval1: " << e1 << "\n\n";
	std::cerr << "--------eval2---------" << "\n";
	int e2 = eval2(board);
  std::cerr << "eval2: " << e2 << "\n\n";
	if (isWin(*board)) {
		std::cerr << "is a win!" << endl;
	}
	printBoard(board);
}

//copied from miniMax function.
// since minimax evaluates board positions without returning the board,
// this (notably non-recursive) wrapper chooses the best possible move
// based on the original starting board.
// called from main on board passed thru stdin.
int* chooseMove(Board *board, int depth){
  vector<int*> nextMoves = board->getNextMoves();

  cerr << "Size of nextMoves is " << nextMoves.size() << endl;

  int max = -100000;
  int* maxMove = new int[4];
  for (int i = 0; i < nextMoves.size(); i++){
    cerr << moveToString(nextMoves[i]) << endl;
    Board *childBoard = new Board(board, nextMoves[i]); // applies nextMoves[i] to the board
    int score = minimax(childBoard, false, depth-1); // false bc their turn now
    if (score > max){
      max = score;
      maxMove = nextMoves[i];
      if(nextMoves[i][COLOR_INDEX] > 3)
      	cerr << "Found a next move with color greater than three: " << moveToString(nextMoves[i]) << endl;
      for (int j = 0; j < 4; j++){
        maxMove[j] = nextMoves[i][j];
      }
      // cerr << moveToString(maxMove) << endl;
    }
  }
  cerr << "Choosing " + moveToString(maxMove) << "because it has score " << to_string(max) << endl;
  return maxMove;
}


//TODO: fix bug where script dies if playing first (bc lastPlay = "null")
//TODO: fix bug where script chooses losing move when it doesn't have to.
  //although I'm not sure that's actually what happened?
//Script sometimes tries illegal moves

int main(int argc, char* argv[])
{

  int depth = 4; //for minimax function
  // print to stderr for debugging purposes
  // remove all debugging statements before submitting your code
  std::cerr << "Given board "  << argv[1] << " thinking...\n" <<  std::flush;

  string inpt = argv[1];
  if (inpt == "board1") {
	  inpt = "[13][302][1003][31002][100003][3000002][121212]LastPlay:(1,3,1,3)";
  }
  if (inpt == "board2") {
	  inpt = "[13][302][1003][31002][100003][3000002][10000003][2121212]LastPlay:(1,4,1,3)";
  }
	if (inpt == "board3") {
	  inpt = "[13][302][1203][31102][100003][3000002][121212]LastPlay:(1,4,1,2)";
	}
	if (inpt == "board4") {
	  inpt = "[13][302][1103][32102][133333][21212]LastPlay:(1,3,1,2)";
	}
	if (inpt == "board5") {
	  inpt = "[13][302][1003][30002][100003][3000002][121212]LastPlay:null";
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
   //testGiuliano(startingBoard);


  /*
  if (isWin(*startingBoard)){
    cerr << "debug: WIN!" << endl;
  }
  */

  // parse the input string, i.e., argv[1]

  // perform intelligent search to determine the next move

  int* move = chooseMove(startingBoard, depth);

  // print to stdout for AtroposGame
  //cerr << "hey" + moveToString(move) << endl;
  cout << "(" << move[0] << "," << move[1] << "," << move[2] << "," << move[3] << ")" << endl;
  // As you can see Zeek's algorithm is not very intelligent. He
  // will be disqualified.



  return 0;

}
