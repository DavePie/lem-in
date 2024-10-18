# `lem-in` - Dinic's algorithm (and more)
The goal of this project is to move your ant colony from one location to another in as few turns as possible. This implementation uses a combination of Dinc's algorithm and a priority queue. The result is rendered with 
## Problem Overview
Suppose we have a set of rooms connected by tunnels. Each room can only hold one ant, with the exception of the starting and ending room. During each turn, every ant can move into an adjacent room connected by a tunnel. The goal is that given `N` ants, minimize the number of turns it takes to move all the ants from the starting room to the ending room.\
#### Basic Case (1 ant)
This case is simple; simply find the shortest path through breath-first-search.
#### Complicated Case (2+ ants)
Simply using the shortest path can be suboptimal; if there are multiple paths, we can send more ants to the end at once.
### Naive solution
Suppose we use the shortest path, then the next shortest path, and so on until adding paths no longer decreases the number of turns. However, consider this case:\
\
While we can see there is a more direct path, this path blocks two slightly longer paths, shown below. If we have enough ants, it becomes more efficient to use both paths concurrently, as while it takes longer for the first ant to arrive, the fact that we have two ants arriving at a time causes it to take less turns to finish.

### Brute force solution
Find all the possible paths and all the possible combination of paths. Then chose the best combination. The problem with this approach is its time complexity even with fairly small maps you can have many possible paths, and you can quickly get millions of paths. Then to find every combination of path, and check for intersections