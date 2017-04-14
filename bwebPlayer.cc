#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
  // print to stderr for debugging purposes
  // remove all debugging statements before submitting your code
  //cerr << "Given board "  << argv[1] << " thinking...\n" <<  flush;

  string inpt = argv[1];
  std::string delimiter = "[";

  //https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
  vector<string> rows;
  size_t pos = 0;
  string token;
  while ((pos = inpt.find(delimiter)) != string::npos) { //while can still find "["
      token = inpt.substr(0, pos-1); //substr from [ to ], don't include ]
      rows.push_back(token); //append to vector
      inpt.erase(0, pos + delimiter.length());
  }
  //getting last move:
  pos = inpt.find("]");
  token = inpt.substr(0, pos);
  rows.push_back(token);
  inpt.erase(0, pos + 1);
  //strip "LastMove:" from front of last line
  string lastMove = inpt.substr(9, inpt.length());

  //db: print the vector contents
  //from stackoverflow
  // for (std::vector<string>::const_iterator i = rows.begin(); i != rows.end(); ++i)
  //   cerr << *i << endl;
  // cerr << lastMove << endl;

  //eval func gets lastMove string, whoseMove true/false, boardState string[]

  // parse the input string, i.e., argv[1]

  // perform intelligent search to determine the next move

  // print to stdout for AtroposGame
  std::cout << "(1,2,2,2)";
  // As you can see Zeek's algorithm is not very intelligent. He
  // will be disqualified.

  return 0;
}
