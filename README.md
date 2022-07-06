# k-betweenness

Betweenness centrality based on shortest paths is a standard measure of control utilized in numerous studies and implemented in all relevant software tools for network analysis.
To exclude paths longer than k, we only have to stop the breadth-first search of the basic algorithm when a vertex of distance k is reached. 
For this purpose, we use the algorithm defined by Ulrik Brandes in On variants of shortest-path betweenness centrality and their generic computation (2008).
Dependencies
- igraph
