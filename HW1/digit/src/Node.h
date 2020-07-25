#pragma once

#include<vector>
#include<unordered_map>
#include<iostream>
#include <cmath>
#include <cassert>
#include<unordered_set>

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

using namespace std;
using pos = pair<int, int>;

struct MatrixHash {
	size_t operator()(const vector<vector<int>>& v) const {
		std::hash<int> hasher;
		size_t seed = 0;
		for (int i = 1; i <= 5; i++) 
		{
			for (int j = 1; j <= 5; j++)
			{
				seed ^= hasher(v[i][j]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}
		}
		return seed;
	}
};

//using searched_hash = std::unordered_map<vector<vector<int>>, int, MatrixHash>;
using node_hash = std::unordered_set<vector<vector<int>>, MatrixHash>;
using num_pos_map = unordered_map<int, pos>;

namespace astar
{
	class Node
	{
	public:
		struct Movement
		{
			int num;
			pos num_origin_pos;
			int zero_idx;
			int dir; // 0:up, 1:down, 2:left, 3:right
		};

		Node(vector<vector<int>>* parent_num_matrix, vector<vector<int>>* answer_matrix, 
			size_t* answer_hash, num_pos_map* answer_map, node_hash* searched_set);
		
		Node(Node* parent, node_hash* searched_set, vector<vector<int>>* parent_num_matrix, vector<vector<int>>* answer_matrix, 
			size_t* answer_hash, num_pos_map* answer_map, int parent_g, Movement parent_mv, pos parent_seven_pos, vector<pos> parent_zero_pos);
		
		vector<Node*> expand();

		double calculate_h();

		bool check_goal();

		vector<int> get_seven_move_direction();

		vector<Movement> get_nonseven_movement();

		bool is_node_searched(Node*);

		bool operator< (const Node& b) const
		{
			return (this->g + this->h) > (b.g + b.h);
		}
		friend ostream& operator<< (ostream& out, const Node& a);

		Node* parent_node;
		Movement parent_mv;
		double h;
		int g;
	protected:
		
		vector<vector<int>> num_matrix;
		size_t* answer_hash;
		num_pos_map* answer_map;

		pos seven_pos; // seven_pos is the position of center 7
		vector<pos> zero_pos;

		vector<vector<int>>* answer_matrix;
		node_hash* searched_set;
	};
}