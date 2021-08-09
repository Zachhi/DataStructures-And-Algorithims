#ifndef DIRECTEDGRAPH_H
#define DIRECTEDGRAPH_H

#include <iostream>
#include <vector>
#include <cassert> 
#include <cmath> 
#include <cstddef> 
#include <iostream> 
#include <list> 
#include <sstream> 
#include <stack> 
#include <queue> 
#include <unordered_map> 
#include <unordered_set> 

//EDGE STRUCT
struct EdgeDirected {
    int check = 0;
    double weight = 0;
};
struct VertexDirected
{
    size_t id{};
    int index{};
    bool visited = false;
    size_t numEdgeRecieving = 0;
    size_t numEdgeGiven = 0;
    double distance = INFINITY;
};

class GraphDirected {

public:
    //Initializers
    std::unordered_set<size_t> unvisited {};        //for dijkstras 
    std::vector<std::vector<EdgeDirected>> graphMatrix {}; //adjacency matrix for the graph
    std::vector<VertexDirected> vertexList {};              //keep track of all verticies and their values
    std::vector<std::vector<size_t>> paths;         //for paths
    size_t numOfVertex {};
    size_t numOfEdge {};
    

    //Constructors
    GraphDirected() 
    {
        //std::unordered_set<size_t> shortestPathSet (0);
        std::unordered_set<size_t> unvisited (0);
        std::vector<std::vector<EdgeDirected>> graphMatrix(0); 
        std::vector<VertexDirected> vertexList(0);
        std::vector<std::vector<size_t>> paths(0); 
        numOfVertex = 0;
        numOfEdge = 0;
    }
    GraphDirected(const GraphDirected& other)// - constructs a deep copy of a graph
    {
        vertexList.clear();
        for(int i = 0; i < graphMatrix.size(); i++)
            graphMatrix[i].clear();
        for(int i = 0; i < paths.size(); i++)
            paths[i].clear();
        
        for(int i = 0; i < other.vertexList.size(); i++)
        {
            vertexList.push_back(other.vertexList[i]);
        }

        graphMatrix.resize(other.graphMatrix.size());
        for(int i = 0; i < other.graphMatrix.size(); i++)
        {
            for(int j = 0; j < other.graphMatrix[i].size(); j++)
            {
                graphMatrix[i].push_back(other.graphMatrix[i][j]);
            }
        }
        paths.resize(other.paths.size());
        for(int i = 0; i < other.paths.size(); i++)
        {
            for(int j = 0; j < other.paths[i].size(); j++)
            {
                paths[i].push_back(other.paths[i][j]);
            }
        }        

        unvisited.clear();
        for(auto x : other.unvisited)
        {
            unvisited.insert(x);
        }

        numOfVertex = other.numOfVertex;
        numOfEdge = other.numOfEdge;
    }
    GraphDirected& operator=(const GraphDirected& other)// - assigns a deep copy of a graph
    {   
        if(this != &other)
        {
        this->vertexList.clear();
        for(int i = 0; i < graphMatrix.size(); i++)
            this->graphMatrix[i].clear();
        for(int i = 0; i < paths.size(); i++)
            this->paths[i].clear();
        
        for(int i = 0; i < other.vertexList.size(); i++)
        {
            this->vertexList.push_back(other.vertexList[i]);
        }

        this->graphMatrix.resize(other.graphMatrix.size());
        for(int i = 0; i < other.graphMatrix.size(); i++)
        {
            for(int j = 0; j < other.graphMatrix[i].size(); j++)
            {
                this->graphMatrix[i].push_back(other.graphMatrix[i][j]);
            }
        }
        this->paths.resize(other.paths.size());
        for(int i = 0; i < other.paths.size(); i++)
        {
            for(int j = 0; j < other.paths[i].size(); j++)
            {
                this->paths[i].push_back(other.paths[i][j]);
            }
        } 

        this->unvisited.clear();
        for(auto x : other.unvisited)
        {
            this->unvisited.insert(x);
        }
        this->numOfVertex = other.numOfVertex;
        this->numOfEdge = other.numOfEdge;
        }
        return *this;
    }
    ~GraphDirected()// - destructs a graph (frees all dynamically allocated memory)
    {

    }

    //Capacity
    size_t vertex_count() const 
    {
        return numOfVertex;
    }
    size_t edge_count() const 
    {
        return numOfEdge;
    }

    //Element Access
    bool contains_vertex(size_t id) const
    {
        //loop through vertex list checking for id
        for(VertexDirected x : vertexList) 
        {
            if (x.id == id)
                return true;
        }
        return false;
    }
    bool contains_edge(size_t src, size_t dest) const
    {
        //first check if the source and destination exist
        if(!contains_vertex(src) || !contains_vertex(dest))
        {
            return false;
        }
        //now retrieve index's of source and destination vertex
        int srcIndex = getIndex(src);
        int destIndex = getIndex(dest);

        if(graphMatrix[srcIndex][destIndex].check == 1)
            return true;
        
        return false;    
    }
    double cost(size_t src, size_t dest) const
    {
        double inf = INFINITY;
        //if edge doesnt esixt return inf
        if(!contains_edge(src, dest))
        {
            return inf;
        }
        //else return weight of edge
        else
            {
                int srcIndex = getIndex(src);
                int destIndex = getIndex(dest);
                double eWeight = graphMatrix[srcIndex][destIndex].weight;
                return eWeight;
            }
    }

    //Modifiers
    bool add_vertex(size_t id)
    {
        //check if already added
        if(contains_vertex(id))
            return false;

        // if not already added, make a new vertex and pushback onto vertex list
        VertexDirected newV;
        newV.id = id;
        newV.index = vertexList.size();
        vertexList.push_back(newV);   

        //resize the graph here
        graphMatrix.resize(graphMatrix.size()+1);
        for(int i = 0; i < graphMatrix.size(); i++)
            graphMatrix[i].resize(graphMatrix.size());

        numOfVertex++;

        return true;
    }
    bool add_edge(size_t src, size_t dest, double weight=1)
    {
        
        //check if there already is an edge from src to dest
        if(contains_edge(src, dest) || !contains_vertex(src) || !contains_vertex(dest))
            return false;

        //if not, add it
        int srcIndex = getIndex(src);
        int destIndex = getIndex(dest);

        //update these values
        vertexList[srcIndex].numEdgeGiven +=1;
        vertexList[destIndex].numEdgeRecieving +=1;
        //set check to 1 so we know edge exists
        graphMatrix[srcIndex][destIndex].check = 1;
        //set the weight to given weight
        graphMatrix[srcIndex][destIndex].weight = weight;

        numOfEdge++;

        return true;
        
    }
    bool remove_vertex(size_t id)
    {
        //return false if vertex isnt found
        if(!contains_vertex(id))
            return false;

        //erase vertex from vertexList
        int vertexIndex = getIndex(id);
        //fix numOfEdge variable first
        for(int i = 0; i < graphMatrix.size(); i++)
        {
            if(i!=vertexIndex)
            {
            if(graphMatrix[i][vertexIndex].check==1)
            {
                vertexList[vertexIndex].numEdgeRecieving--;
                vertexList[i].numEdgeGiven--;
                numOfEdge--;
            }
            if(graphMatrix[vertexIndex][i].check==1)
            {
                vertexList[vertexIndex].numEdgeGiven--;
                vertexList[i].numEdgeRecieving--;
                numOfEdge--;
            }
            }
        }
        //now erase
        vertexList.erase(vertexList.begin() + vertexIndex);

        //reset the vertex indexes after deleting a vertex from the list
        resetVertexIndexs();

        //fix the graph now

        //delete the row the vertex owned
        graphMatrix.erase(graphMatrix.begin() + vertexIndex);
        //delete the coolumn the vertex owned
        for(int i = 0; i < graphMatrix.size(); i++)
            graphMatrix[i].erase(graphMatrix[i].begin() + vertexIndex);

        numOfVertex--;

        return true;
    }
    bool remove_edge(size_t src, size_t dest)
    {
        //return false if edge doesnt exist
        if(!contains_edge(src, dest))
        {
            return false;
        }
        
        //if does exist, just set check and weight to 0
        int srcIndex = getIndex(src);
        int destIndex = getIndex(dest);

        graphMatrix[srcIndex][destIndex].check = 0;
        graphMatrix[srcIndex][destIndex].weight = 0;

        //update these values
        vertexList[srcIndex].numEdgeGiven --;
        vertexList[destIndex].numEdgeRecieving --;

        numOfEdge--;

        return true;
    }

    //Dijkstra's Functions
    void dijkstra(size_t srcId) 
    {   
        if(numOfVertex<1)
        {
            return;
        }
        for(int i = 0; i < paths.size(); i++)
            paths[i].clear();
        paths.resize(vertexList.size());    //clear the paths matrix
        unvisited.clear();
        for(int i = 0; i < vertexList.size(); i++)
        {
            vertexList[i].visited = false;
            vertexList[i].distance = INFINITY;
        }
        
        //init
        double min;
        size_t currVertex;
        size_t nextVertex;
        int currIndex;
        int nextIndex;
        double currDist;
        double inf = INFINITY;
        std::vector<size_t> currentPath;
        std::vector<size_t> modifiedPath;

        
        //fill set up with unvisited nodes
        for(int i = 0; i < vertexList.size(); i++)
            unvisited.insert(vertexList[i].id);

        //assign distance of source to 0 so its picked first 
        currVertex = srcId;
        currIndex = getIndex(srcId);
        vertexList[currIndex].distance = 0;
        currDist = vertexList[currIndex].distance;
        //initialize paths
        for(int i = 0; i < vertexList.size(); i++)
        {
            paths[i].push_back(vertexList[i].id);
        }
        if(vertexList[currIndex].numEdgeGiven == 0)
        {
            return;
        }
        //while the unvisited set is not empty ->
        while(unvisited.size()>0)
        {
            //check for unreachable nodes
            bool isUnreachable = true;
            for(size_t x : unvisited)
            {
                size_t index = getIndex(x);
                if(vertexList[index].distance != inf)
                    isUnreachable = false;
            }
            if(isUnreachable)
            {
                break;
            }

            
            //set current path
            currentPath = paths[currIndex];

            //erase at beggining
            unvisited.erase(currVertex);
            min = inf;
            //update distance of all verticies that are adjacent to current vertex
            for(int i = 0; i < graphMatrix.size(); i++)
            {
                currDist = vertexList[currIndex].distance;
                if(graphMatrix[currIndex][i].check==1 && !vertexList[i].visited)      //if curr->vertex and that vertex has not been visited
                {
                    double oneToOneDist = cost(currVertex, vertexList[i].id);  //get distance of currVertex->adjacentVertex
                    currDist += oneToOneDist;                                 //get total distance: distance of what it take to get to currVertex + the distance above   

                    //everytime we modify min distance of a node, update the path
                    if(currDist < vertexList[i].distance)                       //if distance found is less than its current distance, update it
                    {                                           
                        vertexList[i].distance = currDist;

                        modifiedPath = currentPath;
                        modifiedPath.push_back(paths[i][paths[i].size()-1]);
                        paths[i] = modifiedPath;
                    }
                }
            }

            //set visited for currVertex = true
            vertexList[currIndex].visited = true;

            //now we need to find the next node to visit
            for(int i = 0; i < vertexList.size(); i++)
            {
                if(vertexList[i].distance < min && !vertexList[i].visited)
                {
                    min = vertexList[i].distance;
                    nextVertex = vertexList[i].id;
                    nextIndex = getIndex(nextVertex);
                }
            }
            
            //update values and repeat
            currVertex = nextVertex;
            currIndex = nextIndex;
            currDist = vertexList[currIndex].distance;
        }
        
    }
    double distance(size_t id) const// - assumes Dijkstra has been run, returns the cost of the shortest path from the Dijkstra-source vertex to the specified destination vertex, or INFINITY if the vertex or path does not exist.
    {
        size_t srcId;
        size_t idIndex;
        double inf = INFINITY;
        for(int i = 0; i < vertexList.size(); i++)
        {
            if(vertexList[i].distance == 0)
                srcId = vertexList[i].id;
            if(vertexList[i].id == id)
                idIndex = i;
        }

        if(!contains_vertex(id))
            return inf;
        
        double ans = vertexList[idIndex].distance;
        return ans;
    }

    //Print Path
    void print_shortest_path(size_t dest_id, std::ostream& os=std::cout) const// - assumes Dijkstra has been run, pretty prints the shortest path from the Dijkstra source vertex to the specified destination vertex in a “ → “- separated list with “ distance: #####” at the end, where <distance> is the minimum cost of a path from source to destination, or prints “<no path>\n” if the vertex is unreachable.
    {
        if(numOfVertex<1)
        {
            os << "<no path>\n";
            return;
        }
        if(!contains_vertex(dest_id))
        {
            os << "<no path>\n";
            return;            
        }
        int srcIndex;
        for(int i = 0; i < vertexList.size(); i++)
            if(vertexList[i].distance==0)
                srcIndex = vertexList[i].index;
        
        size_t src = vertexList[srcIndex].id;
        int destIndex = getIndex(dest_id);

        if(!contains_vertex(dest_id)||!contains_vertex(src)||distance(dest_id) == INFINITY)
        {
            os << "<no path>\n";
            return;
        }
  
        for(int i = 0; i < paths[destIndex].size(); i++)
        {
            if(i == paths[destIndex].size()-1)
                os << paths[destIndex][i] << " distance: " << distance(dest_id) << std::endl;
            else
                os << paths[destIndex][i] << " --> ";
        }
    }

    //My Functions
    void displayMatrix()
    {
        std::cout << "X: ";
        for(int i = 0; i < graphMatrix.size(); i++)
            std::cout << i << " ";
        std::cout << std::endl;
        for(int i = 0; i <graphMatrix.size(); i++)
        {
            std::cout << i << ": ";
            for(int j = 0; j < graphMatrix[i].size(); j++)
                std::cout << graphMatrix[i][j].check << " ";
            std::cout << std::endl;
        }
    }
    void displayVertex()
    {
        for(int i = 0; i < vertexList.size(); i++)
            std::cout << "Vertex Id: " << vertexList[i].id << " Vertex index: " << vertexList[i].index << " is known: " << vertexList[i].visited << std::endl;
    }
    int getIndex(size_t id) const
    {
        int index = -99;
        for(VertexDirected x : vertexList)
            if(x.id == id)
                index = x.index;
        
        return index;
    }
    void resetVertexIndexs()
    {
        for(int i = 0; i < vertexList.size(); i++)
            vertexList[i].index = i;
    }
    void displayShortestSet()
    {   
        size_t id = -99;
        for(int i = 0; i < vertexList.size(); i++)
        {
            if(vertexList[i].distance == 0)
                id = vertexList[i].id;
        }
        std::cout << "Vertex: " << "            " << "Distance from " << id << std::endl;
        for(int i = 0; i < vertexList.size(); i++)
        {
            std::cout << vertexList[i].index << "                       " << vertexList[i].distance << std::endl;
        }
    }
};

#endif  // GRAPH_H
