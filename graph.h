// graph.h <Starter Code>
// < Your name >
//
// Basic graph class using adjacency matrix representation.  Currently
// limited to a graph with at most 100 vertices.
//
// University of Illinois at Chicago
// CS 251: Fall 2021
// Project #7 - Openstreet Maps
//

#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>
#include <map>


using namespace std;

template<typename VertexT, typename WeightT>
class graph {
private:
    map<VertexT, map<VertexT, WeightT> > adjList;


    //
    // We are using adjacency matrix implementation, where rows
    // are the starting vertex and cols are the ending vertex.
    // We keep track of the vertices in the Vertices vector,
    // where the vertex's position in the vector --- 0, 1, 2,
    // 3, 4, 5, ... --- denotes the row in the adjacency matrix
    // where their edges are found.  Example: if vertex "ORD" is
    // in position 1 of the Vertices vector, then row 1 of
    // AdjMatrix are the edges that start at "ORD" and lead to
    // other vertices.
    //
public:
    //
    // constructor:
    //
    // Constructs an empty graph where n is the max # of vertices
    // you expect the graph to contain.
    //
    // NOTE: the graph is implemented using an adjacency matrix.
    // If n exceeds the dimensions of this matrix, an exception
    // will be thrown to let you know that this implementation
    // will not suffice.
    //
    graph() {
    }

    //
    // NumVertices
    //
    // Returns the # of vertices currently in the graph.
    //
    int NumVertices() const {
        return adjList.size();
    }

    //
    // NumEdges
    //
    // Returns the # of edges currently in the graph.
    //
    int NumEdges() const {
        int count = 0;
        for (auto i : adjList) {
            count += i.second.size();
        }
        return count;
    }

    //
    // addVertex
    //
    // Adds the vertex v to the graph if there's room, and if so
    // returns true.  If the graph is full, or the vertex already
    // exists in the graph, then false is returned.
    //
    bool addVertex(VertexT v) {
        if (adjList.count(v) == 1) {
            return false;
        }
        map<VertexT, WeightT> myMap;
        adjList[v] = myMap;
        return true;
    }

    //
    // addEdge
    //
    // Adds the edge (from, to, weight) to the graph, and returns
    // true.  If the vertices do not exist or for some reason the
    // graph is full, false is returned.
    //
    // NOTE: if the edge already exists, the existing edge weight
    // is overwritten with the new edge weight.
    //
    bool addEdge(VertexT from, VertexT to, WeightT weight) {
        if (adjList.count(from) == 0 || adjList.count(to) == 0) {
            return false;
        }
        adjList[from][to] = weight;
        return true;
    }

    //
    // getWeight
    //
    // Returns the weight associated with a given edge.  If
    // the edge exists, the weight is returned via the reference
    // parameter and true is returned.  If the edge does not
    // exist, the weight parameter is unchanged and false is
    // returned.
    //
    bool getWeight(VertexT from, VertexT to, WeightT& weight) const {
        if (adjList.count(from) == 0 || adjList.at(from).count(to) == 0) {
            return false;
        }
        weight = adjList.at(from).at(to);
        return true;
    }

    //
    // neighbors
    //
    // Returns a set containing the neighbors of v, i.e. all
    // vertices that can be reached from v along one edge.
    // Since a set is returned, the neighbors are returned in
    // sorted order; use foreach to iterate through the set.
    //
    set<VertexT> neighbors(VertexT v) const {
        set<VertexT> mySet;
        if (adjList.count(v) == 0)
            return mySet;
        for (auto i : adjList.at(v))
            mySet.insert(i.first);
        return mySet;
    }

    //
    // getVertices
    //
    // Returns a vector containing all the vertices currently in
    // the graph.
    //
    vector<VertexT> getVertices() const {
        vector<VertexT> myVector;
        for (auto i : adjList)
            myVector.push_back(i.first);
        return myVector;
    }

    //
    // dump
    //
    // Dumps the internal state of the graph for debugging purposes.
    //
    // Example:
    //    graph<string,int>  G(26);
    //    ...
    //    G.dump(cout);  // dump to console
    //
    void dump(ostream& output) const {
        output << "***************************************************" << endl;
        output << "********************* GRAPH ***********************"
            << endl;

        output << "**Num vertices: " << this->NumVertices() << endl;
        output << "**Num edges: " << this->NumEdges() << endl;

        output << endl;
        output << "**Vertices:" << endl;
        int i = 0;
        for (auto& v : adjList) {
            output << " " << i++ << ". " << v.first << endl;
        }

        output << endl;
        output << "**Edges:" << endl;
        for (auto v : adjList) {
            output << v.first << ": ";

            for (auto e : v.second) {
                output << "(" << e.first << ","
                    << e.second
                    << ") ";
            }
            output << endl;
        }
        output << "**************************************************"
            << endl;
    }
};
