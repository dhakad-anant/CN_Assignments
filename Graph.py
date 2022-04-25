class Graph:
    def __init__(self, num_nodes, edges) -> None:
        # Initializing edges and number of nodes.
        self.num_nodes = num_nodes
        self.adj_mat = [[] for i in range(num_nodes)]
        self.edges = edges

        # Building adjacency table.
        self.initial_config()

    def initial_config(self):
        # Filling adjacency matrix
        for edge in self.edges:
            self.adj_mat[edge[0]].append(edge[1])
            self.adj_mat[edge[1]].append(edge[0])
            
    def get_neighbor(self, node):
        """ Returns a list of neighbors of node """
        return self.adj_mat[node]
    
    def get_neighbor_router(self, node, routers):
        """ Returns router objects corresponding to neighbors of node """
        neighbor = self.get_neighbor(node)
        neighbor_routers = []
        for i in range(self.num_nodes):
            if i in neighbor:
                neighbor_routers.append(routers[i])
        return neighbor_routers