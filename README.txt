HOMEWORK 6: MAZE MAKER


NAME:  <Ziyun Xiao >


COLLABORATORS: 
You must do this assignment on your own, as described in the 
Academic Integrity Policy.  If you did discuss the problem or error
messages, etc. with anyone, please list their names here.



ALGORITHM ANALYSIS:
What's the order notation of your algorithm?

for ShortestPath function : O(h*w)  => O(n^2)
							h and w are the height and the width of the board
for FindMazes:  O(nlogn*w*h*(w*h+2ab)*x*y)  => O(n^7*logn)
                nlogn is for the sort function. w and h are the width and height of the board
                a and b are the width and height of the block
                x is the number of the blocks
                y is the number of ways to combine the blocks in the board
             
for FindOneMaze: the worse case is same as FindMazes function



TEST CASE SUMMARY:
How did your program perform on the different test cases?  Summarize
the running times.  (It's ok if it didn't finish the harder examples.)
What new test cases did you create and how did it perform on those
tests?

the shortest path function is fast , the running time is about one seconds
the FindOneMazes is much faster than FindMazes for most of the test cases, it takes a few seconds
the FindMazes function is very very slow when solving complicated test cases
I tried a few test cases with large board but 2 or 3 blocks and some with normal size board but 4 or 5 blocks,
it is much slower when there are more block. 
so I think the number of blocks effects the running time of FindMazes most, so does the size of the board, but the influence is less.


MISC. COMMENTS TO GRADER:  
Optional, please be concise!

