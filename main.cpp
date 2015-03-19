#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <list>
#include "board.h"
using namespace std;

// ----------------------------------------------------------------------------------------
// Prototypes of the functions you need to write (you may change the
// interface as necessary to implement your algorithm)

// find the shortest path through a maze board
int ShortestPath(const Board &board);
//helper function
int path(int x, int y, int num,const Location& end,Board board);

// find all ways to pack the specified blocks into the board such that
// the shortest path is equal to desired path length
void FindMazes( const Board &board, int desired_path_length, const std::vector<Block> &blocks, std::ostream &ostr);
//helper function
void PutBlock(const vector <Block>& blocks, int num,vector <Board>& can, Board& board); // put blocks into the board
bool check(int x,int y, int w,int h, Board &board); // the check if the block can be put in (w,h) postion of the board
void Unique(const vector <Board> &solutions,vector <Board> & unique); // find the unique solution(s)

// find one solution
void FindOneMaze(const Board &board, int desired_path_length, const std::vector<Block> &blocks, std::ostream &ostr);
//helper funcion
void FindOneMaze(Board & scratch, int n, vector <Block> sorted,int num, bool &find);
// ----------------------------------------------------------------------------------------
// Create a number of test boards with known answers to verify that
// Part 1 (Shortest Path) is working
void TestPart1() {

  // These are the examples from the homework handout

  std::vector<std::string> test1;
  test1.push_back("E   S ");
  test1.push_back(" ***  ");
  test1.push_back("      ");
  int test1_path_length = ShortestPath(Board(test1));
  assert (test1_path_length == 4);

  std::vector<std::string> test2;
  test2.push_back(" S*E ");
  test2.push_back(" *** ");
  test2.push_back("     ");
  int test2_path_length = ShortestPath(Board(test2));
  assert (test2_path_length == 10);
  
  std::vector<std::string> test3;
  test3.push_back("E***  ");
  test3.push_back(" ***S*");
  test3.push_back("     *");
  int test3_path_length = ShortestPath(Board(test3));
  assert (test3_path_length == 7);
  
  std::vector<std::string> test4;
  test4.push_back(" * E ");
  test4.push_back("  ** ");
  test4.push_back("S   *");
  int test4_path_length = ShortestPath(Board(test4));
  assert (test4_path_length == -1);

  //
  // Add your own test cases too!
  //
  vector<string> test5;
  test5.push_back("S* *   ");
  test5.push_back(" *   * ");
  test5.push_back(" * *** ");
  test5.push_back("   *   ");
  test5.push_back(" *** * ");
  test5.push_back("      E");
  int test5_path_length= ShortestPath(Board(test5));
  assert (test5_path_length ==11);
  
  vector<string> test6;
  test6.push_back("     *E   ");
  test6.push_back(" ***S* ** ");
  test6.push_back("  *  *  * ");
  test6.push_back("  * *     ");
  test6.push_back("  * * *** ");
  test6.push_back("*     *   ");
  test6.push_back("  ***   * ");
  int test6_path_length=ShortestPath(Board(test6)) ;
  assert(test6_path_length==13);

}

bool larger_than_size(const Block & b1, const Block & b2)
{
	return (b1.getWidth()*b1.getHeight())>(b2.getWidth()*b2.getHeight());
}
// ----------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {

  // ---------------------------------------
  // If there are no command line arguments, just test Part 1
  if (argc == 1) {
    TestPart1();
    return 0;
  }

  // ---------------------------------------
  // Command line arguments for Part 2
  if (argc != 4) {
    std::cerr << "ERROR: incorrect number of arguments" << std::endl;
    std::cerr << "usage:  " << argv[0] 
	      << " <puzzle_file> <output_file> <all_solutions/one_solution>" << std::endl;
    exit(1);
  }
  
  std::ifstream istr(argv[1]);
  if (!istr) {
    std::cerr << "ERROR: cannot read input file " << argv[1] << std::endl;
    exit(1);
  }

  std::ofstream ostr(argv[2]);
  if (!ostr) {
    std::cerr << "ERROR: cannot write output file " << argv[2] << std::endl;
    exit(1);
  }

  if (std::string(argv[3]) != std::string("all_solutions") &&
      std::string(argv[3]) != std::string("one_solution")) {
    std::cerr << "usage:  " << argv[0] 
	      << " <puzzle_file> <output_file> <all_solutions/one_solution>" << std::endl;    
    exit(1);
  }

  // ---------------------------------------
  // Read in the requirements for the maze
  std::string token;
  istr >> token;
  assert (token == "board");
  int width, height;
  istr >> width >> height;
  Location start,end;
  istr >> token;
  assert (token == "start");
  istr >> start.x >> start.y;
  istr >> token;
  assert (token == "end");
  istr >> end.x >> end.y;
  istr >> token;
  assert (token == "desired_path_length");
  int desired_path_length;
  istr >> desired_path_length;
  std::vector<Block> blocks;
  while (istr >> token) {
    if (token == "block") {
      int w,h;
      char c;
      istr >> w >> h >> c;
      blocks.push_back(Block(w,h,c));
    }
  }

  // ---------------------------------------
  // Create an empty maze board
  Board board(width,height,start,end);
  
  if (std::string(argv[3]) == std::string("all_solutions")) {
    // find and output all solutions
    FindMazes(board,desired_path_length,blocks,ostr);
  } else {
    // find and output any one solution
    FindOneMaze(board,desired_path_length,blocks,ostr);
  }
}

int ShortestPath( const Board &board)
{
	Location s (board.getStart().x,board.getStart().y);
	Location e (board.getEnd().x, board.getEnd().y);
	int num=0; // number of step
	num=path(s.x,s.y,num,e,board);
	if(num==board.getHeight()*board.getWidth())
		return -1;
	else
		return num;
}
int path(int x, int y, int num,const Location &end , Board board)
 {
	if(x==end.x && y==end.y) // when it reach the end
		return num ;
	else
	{
		Location temp(x,y);
		if(!(board.validLocation(temp))||(board.get(temp)!=' '&& board.get(temp)!='S'&& board.get(temp)!='E')) // hits the edge or hit the block
			 return board.getHeight()*board.getWidth();  // return a really large, so it won't choose this path
		else
		{
			board.set(temp,'*');
			int a,b,c,d;
			//recurse four directions
			a=path(x,y-1,num+1,end,board);
			b=path(x,y+1,num+1,end,board);
			c=path(x-1,y,num+1,end,board);
			d=path(x+1,y,num+1,end,board);
			num=min(min(a,b),min(c,d));    //take minimum value of number of steps
			return num;
		}
	}
}

void FindMazes(const Board &board, int desired_path_length, const std::vector<Block> &blocks, std::ostream &ostr)
{
	vector <Board> can; // store all the candidates
	vector <Board> solutions;//store all the solutions
	vector <Board> unique; // store all the unique solutions
	vector<Block> sorted (blocks);
	Board scratch(board);
	sort(sorted.begin(),sorted.end(),larger_than_size); // sort the block by size O(nlogn)

	int num=0; // the index of vector sorted blocks
	int n=0; // number of disired path length maze

	PutBlock(sorted, num, can, scratch);
	ostr<<can.size()<<"candidate board(s)"<<endl;
	for(unsigned int i=0; i<can.size() ; i++){
		can[i].Print(ostr);
		ostr<<endl;
		if(ShortestPath(can[i])==desired_path_length)
		{
			n++;
			solutions.push_back(can[i]);
		}
	}
	ostr<<n<<"solution(s) with path length "<< desired_path_length<<endl;
	for(unsigned int i=0; i<solutions.size();i++){
		solutions[i].Print(ostr);
		ostr<<endl;
	}
	Unique(solutions,unique);
	ostr<<unique.size()<<"unique maze(s)"<<endl;
	for(unsigned int i=0;i<unique.size();i++){
		unique[i].Print(ostr);
		ostr<<endl;
	}
}

void PutBlock(const vector<Block> &blocks, int num, vector <Board> &can, Board &board)
{
	//base case
	if(num==blocks.size()) // all the blocks have been put into the board
	{
		can.push_back(board); //add the board to the candidates
		return;
	}
	//recrusive  part
	else
	{
		unsigned int h=blocks[num].getHeight(); // height of the block
		unsigned int w=blocks[num].getWidth();  // width of the block
	    Location s (board.getStart().x,board.getStart().y);  // the coordinate of start point
		Location e (board.getEnd().x, board.getEnd().y);     // the coordinate of end point
		for(int i=0; i<board.getWidth();i++){       
			for(int j=0;j<board.getHeight();j++){
				if(check(i,j,w,h,board))// the block can be put vertically  O(h*w)
				{
					//write the block into the board
					for(unsigned int m=0;m<w;m++){
						for(unsigned int n=0;n<h;n++){
							board.set(Location(i+m,j+n),blocks[num].getC());
						}
					}
					PutBlock(blocks,num+1,can,board); // put the next block
					//erase this block
					for(unsigned int m=0;m<w;m++){
						for(unsigned int n=0;n<h;n++){
							board.set(Location(i+m,j+n),' ');
						}
					}
				}
			}
		}
	}
}

bool check(int x, int y,int w,int h, Board &board)
{
	Location s (board.getStart().x,board.getStart().y);  // the coordinate of start point
	Location e (board.getEnd().x, board.getEnd().y);     // the coordinate of end point
	//if the block can be put vertically
	if((x+w)<=board.getWidth()&&(y+h)<=board.getHeight()){ // check the height and width can fit
		//check it won't cover other blocks or start\end point
		for(int i=0; i<w; i++){
			for(int j=0;j<h; j++){
				if(board.get(Location(x+i,y+j))!=' ') 
					return false;
			}
		}
		return true;
	}
	return false;
}

void Unique(const vector <Board> & solutions, vector <Board>& unique)
{
	list<Board> u(solutions.begin(),solutions.end()); //convert the solutions into list type
	list<Board>::iterator p=u.begin();
	
	for(p;p!=u.end();p++){
		for(list<Board>::iterator q=p; q!=u.end();q++){
			bool unique=false;
			if( p!=q && !unique ){  //won't compare one board to itslef 
				for(int h=0; h<p->getHeight();h++){
					for(int w=0;w<p->getWidth();w++){
						if(p->get(Location(w,h))!=q->get(Location(w,h))){ // if one charater is different, they are unique
							unique=true;
							break;
						}
					}
					if(unique) 
						break;
				}
				if(!unique) {// not unique, erase duplicate board
					q=u.erase(q);
					q--;
				}
			}
		}
	}				
	unique =vector <Board>(u.begin(),u.end()); // convert back to vector type
	// rewrite the block into #
	for(unsigned int i=0; i< unique.size();i++)
	{
		for(int h=0;h<unique[i].getHeight();h++){
			for(int w=0; w<unique[i].getWidth();w++){
				if(unique[i].get(Location(w,h))!=' ' &&unique[i].get(Location(w,h))!='S'&&unique[i].get(Location(w,h))!='E')
					unique[i].set(Location(w,h),'#');
			}
		}
	}			
}

void FindOneMaze(const Board &board, int desired_path_length, const std::vector<Block> &blocks, std::ostream &ostr)
{
	vector<Block> sorted (blocks);
	Board scratch(board);
	sort(sorted.begin(),sorted.end(),larger_than_size); // sort the block by size

	bool find=false;
	int num=0; // the index of vector sorted blocks

	FindOneMaze(scratch, desired_path_length, sorted, num, find);
	for(int w=0; w<scratch.getWidth();w++){
		for(int h=0;h<scratch.getHeight();h++){
			if(scratch.get(Location(w,h))!=' ' &&scratch.get(Location(w,h))!='S'&&scratch.get(Location(w,h))!='E')
				scratch.set(Location(w,h),'#');
		}
	}
	scratch.Print(ostr);
}

void FindOneMaze(Board & scratch, int n, vector <Block> sorted, int num,bool & find)
{
	//base case , if find the maze which shortest path is n
	if(num==sorted.size()) // all the blocks have been put into the board
	{
		if(ShortestPath(scratch)==n) // find the maze!
			find=true;
	}
	//recrusive  part
	else
	{
		unsigned int h=sorted[num].getHeight(); // height of the block
		unsigned int w=sorted[num].getWidth();  // width of the block
		Location s (scratch.getStart().x,scratch.getStart().y);  // the coordinate of start point
		Location e (scratch.getEnd().x, scratch.getEnd().y);     // the coordinate of end point
		for(int i=0; i<scratch.getWidth();i++)
		{
			for(int j=0;j<scratch.getHeight();j++){
				if(check(i,j,w,h,scratch))// the block can be put into the board
				{
					//write the block into the board
					for(unsigned int m=0;m<w;m++){
						for(unsigned int n=0;n<h;n++){
							scratch.set(Location(i+m,j+n),sorted[num].getC());
						}
					}
					if(!find){
						FindOneMaze(scratch,n,sorted,num+1,find); // put the next block
						if(!find){
							//erase this block
							for(unsigned int m=0;m<w;m++){
								for(unsigned int n=0;n<h;n++){
									scratch.set(Location(i+m,j+n),' ');
								}
							}
						}
						else
							break;
					}
					else
						break;
				}
			}
			if(find)
				break;
		}
	}
}
