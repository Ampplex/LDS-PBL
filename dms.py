def tsp_dfs(graph, start, path=None, visited=None, cost=0, min_cost=[float('inf')], best_path=[None]):
    n = len(graph)
    
    if path is None:
        path = [start]
    if visited is None:
        visited = set([start])
    
    # Base case: If all cities are visited, return to the start city
    if len(path) == n:
        total_cost = cost + graph[path[-1]][start]  # Complete the cycle
        if total_cost < min_cost[0]:  # Update minimum cost and path
            min_cost[0] = total_cost
            best_path[0] = path + [start]
        return
    
    # Recursive DFS for all unvisited cities
    for next_city in range(n):
        if next_city not in visited:
            visited.add(next_city)
            tsp_dfs(graph, start, path + [next_city], visited, 
                    cost + graph[path[-1]][next_city], min_cost, best_path)
            visited.remove(next_city)  # Backtrack

    return min_cost[0], best_path[0]

# Example Graph (Adjacency Matrix Representation)
graph = [
    [0, 10, 15, 20],
    [10, 0, 35, 25],
    [15, 35, 0, 30],
    [20, 25, 30, 0]
]

start_city = 0
cost, path = tsp_dfs(graph, start_city)
print("Minimum Cost:", cost)
print("Optimal Path:", path)