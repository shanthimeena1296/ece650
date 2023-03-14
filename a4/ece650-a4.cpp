// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>      // std::stringstream
#include <regex>
#include <vector>
#include <queue>
#include <cstdio>
#include <string>
#include <algorithm>
#include <memory>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"

using namespace Minisat;

// https://www.geeksforgeeks.org/shortest-path-unweighted-graph/

// Function to class graph for vert and edge
class create_graph {
  int vert;
  public:
    std::vector < int > * adj_l;
  explicit create_graph(int vert) {
    this -> vert = vert;
    this -> adj_l = new std::vector < int > [vert + 1];
  };

};
create_graph * undirected_graph;
// function to add edges
void edge_add(std::vector < int > vert_array[], int v1, int v2) {
  vert_array[v1].push_back(v2);
  vert_array[v2].push_back(v1);
}

//declaring
std::vector < int > solveVertexCover(int, std::vector <int>);
std::vector<int> find_vertexcover(int, std::vector<int> [], int, int);

//For vertex cover solving
std::vector < int > solveVertexCover(int vertex, std::vector <int> edge[], int edge_count){
    
  std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
 
  	std::vector<int> value;
	std::vector<int> outValue;

	int vect_size = vertex;
	int min = 1;
	std::vector<int> tmp; 
	tmp.push_back(-1);
    //calculating the mid value
	while (vect_size >= min) {
		int mid_value = (vect_size + min) / 2;
		value = find_vertexcover(vertex, edge, mid_value, edge_count);
	//checking whether it sat or not
	//	bool nosat = std::equal(value.begin(), value.end(), tmp.begin());
	bool nosat = -1;
	if (value.size()==1 && value.at(0)==-1)
	{
			nosat = 1;
	}
	else{
			nosat = 0;
	}
	//If not satisfy, then do below
		if (not nosat) {
			vect_size = mid_value - 1;
			outValue.clear();
			outValue = value;
		}
		else {
			min = mid_value + 1;
		}
	value.clear();
	}
	//sort the vector cover values in ascending
	sort(outValue.begin(), outValue.end());
	//Printing the final output here
	for (int i = 0; i < outValue.size(); ++i) {
		std::cout << outValue[i] << " ";
	}
	std::cout << std::endl;
	return{0};
}

//https://git.uwaterloo.ca/ece650-1221/minisat-example/-/blob/master/ece650-minisat.cpp
//using minisat for finding the vertex cover
std::vector<int> find_vertexcover(int vertex_size, std::vector<int> edge[], int mid, int edge_count) {

  std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());	

	Minisat::Lit arr[vertex_size][mid];

	for (int i = 0; i < vertex_size; i++) {
		for (int j = 0; j < mid; j++) {
			arr[i][j] = Minisat::mkLit(solver->newVar());
		}
	}
	// At least (exactly only one) one vertex is the ith vertex in the vertex cover, i in [1,k]
	for (int i = 0; i < mid; i++) {
		Minisat::vec<Minisat::Lit> clause;
		for (int j = 0; j < vertex_size; j++) {
			clause.push(arr[j][i]);
		}
		solver -> addClause(clause);
		clause.clear();

	}
	// No one vertex can appear twice in a vertex cover
	for (int m = 0; m < vertex_size; m++) {
		for (int p = 0; p < mid; p++) {
			for (int q = p + 1; q < mid; q++) {
				solver->addClause(~arr[m][p], ~arr[m][q]);

			}
		}
	}
	// No more than one vertex appears in the mth position of the vertex cover.
	for (int mth_pos = 0; mth_pos < mid; mth_pos++){
		for (int p = 0; p < vertex_size; p++) {
			for (int q = p + 1; q < vertex_size; q++) {
				solver->addClause(~arr[p][mth_pos], ~arr[q][mth_pos]);

			}
		}
	}
	// Every edge is incident to at least one vertex in the vertex cover
  for (int i = 1; i <= vertex_size; i++){	
		Minisat::vec<Minisat::Lit> clause;
		for (int l = 0; l < edge[i].size(); l++){
      		for(int m = 0; m < mid; m++){
				int src = i - 1;
				int dst = edge[i][l] - 1;
				clause.push(arr[src][m]);
				clause.push(arr[dst][m]);
			}
		  solver->addClause(clause);
		}
		clause.clear();
	}

  bool res = solver->solve();
  
	std::vector<int> vertx_data;
	if (res) {
  	for (int i = 0; i < vertex_size; i++){
		for (int j = 0; j < mid; j++){
        if(solver->modelValue(arr[i][j])==l_True)
	    {
			vertx_data.push_back(i + 1);
		}
		}
		}
		return vertx_data;
	}
	else {
			std::vector<int> temp; temp.push_back(-1);		
  	return temp;
	}
}

int main() {
int v =-1;
  //read from stdin until EOF
  while (!std::cin.eof()) {
    std::string line;
    std::getline(std::cin, line);

    if (line.empty()) {
      continue;
    }  
    try{
      int edge_count = 0;
      if (line.find('V') == 0) {
        v = std::stoi(line.substr(2));
        delete undirected_graph;
        undirected_graph = new create_graph(v);    
      }
      if (line.find('E') == 0) {
        std::size_t spec_char = line.find('<');
        int flag = 0;
        while (spec_char != std::string::npos) {
          std::size_t spec_char1 = line.find('>', spec_char);
          std::string edge_input = line.substr(spec_char + 1, spec_char1 - spec_char - 1);
          std::size_t seperator = edge_input.find(',');
          int edge_0 = std::stoi(edge_input.substr(0, seperator));
          int edge_1 = std::stoi(edge_input.substr(seperator + 1));
          if (edge_0 > v or edge_1 > v or edge_0 < 0 or edge_1 < 0 or edge_0 == edge_1) {
            std::cout << "Error: Incorrect edge input." << std::endl;
            flag = 1;
            break;
          }
          spec_char = line.find('<', spec_char + 1);
        }
        edge_count = 0;
        if (flag == 0) {
          std::size_t spec_char2 = line.find('<');
          while (spec_char2 != std::string::npos) {
            std::size_t spec_char3 = line.find('>', spec_char2);
            std::string edge_input = line.substr(spec_char2 + 1, spec_char3 - spec_char2 - 1);
            std::size_t seperator = edge_input.find(',');
            int edge_0 = std::stoi(edge_input.substr(0, seperator));
            int edge_1 = std::stoi(edge_input.substr(seperator + 1));
            edge_add(undirected_graph -> adj_l, edge_0, edge_1);
            edge_count++;
            spec_char2 = line.find('<', spec_char2 + 1);
          }
        }
        solveVertexCover(v, undirected_graph -> adj_l, edge_count);

      }

    } catch (const std::exception & e) {
      std::cout << "Error: Invalid Input" << '\n';
    }
  }
  return 0;
}
