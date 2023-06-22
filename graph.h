// graph.h <Starter Code>
// Zach Savignano
//
// Basic graph class using adjacency matrix representation.  Currently
// limited to a graph with at most 100 vertices.
//
//
// Adam T Koehler, PhD
// University of Illinois Chicago
// CS 251, Fall 2022
//
// Project Original Variartion By:
// Joe Hummel, PhD
// University of Illinois at Chicago
//

#pragma once

#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <vector>

using namespace std;

template <typename VertexT, typename WeightT> class graph {
private:
  struct node {
    VertexT to;
    WeightT weight;
    bool operator<(const node &other) const { return weight < other.weight; }
  };

  map<VertexT, set<node>> adjList;
  set<VertexT> vertices;

public:
  //
  // constructor:
  //
  // Constructs an empty graph
  //
  //
  graph() {}

  //
  // NumVertices
  //
  // Returns the # of vertices currently in the graph.
  //
  int NumVertices() const { return static_cast<int>(this->vertices.size()); }

  //
  // NumEdges
  //
  // Returns the # of edges currently in the graph.
  //
  int NumEdges() const {
    int count = 0;
    //
    // loop through the adjacency list and count how many
    // edges currently exist:
    //
    for (VertexT v : vertices) {
      if (adjList.count(v) > 0)
        count += adjList.at(v).size();
    }
    return count;
  }

  //
  // addVertex
  //
  // Adds the vertex v to the graph. If the vertex already
  // exists in the graph, then false is returned.
  //
  bool addVertex(VertexT v) {
    if (vertices.count(v) > 0)
      return false;
    vertices.insert(v);
    return true;
  }

  //
  // addEdge
  //
  // Adds the edge (from, to, weight) to the graph, and returns
  // true.  If the vertices do not exist false is returned.
  //
  // NOTE: if the edge already exists, the existing edge weight
  // is overwritten with the new edge weight.
  //
  bool addEdge(VertexT from, VertexT to, WeightT weight) {
    if (vertices.count(to) == 0 || vertices.count(from) == 0)
      return false;
    if (adjList.count(from) > 0) {
      for (node n : adjList.at(from)) {
        if (n.to == to) {
          n.weight = weight;
          return true;
        }
      }
    }
    node temp;
    temp.to = to;
    temp.weight = weight;
    adjList[from].insert(temp);
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
  bool getWeight(VertexT from, VertexT to, WeightT &weight) const {
    if (adjList.count(from) > 0) {
      for (node n : adjList.at(from)) {
        if (n.to == to) {
          weight = n.weight;
          return true;
        }
      }
    }
    return false;
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
    set<VertexT> S;
    if (adjList.count(v) > 0) {
      for (node n : adjList.at(v))
        S.insert(n.to);
    }
    return S;
  }

  //
  // getVertices
  //
  // Returns a vector containing all the vertices currently in
  // the graph.
  //
  vector<VertexT> getVertices() const {
    vector<VertexT> temp;
    for (VertexT v : vertices)
      temp.push_back(v);
    return temp;
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

  void dump(ostream &output) const {
    output << "***************************************************" << endl;
    output << "********************* GRAPH ***********************" << endl;

    output << "**Num vertices: " << this->NumVertices() << endl;
    output << "**Num edges: " << this->NumEdges() << endl;

    output << endl;
    output << "**Vertices:" << endl;
    int count = 0;
    for (VertexT v : vertices) {
      output << " " << count << ". " << v << endl;
      count++;
    }

    output << endl;
    output << "**Edges:" << endl;
    count = 0;
    for (VertexT v : vertices) {
      output << v << ": ";
      if (adjList.count(v) > 0) {
        for (node n : adjList.at(v)) {
          output << "(" << v << "," << n.to << "," << n.weight << ") ";
        }
      }
      output << endl;
    }
    output << "**************************************************" << endl;
  }
};
