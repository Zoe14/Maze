#include <cassert>
#include <iostream>
#include "board.h"


// ===========================================================
// BOARD CONSTRUCTOR 1: construct a board from strings
Board::Board(const std::vector<std::string> &board_) {
  // copy the basic representation
  board = board_;

  // determine the dimensions of the board
  height = board.size();
  assert (height > 0);
  width = board[0].size();
  assert (width > 0);
  // confirm that all rows have the same number of columns
  for (int i = 1; i < height; i++) {
    assert (board[i].size() == width);
  }
  
  // Find the start and end tokens
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if (board[y][x] == 'S') {
	// there should be only one start location on the board
	assert (start.x == -1 && start.y == -1);
	start.x = x; start.y = y;
      }
      if (board[y][x] == 'E') {
	// there should be only one end location on the board
	assert (end.x == -1 && end.y == -1);
	end.x = x; end.y = y;
      }
    }
  }

  // confirm that start and end locations were found
  assert (start.x != -1 && start.y != -1);
  assert (end.x != -1 && end.y != -1);
}


// ===========================================================
// BOARD CONSTRUCTOR 2: construct an empty board 
Board::Board(int width_, int height_, const Location &start_, const Location &end_) {
  width = width_;
  height = height_;
  start = start_;
  end = end_;
  assert (width > 0);
  assert (height > 0);
  // create the empty board
  board = std::vector<std::string>(height,std::string(width,' '));
  assert (validLocation(start));
  assert (validLocation(end));
  assert (start != end);
  // label the start and end locations
  board[start.y][start.x] = 'S';
  board[end.y][end.x] = 'E';
}


// ===========================================================

// verify that the location is within the bounds of the board
bool Board::validLocation(const Location &l) const {
  if (l.x < 0 || l.x >= width ||
      l.y < 0 || l.y >= height) return false;
  return true;
}


// read a slot in the board
char Board::get(const Location &l) const {
  assert (validLocation(l));
  return board[l.y][l.x];
}


// set a slot in the board
void Board::set(const Location &l, char value) {
  assert (validLocation(l));
  board[l.y][l.x] = value;
}


// output the board
void Board::Print(std::ostream &ostr) const {
  ostr << std::string(width+2,'#') << std::endl;
  for (int i = 0; i < height; i++) {
    ostr << "#" << board[i] << "#" << std::endl;
  }
  ostr << std::string(width+2,'#') << std::endl;
}

