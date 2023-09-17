# Djkistra’s Shortest Path Finder

## About Project

My project employs the priority queue, adjacent lists, sets, hash tables and graphs data structures to find the shortest paths that can be followed from any chosen vertex to all the other vertices in the graph using Dijkstra's algorithm.

The graph has 100 vertices placed on a cartesian plane such that they y coordinate of each vertex is its x-value squared. The x values run from 1 to 100.

The distances from each vertex to all other vertices are calculated and saved in an array of adjacent lists (table data structure).

Two vertices are considered to be adjacent to each other if the distance between them is less than 13 otherwise they are not adjacent and the distance between them is considered to be infinity.

Initally all the best or shortest distances to your starting vertex are infinity because they are not known except the distance to your starting point is zero. All these values (infinity and zero) are place into a heap.

The next shortest distance is popped from the heap data structure and used to improve the distances of its adjacent vertices from the source vertex. 

What is interesting about this project is that I used a heap data structure whose priority criteria (best distances) is modified as the best distances are being improved. Thus whenever a vertex has its best distance (length of best path to the source vertex changed) its position in the priority queue may change in real time.

The algorithm works by starting at the starting point that you choose and examing all vertices close to that vertex and improving their best distances from the source vertex and possibly their position in the priority queue. It then pops the next vertex that has the best distance from the heap and adds to the set of already observed vertices. We use the set to keep track to the vertices whose best distances we already know so that we dont calculate them again.

## How to run?

You can directly compile the main.c file through VSCode or use the main.exe program to execute.