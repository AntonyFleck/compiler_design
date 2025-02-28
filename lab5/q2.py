vertices = {1,2,3,4}
edges = [ (1, 2, 1),  # Edge from 1 to 2 with weight 1
    (1, 3, 1),  # Edge from 1 to 3 with weight 1 
    (2, 3, 3),  # Edge from 2 to 3 with weight 3
    (3, 4, 4),
    (4,1,5)]

class Graph:
    def __init__(self):
        self.adj_list ={vertex:[] for vertex in vertices}
    
    def adding_adj_list(self):
        for src,dest,weight in edges:
            self.adj_list[src].append([dest,weight])
            
    def print_adj_list(self):
        for node,neighbors in self.adj_list.items():
            print(f"{node}:{neighbors}")
            
    def create_adj_matrix(self):
        n = len(vertices)
        self.adj_matrix = [[0]*n for i in range(n)]
        for src,dest,weight in edges:
            self.adj_matrix[src-1][dest-1] = weight

    def print_adj_matrix(self):
        for row in self.adj_matrix:
            print(row)





g = Graph()
g.adding_adj_list()
g.print_adj_list()  
g.create_adj_matrix()
g.print_adj_matrix()      
    