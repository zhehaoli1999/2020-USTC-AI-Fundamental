#pragma once

#include <iostream>
#include <queue>
#include <map>
#include <fstream>
#include "Node.h"

using namespace std;

namespace astar
{
	class Astar
	{
	public:
		struct Node_ptr
		{
			Node node;
			Node* node_ptr;
			inline bool operator< (const Node_ptr& n2) const
			{
				return node < n2.node;
			}
		};

		Astar(vector<vector<int>>* input_num_matrix, vector<vector<int>>* answer_matrix, 
			string output_fname);
		Node* Astar_search();

		Node* Iterative_Astar_search();
		Node* f_limited_search(int& f_limit, int& step);

		void write_to_file(int& path_len, Node* node, ofstream& output_f);
		bool output(Node* );

		Node* root_node;
	protected:
		int path_len;
		string output_fname;
		priority_queue<Node_ptr> fringe_queue;
		node_hash  searched_set;
		size_t answer_hash;
		unordered_map<int, pos> answer_map;
	};
}