INF = 16

class Router:
    def __init__(self, id, neighbor, distance, next_hop) -> None:
        """ Initializing distance vector, neighbors for this router. """
        self.id = id
        self.neighbor = neighbor
        self.distance = distance
        self.num_nodes = len(distance)
        self.next_hop = next_hop

    def update_routing_table(self, neighbor_id, distance, next_hop) -> int:
        """ This function updates the RT of the self router when its neighbor shares routing table with it. """
        updates = 0 # keep tracks of the number of updates done.
        for node in range(self.num_nodes):
            updated_dist = self.distance[node]
            next_hop_node = self.next_hop[node]
            
            # If the least cost path to 'node' from self is through the neighbor
            # then update the distance to ===> distance from neighbor + distance between self and neighbor.
            if self.next_hop[node] == neighbor_id:
                updated_dist = distance[node] + 1
                next_hop_node = neighbor_id
            
            # If the distance of node from neighbor is INF then no need to change anything.
            elif distance[node] == INF:
                continue 

            # Else check if the distance through "self --> neighbor --> node" path is 
            # less than "self --> node" path.
            else:
                if distance[node] + 1 < self.distance[node]:
                    # If yes then update next_hop and distance to neighbor's distance from node
                    # next_hop to neighbor.
                    updated_dist = distance[node] + 1
                    next_hop_node = neighbor_id

            # If there were no change in the distance and next_hop router then simply skip this pass.
            if updated_dist == self.distance[node] and next_hop_node == self.next_hop[node]:
                continue
            
            updates += 1 # increments the updates count.

            # Set updated values in the Routing table of self.
            self.distance[node] = updated_dist 
            self.next_hop[node] = next_hop_node

        return updates # return the number occured in this pass.

    def share_routing_table(self, neighbor_routers) -> int:
        """ Router shares its routing table to its neighbors """
        updates = 0 # keep tracks of the number of updates done.
        for router in neighbor_routers:

            # Applying Split Horizon Method.
            distance = self.distance[:]
            next_hop = self.next_hop[:]

            for i in range(self.num_nodes):
                # If next_hop of self is neighbor router itself
                # then trim the routing table.
                if next_hop[i] == router.id:
                    distance[i] = INF
                    next_hop[i] = -1

            # updating routing tables of neighbors.
            updates += router.update_routing_table(self.id, distance, next_hop)

        return updates # return the number occured in this pass.

    def print_routing_table(self):
        """ For debugging purpose only """
        map = {0:'A', 1:'B', 2:'C', 3:'D', 4:'E', -1:'-'}
        for i in range(self.num_nodes):
            last = " "
            if i == 4:
                last = "\n"
            if self.distance[i] == INF:
                print(f'-{map[self.next_hop[i]]}', end=last)
            else:
                print(f'{self.distance[i]}{map[self.next_hop[i]]}', end=last) 
