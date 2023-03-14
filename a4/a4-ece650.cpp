#include<iostream>
#include "minisat/core/Solver.h"
#include "minisat/core/SolverTypes.h"
#include<string>//for string related functions
#include<stdio.h>// io
#include<cstring>//for string related functions
#include<cstdlib>//for atoi
#include<sstream>//for input parsing
// #include <bits/stdc++.h>
#include<vector>//for vector data type
#include<fstream>//for eof
using namespace std;

//variable declarations
char command,borc;
int V,v1,v2,flage;
vector<int> VinE;
string commandstring,input;
//function declarations
int gatherin();//to read input
void findVC();//to find Vertex Cover
int main()
{
	do
	{
		flage=gatherin();
	}
	while(flage==1);
	return 1;
}
int gatherin()
{
	command=' ';
	commandstring.clear();
	input.clear();
	cin.clear();
	getline(cin,input);
	istringstream iss(input);
	iss >> command;
	iss >> commandstring;

	if(command=='V')
	{
	V=atoi(commandstring.c_str());
	VinE.clear();
	}
	if(command=='E')
	{
		istringstream iss(commandstring);
		iss>>borc;
		while(borc!='}')
		{
			iss>>borc;
			iss>>v1;
			if(v1>V-1)
				{
				cerr<<"Error1: Vertex entered in E input is invalid.\n";
				return 1;
				}
			iss>>borc;
			iss>>v2;
			if(v2>V-1)
				{
				cerr<<"Error2: Vertex entered in E input is invalid.\n";
				return 1;
				}
			iss>>borc;
			iss>>borc;
			VinE.push_back(v1);
			VinE.push_back(v2);
			
		}
	findVC();
	}
	if(cin.eof())
		return 0;
	else
		return 1;

}
void findVC()
{
	
	std::vector<int> VC;
	int low=0;
	int high = V;
	int mid;
	while(low<=high)
	{
		std::vector<std::vector<Minisat::Lit>> literals(V);		
		mid=(low+high)/2;
		Minisat::Solver solver;
		for(int i=0;i<V;i++)
		{ 
			for(int j=0;j<mid;j++)
			{
				Minisat::Lit l1= Minisat::mkLit(solver.newVar());
				literals[i].push_back(l1);
			}
		}		
		
		//At least one vertex is the ith vertex in the vertex cover:  
		for (int i= 0; i < mid; i++)
		 {
			Minisat::vec<Minisat::Lit> lr1;
			for (int v = 0; v < V; v++)
			{
				lr1.push(literals[v][i]);
			}
			solver.addClause(lr1);
			lr1.clear();
		}
			
		//No one vertex can appear twice in a vertex cover.
		for (int m= 0; m < V; m++)
		 {
			
			for (int q = 0; q < mid; q++) 
			{
							
				for(int p=0;p<q;p++)
				{
				solver.addClause(~literals[m][p],~literals[m][q]);	
				}
			}
		}
		//No more than one vertex appears in the mth position of the vertex cover.
		for (int m= 0; m < mid; m++)
		 {
			
			for (int q = 0; q < V; q++) 
			{
							
				for(int p=0;p<q;p++)
				{
					solver.addClause(~literals[p][m],~literals[q][m]);
				}
				
				
			}
		}
		
		//Every edge is incident to at least one vertex in the vertex cover.
		int El = VinE.size();
		for (int i = 0; i < El; i=i+2) 
			{
				Minisat::vec<Minisat::Lit> lr4;	
				int j=VinE[i];
				int r=VinE[i+1];			
				for(int k=0;k<mid;k++)
				{
					lr4.push(literals[j][k]);
					lr4.push(literals[r][k]);
				}
				solver.addClause(lr4);
				lr4.clear();
			}	
		
		auto sat=solver.solve();
		if (sat)
		 {
			high = mid-1;
			
			VC.clear();
			for(int i=0;i<V;i++)
			{
				for(int j=0;j<mid;j++)				
				{	
				Minisat::lbool tf = solver.modelValue(literals[i][j]);					
					if(tf == Minisat::l_True)
					{
						VC.push_back(i);
					}
				}
			} 
	    	}
		 else
 		{	
			low=mid+1;
	        }
	}
//Printing the VC:
int VCl=VC.size();
for(int i=0;i<VCl;i++)
{ std::cout <<VC[i]<<" ";
}
cout<<endl;
}
