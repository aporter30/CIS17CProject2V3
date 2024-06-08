/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   graph.h
 * Author: ajporter
 *
 * Created on June 1, 2024, 11:45 AM
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class Graph {
private:
    unordered_map<string, vector<string> > adjList;

public:
    void addEdge(const string& v1, const string& v2);
    void BFS(const string& start);
    void DFSUtil(const string& v, unordered_map<string, bool>& visited);
    void DFS(const string& start);
};

#endif // GRAPH_H
