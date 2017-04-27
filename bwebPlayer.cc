//@author Brandon Webster, Giuliano Conte, Miles Mahon
//bweb@bu.edu, gaconte@bu.edu, mjm382@bu.edu
//4/27/17
//CS440 P3
//
// bwebPlayer.cc an AI player to play the game Atropos
//
// to build: 
//    g++ -o bwebPlayer bwebPlayer.cc
// or if it doesn't default to c++11:
//    g++ -std=c++11 -o bwebPlayer bwebPlayer.cc

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define COLOR_INDEX 0
#define HEIGHT_INDEX 1
#define LDISTANCE_INDEX 2
#define RDISTANCE_INDEX 3

// global for which eval function we use (1-5). Assigned in main function at bottom
int EVAL;

bool isValid(int* move, int size);
bool isValid(int i, int j, int k, int size);
string moveToString(int *move);

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

  vector<int> getNearbyColors(int h, int ld, int rd);

};

//Board class, takes in string given from atroposGame and represents it so it can be easily processed
//by our functions
Board::Board(string initialString){
  std::string delimiter = "[";
  vector<string> boardStateCopy; //"Copy" bc will be flipped later in function, see *1

  //https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
  size_t pos = 0;
  string token;
  initialString.erase(0, pos + delimiter.length());

  while ((pos = initialString.find(delimiter)) != string::npos) { //while can still find "["
      token = initialString.substr(0, pos-1); //substr from [ to ], don't include ]
      boardStateCopy.push_back(token); // append to list
      initialString.erase(0, pos + delimiter.length());
  }

  //getting last move:
  pos = initialString.find("]");
  token = initialString.substr(0, pos);
  boardStateCopy.push_back(token);
  initialString.erase(0, pos + 1);

  //flip the board so index of row = height of row *1
  for (int i = boardStateCopy.size()-1; i >= 0; i--){
    boardState.push_back(boardStateCopy[i]);
  }

  //strip "LastMove:" from front of last line ("LastMove:" = 9 chars hence substr(9))
  this->lastMove = new int[4];
  if(initialString.substr(9) != "null")
		this->setLastMove(initialString.substr(9, initialString.length()));
	else{ // case where "LastMove:null"
		this->lastMove[COLOR_INDEX] = -1;
		this->lastMove[HEIGHT_INDEX] = -1;
		this->lastMove[LDISTANCE_INDEX] = -1;
		this->lastMove[RDISTANCE_INDEX] = -1;
	}

}

//creates a new board which is oldBoard with newMove applied
  // sets the returned board's last move to newMove
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

}

// parse the move string and set lastMove = move
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

// returns all possible moves that you can make, based on the lastMove made on
  // this board. as a vector of moves (int[4])'s'
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
			if (lastMove[1] == 1) tempMove[2] -= 1;
			tempMove[3] = lastMove[3] + 1; // right distance
			if (lastMove[1] == 1) tempMove[3] -= 1;
	    if(isValid(tempMove, getBoardSize()) && this->getColorAt(tempMove[HEIGHT_INDEX], tempMove[LDISTANCE_INDEX], tempMove[RDISTANCE_INDEX]) == 0){
	        newMoves.push_back(tempMove);
	    }

	    // bottom right
	    tempMove = new int[4];
	    tempMove[0] = color; // color
	    tempMove[1] = lastMove[1] - 1; // height
			tempMove[2] = lastMove[2] + 1; // left distance
			if (lastMove[1] == 1) tempMove[2] -= 1;
			tempMove[3] = lastMove[3]; // right distance
			if (lastMove[1] == 1) tempMove[3] -= 1;
	    if(isValid(tempMove, getBoardSize()) && this->getColorAt(tempMove[HEIGHT_INDEX], tempMove[LDISTANCE_INDEX], tempMove[RDISTANCE_INDEX]) == 0){
	        newMoves.push_back(tempMove);
	    }

	  }
	}else{
		srand(time(NULL));

	    int height = rand() % (getBoardSize() - 2) + 1;
	    int ld = rand() % (boardState[height].length() - 2) + 1;
	 	for(int color = 1; color <= 3; color++){
	      int *tempMove = new int[4];
	 		tempMove[0] = color; // color
		    tempMove[1] = height; // height
		    tempMove[2] = ld; // left distance
		    tempMove[3] = this->getBoardSize() - tempMove[1] - tempMove[2];
			newMoves.push_back(tempMove);
		}

	    height = rand() % (getBoardSize() - 2) + 1;
	    ld = rand() % (boardState[height].length() - 2) + 1;
	    for(int color = 1; color <= 3; color++){
	      int *tempMove = new int[4];
	      tempMove[0] = color; // color
	      tempMove[1] = height; // height
	      tempMove[2] = ld; // left distance
	      tempMove[3] = this->getBoardSize() - tempMove[1] - tempMove[2];
	      newMoves.push_back(tempMove);
	    }

	    height = rand() % (getBoardSize() - 2) + 1;
	    ld = rand() % (boardState[height].length() - 2) + 1;
	    for(int color = 1; color <= 3; color++){
	      int *tempMove = new int[4];
	      tempMove[0] = color; // color
	      tempMove[1] = height; // height
	      tempMove[2] = ld; // left distance
	      tempMove[3] = this->getBoardSize() - tempMove[1] - tempMove[2];
	      newMoves.push_back(tempMove);
	    }

	   	height = rand() % (getBoardSize() - 2) + 1;
	    ld = rand() % (boardState[height].length() - 2) + 1;
	    for(int color = 1; color <= 3; color++){
	      int *tempMove = new int[4];
	      tempMove[0] = color; // color
	      tempMove[1] = height; // height
	      tempMove[2] = ld; // left distance
	      tempMove[3] = this->getBoardSize() - tempMove[1] - tempMove[2];
	      newMoves.push_back(tempMove);
	    }
	}


  if(newMoves.size() == 0){
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
	  				newMoves.push_back(tempMove);
  				}
  			}
  		}
  	}
  }

  return newMoves;
}

// returns the colors of all adjacent squares (up to 6 colors)
vector<int> Board::getNearbyColors(int h, int ld, int rd){
    vector<int> colors;

    // top left
  	if(isValid(h + 1, ld - 1, rd, getBoardSize())) // right distance
			colors.push_back(getColorAt(h + 1, ld - 1, rd));

    // top right
  	if(isValid(h + 1, ld, rd - 1, getBoardSize())) // right distance
			colors.push_back(getColorAt(h + 1, ld, rd - 1));

    // right
    if(isValid(h, ld + 1, rd - 1, getBoardSize())) // right distance
			colors.push_back(getColorAt(h, ld + 1, rd - 1));

    // bottom right
    int h2, l2, r2;
    h2 = h - 1; // height
		l2 = ld + 1; // left distance
		if (h == 1) l2 -= 1;
		r2 = rd; // right distance
		if (h == 1) r2 -= 1;
    if(isValid(h2, l2, r2, getBoardSize())) // right distance
			colors.push_back(getColorAt(h2, l2, r2));

		// bottom left
    h2 = h - 1; // height
		l2 = ld; // left distance
		if (h == 1) l2 -= 1;
		r2 = rd + 1; // right distance
		if (h == 1) r2 -= 1;
    if(isValid(h2, l2, r2, getBoardSize())) // right distance
			colors.push_back(getColorAt(h2, l2, r2));

    // left
    if(isValid(h, ld - 1, rd + 1, getBoardSize())) // right distance
			colors.push_back(getColorAt(h, ld - 1, rd + 1));

	return colors;

}

// get color at this board position (notice that rightDistance is not necessary)
int Board::getColorAt(int height, int leftDistance, int rightDistance){
  string row = this->boardState[height];
  char color = row[leftDistance];
  return color - '0';
}


//triangle holds the COLORS of an arbitrary triangle, no coordinates
//This facilitates checking for board end-states
class Triangle
{
public:
  int top;
  int left;
  int right;
  Triangle();
  ~Triangle();
  //db:
  operator std::string() const {return (to_string(top) + "," + to_string(left) + "," + to_string(right));};

  //tries to find a place to add the color to the triangle (top left or right)
  void add(int color){
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
    //checks that colors are different, and not 0 (no color)
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

  // checks if all three colors are initialized to something (not -1)
  bool isValid(){
    if (this->top != -1 && this->right != -1 && this->left != -1)
      return true;
    return false;

  }

};

// default constructor
Triangle::Triangle(){
  top = -1;
  right = -1;
  left = -1;
}

Triangle::~Triangle(){
}

int eval(Board *board);

//ported from AtroposCircle.java -- checks if a board position is on the board
bool isValid(int* move, int size){
  if (move[1] >= size || move[2] >= size || move[3] >= size){
    return false;
  }

	if (move[1] < 0 || move[2] < 0 || move[3] < 0){
    return false;
  }

  return (move[1] + move[2] + move[3] == size);
}

// overloading
bool isValid(int i, int j, int k, int size){
  if (i >= size || j >= size || k >= size){
    return false;
  }

		if (i < 0 || j < 0 || k < 0){
    return false;
  }

  return (i + j + k == size);
}

// checks if board is at an end state
// assumes this will be checked at every move, so only checks
  // if lastMove was a winning/losing move
// Will *not* work if lastMove is entered incorrectly (assumes lastMove not null)
bool isWin(Board board){

  // checks all "triangles" that lastMove is a part of
  // if the triangle contains all 3 colors, it's a "win" (end board state)
    // triangle->add adds a color to a triangle
    // triangle->isWin checks if the three colors it holds are different
  vector<string> boardState = board.getBoardString();
  int* lastMove = board.getLastMove();

  // parse parameters
  int color = lastMove[0];
  int height = lastMove[1];
  int left = lastMove[2];
  int right = lastMove[3];
  //cerr << left << endl;

  // init values
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
  if (isValid(i, j, k, board.getBoardSize())){
    c1 = board.getColorAt(i, j, k);
  }

  // top right
  i = height + 1; // height
  j = left; // left distance
  k = right-1; // right distance
  if (isValid(i, j, k, board.getBoardSize())){
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
    if (triangles[x].isWin()){
      return true;
    }
  }
  return false;

}

// overloading isWin; for use with eval2
bool isWin(vector<int> colors, int lastMoveColor){

  vector<Triangle> triangles;
  Triangle *tri = new Triangle();

  //top left, top right, middle
  tri->add(colors[0]);
  tri->add(colors[1]);
  tri->add(lastMoveColor);
  if (tri->isValid()){
    triangles.push_back(*tri);
  }
  tri = new Triangle();

  //top right, right, middle
  tri->add(colors[1]);
  tri->add(colors[2]);
  tri->add(lastMoveColor);
  if (tri->isValid()){
    triangles.push_back(*tri);
  }
  tri = new Triangle();

  //right, bottom right, middle
  tri->add(colors[2]);
  tri->add(colors[3]);
  tri->add(lastMoveColor);
  if (tri->isValid()){
    triangles.push_back(*tri);
  }
  tri = new Triangle();

  //bottom right, bottom left, middle
  tri->add(colors[3]);
  tri->add(colors[4]);
  tri->add(lastMoveColor);
  if (tri->isValid()){
    triangles.push_back(*tri);
  }
  tri = new Triangle();

  //bottom left, left, middle
  tri->add(colors[4]);
  tri->add(colors[5]);
  tri->add(lastMoveColor);
  if (tri->isValid()){
    triangles.push_back(*tri);
  }
  tri = new Triangle();


  //wrap around!

  //left, top left, middle
  tri->add(colors[5]);
  tri->add(colors[0]);
  tri->add(lastMoveColor);
  if (tri->isValid()){
    triangles.push_back(*tri);
  }

  for (int x = 0; x < triangles.size(); x++){
    if (triangles[x].isWin()){
      return true;
    }
  }
  return false;

}

//wrapper because "isWin" is a misnomer
bool isEndState(Board* board){ 
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

// minimax without AB pruning, deprecated
int minimax(Board *board, bool myTurn, int depth){ //returns maximum score possible to attain from this board

  vector<int*> nextMoves(board->getNextMoves()); //

  //base cases
  if (isWin(*board)){ //if isWin(board) and is myTurn, then my opponent was one who played losing move
    if (myTurn){
      return 1111;
    } else {
      return -1111;
    }
  } else if (depth == 0){
    int ev = eval(board);
    if (myTurn) {
      return ev;
    }
    else {
      return -1*ev;
    }
  }
  else if (nextMoves.size() == 0){
    int ev = eval(board);
    if (myTurn) {
      return ev;
    }
    else {
      return -1*ev;
    }
  }

  //find valid moves
  //evaluate valid moves
  //pick best move
  //call minimax again with that move done on the board

  if (myTurn){ //we are maximizing
    int max = -100000; //NOTE: 0 is worst possible eval score, currently
    for (int i = 0; i < nextMoves.size(); i++){
      Board *childBoard = new Board(board, nextMoves[i]); // applies nextMoves[i] to the board
      int score = minimax(childBoard, false, depth-1); // false because their turn now
      if (score > max){
        max = score;
      }
    }
    return max;
  }
  else { //minimizing
    int min = 100000;
    for (int i = 0; i < nextMoves.size(); i++){
      Board *childBoard = new Board(board, nextMoves[i]); //applies nextMoves[i] to the board
      int score = minimax(childBoard, true, depth-1); // true bc my turn now
      if (score < min){
        min = score;
      }
    }
    return min;
  }
  return -1500000; //big, unique number for debug
}

// minimax with alpha-beta pruning
int minimaxAB(Board *board, bool myTurn, int depth, int A, int B){ //returns maximum score possible to attain from this board

  vector<int*> nextMoves(board->getNextMoves());

  //check win/lose & depth base case
  if (isWin(*board)){ //if isWin(board) and is myTurn, then my opponent was one who played losing move
    if (myTurn){
      return 1111;
    } else {
      return -1111;
    }
  } else if (depth == 0){
    int ev = eval(board);
    if (myTurn) {
      return ev;
    }
    else {
      return -1*ev;
    }
  } // check for tie/sanity check base case
  else if (nextMoves.size() == 0){
    int ev = eval(board);
    if (myTurn) {
      return ev;
    }
    else {
      return -1*ev;
    }
  }

  //find valid moves
  //evaluate valid moves
  //pick best move
  //call minimax again with that move done on the board

  if (myTurn){ //we are maximizing
    int max = -100000; //NOTE: 0 is worst possible eval score, currently
    for (int i = 0; i < nextMoves.size(); i++){
      Board *childBoard = new Board(board, nextMoves[i]); // applies nextMoves[i] to the board
      int score = minimaxAB(childBoard, false, depth-1, A, B); // false bc their turn now
      if (score > max){
        max = score;
      }
			A = std::max(A, max);
			if (B <= A)
				break;
    }
    return max;
  }
  else { //minimizing
    int min = 100000;
    for (int i = 0; i < nextMoves.size(); i++){
      Board *childBoard = new Board(board, nextMoves[i]); //applies nextMoves[i] to the board
      int score = minimaxAB(childBoard, true, depth-1, A, B); // true because my turn now
      if (score < min){
        min = score;
      }
			B = std::min(B, min);
			if (B <= A)
				break;
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

// eval based on number of choices you have
int eval1(Board* board) {
	return board->getNextMoves().size();
}

// eval based on number of non-losing choices you have. slow.
int eval2(Board* board) {
	vector<int*> nextMoves = board->getNextMoves();
	int result = 0;

	for (int i = 0; i < nextMoves.size(); i++) {
		vector<int> colors = board->getNearbyColors(nextMoves[i][HEIGHT_INDEX], nextMoves[i][LDISTANCE_INDEX], nextMoves[i][RDISTANCE_INDEX]);
		int current_color = board->getColorAt(nextMoves[i][HEIGHT_INDEX], nextMoves[i][LDISTANCE_INDEX], nextMoves[i][RDISTANCE_INDEX]);
		if (!isWin(colors, current_color)) {
			result += 1;
		}
	}

	return result;

}
// eval based on color diversity (worse)
int eval3(Board* board) {
	int* lastMove = board->getLastMove();
	vector<int> colors = board->getNearbyColors(lastMove[HEIGHT_INDEX], lastMove[LDISTANCE_INDEX], lastMove[RDISTANCE_INDEX]);
	int colors_included = 0;
	int c1 = 0, c2 = 0, c3 = 0;
	for(int i = 0; i < colors.size(); i++){
		if(colors[i] == 1)
			c1 = 1;
		if(colors[i] == 2)
			c2 = 1;
		if(colors[i] == 3)
			c3 = 1;
	}

	return c1 + c2 + c3;
}


// This is the actual evaluation function used in our player
// eval based on color diversity (better)
int eval4(Board* board) {
	int* lastMove = board->getLastMove();
	vector<int> colors = board->getNearbyColors(lastMove[HEIGHT_INDEX], lastMove[LDISTANCE_INDEX], lastMove[RDISTANCE_INDEX]);
	colors.push_back(lastMove[COLOR_INDEX]);
	int colors_included = 0;
	int c0 = 0, c1 = 0, c2 = 0, c3 = 0;

	for(int i = 0; i < colors.size(); i++){
    if(colors[i] == 0)
      c0++;
		if(colors[i] == 1)
			c1++;
		if(colors[i] == 2)
			c2++;
		if(colors[i] == 3)
			c3++;
	}

  return max(max(c0, c1),  max(c2, c3));
}

// inverse of eval4, prioritizes spots with least color diversity
int eval5(Board* board) {
	int* lastMove = board->getLastMove();
	vector<int> colors = board->getNearbyColors(lastMove[HEIGHT_INDEX], lastMove[LDISTANCE_INDEX], lastMove[RDISTANCE_INDEX]);
	colors.push_back(lastMove[COLOR_INDEX]);
	int colors_included = 0;
	int c0 = 0, c1 = 0, c2 = 0, c3 = 0;

	for(int i = 0; i < colors.size(); i++){
    if(colors[i] == 0)
      c0++;
		if(colors[i] == 1)
			c1++;
		if(colors[i] == 2)
			c2++;
		if(colors[i] == 3)
			c3++;
	}

  return -max(max(c0, c1),  max(c2, c3));
}

int eval(Board* board){
	switch(EVAL){
		case 1:
			return eval1(board);
		case 2:
			return eval2(board);
		case 3:
			return eval3(board);
		case 4:
			return eval4(board);
		case 5:
			return eval5(board);
		default:
			return eval1(board);
	}
}

//copied from our miniMax function.
// since minimax evaluates board positions without returning the board,
// this (notably non-recursive) wrapper chooses the best possible move
// based on the original starting board.
// called from main on board passed thru stdin.
int* chooseMove(Board *board, int depth){
  vector<int*> nextMoves = board->getNextMoves();
  // cerr << "Size of nextMoves is " << nextMoves.size() << endl;
  int max = -100000;
  int* maxMove = new int[4];
  for (int i = 0; i < nextMoves.size(); i++){
    // cerr << moveToString(nextMoves[i]) << endl;
    Board *childBoard = new Board(board, nextMoves[i]); // applies nextMoves[i] to the board

    int score = minimaxAB(childBoard, false, depth-1, -99999999, 99999999); // myTurn = false bc their turn now
    if (score > max){
      max = score;
      maxMove = nextMoves[i];
      for (int j = 0; j < 4; j++){
        maxMove[j] = nextMoves[i][j];
      }
      // cerr << moveToString(maxMove) << endl;
    }
  }
  cerr << "Choosing " + moveToString(maxMove) << "because it has score " << to_string(max) << endl;
  return maxMove;
}

/**
 * We can achieve well under 5 second move times using MinimaxAB, lookahead depth of 7, and eval4
 */
int main(int argc, char* argv[])
{

  //NOTE: LOOKAHEAD DEPTH
  int depth = 7; //for minimax function

  // test boards
  string inpt = argv[1];
  if (inpt == "board1") {
	  inpt = "[13][302][1003][31002][100003][3000002][121212]LastPlay:(1,3,1,3)";
  }
  if (inpt == "board2") {
	  inpt = "[13][302][1003][31002][100003][3000002][10000003][2121212]LastPlay:(1,4,1,3)";
  }
	if (inpt == "board3") {
	  inpt = "[13][302][1203][31102][100003][3000002][121212]LastPlay:(2,4,1,2)";
	}
	if (inpt == "board4") {
	  inpt = "[13][302][1103][32102][133333][21212]LastPlay:(1,3,1,2)";
	}
	if (inpt == "board5") {
	  inpt = "[13][302][1003][30002][100003][3000002][121212]LastPlay:null";
	}
	if (inpt == "board6") {
	  inpt = "[13][302][1003][30002][100003][3000002][10000003][300000002][1000000003][30100000002][100000000003][1212121212]LastPlay:(1,2,2,8)";
	}


  Board *startingBoard = new Board(inpt);

  // move timing
  EVAL = 4;

	/*code to begin timing each move*/
	//begin timing
  //clock_t begin = clock();

  int* move = chooseMove(startingBoard, depth);

	/*code to record the timing of each move*/
  //clock_t end = clock();
  //double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  //cerr << "moveTime for eval" << EVAL << ": " << elapsed_secs << " seconds" << endl;

  // print to stdout for AtroposGame
  cout << "(" << move[0] << "," << move[1] << "," << move[2] << "," << move[3] << ")" << endl;

  return 0;

}
