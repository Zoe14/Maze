#ifndef _BOARD_H_
#define _BOARD_H_

#include <string>
#include <vector>
#include <fstream>

// ---------------------------------------------------------------------------
// A simple class to store a coordinate on a 2D integer grid
class Location {
 public:
  // constructors
  Location() : x(-1),y(-1) {}
  Location(int x_,int y_) : x(x_),y(y_) {}
  // representation (ok that it's public for such a small helper class)
  int x,y;
  // comparison routines
  bool operator==(const Location &a) const { return (x == a.x && y == a.y); }
  bool operator!=(const Location &a) const { return (x != a.x || y != a.y); }
};


// ---------------------------------------------------------------------------
// A simple class to store the dimensions of a block
class Block {
 public:
  // constructor
  Block(int width_,int height_,char c_) : width(width_),height(height_),c(c_) {}
  // accessors
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  char getC() const { return c; }
 private:
  // representation
  int width,height;
  char c;
};


// ---------------------------------------------------------------------------
// A basic board class which stores the board/maze as a vector of strings
// You may modify or rewrite or not even use this class at all

class Board {
 public:
  // two different constructors
  Board(const std::vector<std::string> &board_);
  Board(int width_, int height_, const Location &start_, const Location &end_);
  // accessors
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  const Location& getStart() const { return start; }
  const Location& getEnd() const { return end; }
  bool validLocation(const Location &l) const;
  char get(const Location& l) const;
  // modifier
  void set(const Location& l, char value);
  // output the board
  void Print(std::ostream &ostr) const;
 private:
  // representation
  int width;
  int height;
  std::vector<std::string> board;
  Location start;
  Location end;
};
#endif

