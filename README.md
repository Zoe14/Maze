# Maze
Find the shortest path for a given maze. And automatically generate a maze with a specific minimum path length.

##ALGORITHM ANALYSIS:
For ShortestPath function :  
O(h*w)  => O(n^2)  
h and w are the height and the width of the board

For FindMazes:  
O(nlogn*w*h*(w*h+2ab)*x*y)  => O(n^7*logn)  
nlogn is for the sort function. w and h are the width and height of the board  
a and b are the width and height of the block  
x is the number of the blocks  
y is the number of ways to combine the blocks in the board  
             
for FindOneMaze:  
the worse case is same as FindMazes function
