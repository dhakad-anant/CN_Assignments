from sklearn import neighbors
from Graph import Graph
from Router import Router

map = {0:'A', 1:'B', 2:'C', 3:'D', 4:'E', -1:'-'} # only for debugging purposes

# debug = True # only for debugging purposes
debug = False # only for debugging purposes

def configure_router():
    """ This function initilizes the routing table """
    for i in range(TOTAL_NUM_NODES):
        neighbor = graph.get_neighbor(i)
        distance = [INF]*TOTAL_NUM_NODES
        next_hop = [-1]*TOTAL_NUM_NODES
        distance[i] = 0 # distance from self is zero
        next_hop[i] = i # next_hop for self is self only
        for node in neighbor:
            distance[node] = 1 # distance from self is one
            next_hop[node] = node # next_hop for neighbor is neighbor only
        
        # initializing a Router object.
        routers.append(Router(i, neighbor, distance, next_hop))

def DISTANCE_VECTOR_ALGO_SIMULATION(startTime, routers, graph, debug=False):
    """ This function returns the time taken by the network to get stable """

    # Starting the simulation
    time = startTime
    while(True):
        updates = 0

        if debug:
            print(f"\n<---### At t={time+1} ###--->")

        for i in range(TOTAL_NUM_NODES):
            # Each router shares its routing table to all its neighbor.
            # Routers share in order --> A, B, ... , E
            updates += routers[i].share_routing_table(graph.get_neighbor_router(i, routers))
            
            if debug: # for debug purposes
                neighbor = graph.get_neighbor(i)
                if len(neighbor) == 2:
                    print(f'Distance Vectors after {map[i]} shares table to {map[neighbor[0]]} & {map[neighbor[1]]}: ')
                else:
                    print(f'Routing Table after {map[i]} shares table to {map[neighbor[0]]}: ')
                for i in range(TOTAL_NUM_NODES):
                    routers[i].print_routing_table()
                print()

        # If there are zero new updates then the system has stabilized.
        if updates == 0:
            if debug:
                print("There Were No updates in this pass. So it was stable one second ago.!!!!!\n\n")
            break

        # Increment the time after one complete round
        time += 1
    
    # return the final time when the system to stabilizes from the startTime.
    return time

if __name__ == "__main__":
    global TOTAL_NUM_NODES
    TOTAL_NUM_NODES = 5
    global INF
    INF = 16

    """ ############################## PART 1 ############################## """

    """ graph configuration """
    global graph 
    # Passing number of nodes and edges in graph.
    graph = Graph(5, [[0, 1], [1, 2], [2, 3], [3, 4], [0, 4]])

    global routers 
    routers = []

    # Initializing router tables
    configure_router()
    
    if debug: # only for debugging purposes
        print("intial routing table---> ")
        for i in range(TOTAL_NUM_NODES):
            routers[i].print_routing_table()
        print("------------------------ \n")
    
    """ Simulating Part1 """
    x = DISTANCE_VECTOR_ALGO_SIMULATION(0, routers, graph, True)
    print("Time at Part1 stabilizes --> ", x, "\n")
    print("Final Routing table after Part1 : ")
    for i in range(TOTAL_NUM_NODES):
        routers[i].print_routing_table()
    print("-------------------------------------")

    """ ############################## PART 2 ############################## """
    
    """ Link BC breaks at t = x+1 """
    # So now edge [1, 2] will be removed from the graph.
    x += 1
    # Passing number of nodes and edges in graph.
    graph = Graph(5, [[0, 1], [2, 3], [3, 4], [0, 4]])
    routers = []
    # Initializing router tables
    configure_router()

    """ Updating the routing table at t = x+2 """
    x += 1
    # Updating for B
    for i in range(TOTAL_NUM_NODES):
        # Update the nodes where the next hop was C for B.
        if routers[1].next_hop[i] == 2:
            routers[1].distance[i] = INF
            routers[1].next_hop[i] = -1
    # Updating for C
    for i in range(TOTAL_NUM_NODES):
        # Update the nodes where the next hop was B for C.
        if routers[2].next_hop[i] == 1:
            routers[2].distance[i] = INF
            routers[2].next_hop[i] = -1

    if debug: # only for debugging purposes
        print("Routing table before Part2---> ")
        for i in range(TOTAL_NUM_NODES):
            routers[i].print_routing_table()
        print("------------------------ \n")

    """ Simulating Part2 """
    x = DISTANCE_VECTOR_ALGO_SIMULATION(x, routers, graph)
    print("\nTime at Part2 stabilizes --> ", x, "\n")
    print("Final Routing table after Part2 : ")
    for i in range(TOTAL_NUM_NODES):
        routers[i].print_routing_table()

