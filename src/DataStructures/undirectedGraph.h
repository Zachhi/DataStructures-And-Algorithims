#ifndef UNDIRECTEDGRAPH_H
#define UNDIRECTEDGRAPH_H

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
struct Edge {
    int check = 0;
    double weight = 0;
};
struct Vertex
{
    size_t id{};
    int index{};
    bool visited = false;
    size_t vertexEdges = 0;
    double distance = INFINITY;
};

class GraphUndirected {

public:
    //Initializers

    std::vector<std::vector<Edge>> graphMatrix {}; //adjacency matrix for the graph
    std::vector<Vertex> vertexList {};              //keep track of all verticies and their values
    std::list<std::pair<size_t,size_t>> result{};
    size_t numOfVertex {};
    size_t numOfEdge {};
    

    //Constructors
    GraphUndirected() 
    {
        //std::unordered_set<size_t> shortestPathSet (0);
        std::unordered_set<size_t> unvisited (0);
        std::vector<std::vector<Edge>> graphMatrix(0); 
        std::vector<Vertex> vertexList(0);
        std::vector<std::vector<size_t>> paths(0); 
        numOfVertex = 0;
        numOfEdge = 0;
    }
    GraphUndirected(const GraphUndirected& other)// - constructs a deep copy of a graph
    {
        vertexList.clear();
        for(int i = 0; i < graphMatrix.size(); i++)
            graphMatrix[i].clear();

        
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
        


        numOfVertex = other.numOfVertex;
        numOfEdge = other.numOfEdge;
    }
    GraphUndirected& operator=(const GraphUndirected& other)// - assigns a deep copy of a graph
    {   
        if(this != &other)
        {
        this->vertexList.clear();
        for(int i = 0; i < graphMatrix.size(); i++)
            this->graphMatrix[i].clear();
        
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

        this->numOfVertex = other.numOfVertex;
        this->numOfEdge = other.numOfEdge;
        }
        return *this;
    }
    ~GraphUndirected()// - destructs a graph (frees all dynamically allocated memory)
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
        for(Vertex x : vertexList) 
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
        Vertex newV;
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
        vertexList[srcIndex].vertexEdges +=1;
        vertexList[destIndex].vertexEdges +=1;
        //set check to 1 so we know edge exists
        graphMatrix[srcIndex][destIndex].check = 1;
        graphMatrix[destIndex][srcIndex].check = 1;
        //set the weight to given weight
        graphMatrix[srcIndex][destIndex].weight = weight;
        graphMatrix[destIndex][srcIndex].weight = weight;

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
                vertexList[vertexIndex].vertexEdges--;
                vertexList[i].vertexEdges--;
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
        graphMatrix[destIndex][srcIndex].check = 0;
        graphMatrix[destIndex][srcIndex].weight = 0;
        //update these values
        vertexList[srcIndex].vertexEdges --;
        vertexList[destIndex].vertexEdges --;

        numOfEdge--;

        return true;
    }

    //Prims
    std::list<std::pair<size_t,size_t>> prim()
    {
        double min;
        size_t currVertex;
        size_t nextVertex;
        int currIndex;
        int nextIndex;
        double currDist;
        double inf = INFINITY;

        std::unordered_set<size_t> mstSet {};
        for(auto it = result.begin(); it!=result.end(); )
        {
            it = result.erase(it);
        }

        if(numOfVertex<1)
        {
            return result;
        }
        for(int i = 0; i < vertexList.size(); i++)
        {
            vertexList[i].visited = false;
            vertexList[i].distance = INFINITY;
        }

        vertexList[1].distance = 0;
        currIndex = 1;
        currVertex = vertexList[currIndex].id;

        while(mstSet.size() < vertexList.size()-1)
        {

            mstSet.insert(currVertex);
            vertexList[currIndex].visited = true;
            //updating the distances
            for(int i = 0; i < graphMatrix.size(); i++)
            {
                if(graphMatrix[currIndex][i].check == 1 && !vertexList[i].visited)
                {
                    double thisDist = cost(currVertex, vertexList[i].id);
                    std::cout << thisDist << " vs " << vertexList[i].distance << std::endl;
                    std::cout << "for " << currVertex << std::endl;
                    if(thisDist < vertexList[i].distance)
                    {
                        vertexList[i].distance = thisDist;
                        std::cout << thisDist << " won for " << currVertex << std::endl;
                    }
                }
            }

            min = inf;
            
            int hold;
            //now choose which distance
            for(int i = 0; i < vertexList.size(); i++)
            {
                if(vertexList[i].distance < min && !vertexList[i].visited )
                {
                    min = vertexList[i].distance;
                    nextVertex = vertexList[i].id;
                    nextIndex = getIndex(nextVertex);
                    hold = i;
                    std::cout << "min value for: " << currVertex << "is " << min <<std::endl;
                }
            }
            if(graphMatrix[hold][currIndex].check != 1 || graphMatrix[hold][currIndex].weight != min)
            {
                for(int i = 0; i < vertexList.size(); i++)
                {
                    if(vertexList[i].visited && graphMatrix[i][hold].check==1 && graphMatrix[i][hold].weight==min)
                    {
                        std::cout << "This is for: " << currVertex << " with min: " <<min <<std::endl;
                        currVertex = vertexList[i].id;
                        currIndex = vertexList[i].index;
                        std::cout << "after is: " << currVertex << " with min: " <<min <<std::endl;
                    }
                }
            }
            result.push_back({currVertex, nextVertex});

            currVertex = nextVertex;
            currIndex = nextIndex;

        }
        return result;
    }
    double distance(size_t id) const 
    {
        double inf = INFINITY;
        if(!contains_vertex(id))
            return inf;

        
        int index = getIndex(id);
        if(vertexList[index].vertexEdges==0)
            return inf;
        if(index == 1)
            return 0;
        double ans;
        

        
        
        for(const auto & pair : result)
        {
             if(pair.second == id)
             {
                 ans = cost(pair.first, pair.second);
             }
        }
        return ans;
    }

    //Print Prims
    void print_minimum_spanning_tree(std::ostream& os=std::cout) const 
    {
         for(const auto & pair : result)
         {
             std::cout << pair.first << " --{" << distance(pair.second) << "} " << pair.second << ";" << std::endl;
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
        for(Vertex x : vertexList)
            if(x.id == id)
                index = x.index;
        
        return index;
    }
    void resetVertexIndexs()
    {
        for(int i = 0; i < vertexList.size(); i++)
            vertexList[i].index = i;
    }
};

#endif  // GRAPH_H