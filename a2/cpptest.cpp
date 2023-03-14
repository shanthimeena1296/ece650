// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>      // std::stringstream
#include <regex>
#include <vector>
#include <queue>
#include <cstdio>
#include <string>


using namespace std;
// https://www.geeksforgeeks.org/shortest-path-unweighted-graph/

// Function to class graph for vert and edge
class create_graph {
  int vert;
  public:
    std::vector < int > * adj_l;
  explicit create_graph(int vert) {
    this -> vert = vert;
    this -> adj_l = new std::vector < int > [vert+1];
  };

};

create_graph * undirected_graph;

// function to add edges
void edge_add(std::vector < int > vert_array[], int v1, int v2) {
  vert_array[v1].push_back(v2);
  vert_array[v2].push_back(v1);
}

// function for BFS 
bool bfs_algo(std::vector < int > vert_arr[], int src, int dest, int v, int predec[]) {
  std::queue < int > vert_queue;
  bool visited_vertices[v];

  for (int i = 0; i <= v; i++) {
    visited_vertices[i] = false;
    predec[i] = -1;
  }

  visited_vertices[src] = true;
  vert_queue.push(src);

  while (!vert_queue.empty()) {
    int u = vert_queue.front();
    vert_queue.pop();
    for (unsigned int i = 0; i < vert_arr[u].size(); i++) {
      if (!visited_vertices[vert_arr[u][i]]) {
        visited_vertices[vert_arr[u][i]] = true;
        predec[vert_arr[u][i]] = u;
        vert_queue.push(vert_arr[u][i]);

        if (vert_arr[u][i] == dest)
          return true;
      }
    }
  }
  return false;
}

// function to calculate the shortest distance between two nodes
void cal_short_dist(vector < int > vect_a[], int start_node, int end_node, int v_s) {
  try
  {
    int predec[v_s];
    if (start_node > v_s || end_node > v_s){
      std::cout<<"Error: src or dst is greater than vertices" << std::endl;
      return;
    }
    if (start_node == end_node){
      std::cout<<start_node<<std::endl;
      return;
    }

    if (!bfs_algo(vect_a, start_node, end_node, v_s, predec)) {
      std::cout << "Error: Path doesn't exist between " << start_node << " " << end_node << std::endl;
      return;
    }
    if (bfs_algo(vect_a, start_node, end_node, v_s, predec)) {
      std::vector < int > v_path;
      int trail = end_node;
      v_path.push_back(trail);
      while (predec[trail] != -1) {
        v_path.push_back(predec[trail]);
        trail = predec[trail];
      }
      for (int i = v_path.size() - 1; i >= 0; i--) {
        if (i != 0) {
          std::cout << v_path[i] << "-";
        } else {
          std::cout << v_path[i] << endl;
        }
      }
    }
    }
    catch(const std::exception& e)
    {
      std::cout << "Error: Invalid Input" << '\n';
    }
  
}

// bool parse_input(std::string line, int v) {
//   line = std::regex_replace(line, std::regex("^ +| +$|( ) +"), "$1");
//   std::string inp = line.substr(0, 2);
//     if (inp.compare(0, 2, "V ") == 0 || inp.compare(0, 2, "E ") == 0 || inp.compare(0, 2, "s ") == 0) {
//       return true;
//     } else {
//       std::cout << "Error: Invalid Vertices input" << std::endl;
//     }
//   return true;
// }

int main() {
  //read from stdin until EOF
  while (!std::cin.eof()) {
    std::string line;
    std::getline(std::cin, line);
    int v;
    std::cout<<v << "  Debug 1"<<std::endl;
    if (line.empty()) {
      continue;
    }
    // parse_input(line, v);
    if (line.find('V') == 0) {
      v = stoi(line.substr(2));
      if(v>=1){
          std::cout<<v << "  Debug 2"<<std::endl;
          delete undirected_graph;
          undirected_graph = new create_graph(v);
      }
      else{
          std::cout<<v <<"Error: Vertices value must be greater or equal to zero"<<std::endl;
      }
    }
    if (line.find('E') == 0) {
      std::size_t spec_char = line.find('<');
      int flag = 0;
      while (spec_char != std::string::npos) {
        std::size_t spec_char1 = line.find('>', spec_char);
        std::string edge_input = line.substr(spec_char + 1, spec_char1 - spec_char - 1);
        std::size_t seperator = edge_input.find(',');
        int edge_0 = stoi(edge_input.substr(0, seperator));
        int edge_1 = stoi(edge_input.substr(seperator + 1));
        if (edge_0 > v or edge_1 > v or edge_0 < 0 or edge_1 < 0 or edge_0 == edge_1) {
          std::cout << "Error: Incorrect edge input." << std::endl;
          flag = 1;
          break;
        }
        spec_char = line.find('<', spec_char + 1);
      }
      if (flag == 0) {
        std::size_t spec_char2 = line.find('<');
        while (spec_char2 != std::string::npos) {
          std::size_t spec_char3 = line.find('>', spec_char2);
          std::string edge_input = line.substr(spec_char2 + 1, spec_char3 - spec_char2 - 1);
          std::size_t seperator = edge_input.find(',');
          int edge_0 = stoi(edge_input.substr(0, seperator));
          int edge_1 = stoi(edge_input.substr(seperator + 1));
          edge_add(undirected_graph -> adj_l, edge_0, edge_1);
          spec_char2 = line.find('<', spec_char2 + 1);
        }
      }
    }
    if (line.find('s') == 0) {
      std::size_t empty_space1 = line.find(' ');
      std::size_t empty_space2 = line.find(' ', empty_space1 + 1);
      int src = stoi(line.substr(empty_space1 + 1));
      int dest = stoi(line.substr(empty_space2 + 1));
      cal_short_dist(undirected_graph -> adj_l, src, dest, v);

    }
  }
  return 0;
}