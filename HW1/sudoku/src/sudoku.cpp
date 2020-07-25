#include "sudoku.h"

using namespace std;
using namespace sudoku;
//#define DEBUG

Sudoku::Sudoku(vector<vector<int>>* input_matrix)
{

	this->matrix = input_matrix;
	this->parent_node = nullptr;
	this->origin_i = 0;
	this->origin_j = 0;
	this->num = (*input_matrix)[0][0];
	this->zero_pos = new vector<pair<int, int>>();
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if ((*input_matrix)[i][j] == 0)
				zero_pos->push_back({ i,j });
		}
	}
}

Sudoku::Sudoku(Sudoku* parent_node, vector<vector<int>>* input_matrix, int i, int j, int num, vector<pair<int, int>>* zero_pos)
{
	this->matrix = input_matrix;
	this->parent_node = parent_node;
	this->origin_i = i;
	this->origin_j = j;
	this->num = num;
	this->zero_pos = zero_pos;
}

vector<vector<int>>* Sudoku::backtracking_search(int depth, int& node_num)
{
	node_num++;
	// if depth > 0, then need to pop zero_pos and change matrix
	if (depth > 0)
	{
		(*matrix)[origin_i][origin_j] = num;
		zero_pos->pop_back(); 
	}
#ifdef DEBUG
	if (1)
	{
		cout << "now at depth" << depth << endl;
		cout << *this << endl;
		cout << "====================================" << endl;
	}
#endif // DEBUG
 	auto expand_nodes = expand();
	for (int i = 0; i < (*expand_nodes).size(); i++)
	{
		auto result_matrix_ptr = (*expand_nodes)[i]->backtracking_search(depth + 1, node_num);
		if (result_matrix_ptr!=nullptr) return result_matrix_ptr;
		else continue;
	}
	if (zero_pos->size() == 0)
	{
		cout << "depth: " << depth << ", solution found: " << endl;
		cout << *this << endl;
		return this->matrix;
	}

	// before return false, need to restore the zero in matrix
	(*matrix)[origin_i][origin_j] = 0;
	zero_pos->push_back({ origin_i, origin_j });
	return nullptr;
}

// expand new nodes from one node 
vector<Sudoku*>* Sudoku::expand()
{
	vector<Sudoku*>* expand_nodes = new vector<Sudoku*>();
	assert((*expand_nodes).size() == 0);
	if (zero_pos->size() == 0) return expand_nodes; 

	// O(1) time to get zero pos
	auto new_zero_pos = zero_pos->back();
	int i = get<0>(new_zero_pos);
	int j = get<1>(new_zero_pos);
	auto feasible_num_set = get_feasible_set(i, j);
	for (int it = 1; it < 10; it++)
	{
		if (feasible_num_set[it])
		{
			Sudoku* new_node = new Sudoku(this,this->matrix, i, j, it, zero_pos);
			(*expand_nodes).push_back(new_node);
		}
	}
	return expand_nodes; // return here;
}

bool* Sudoku::get_feasible_set(int i, int j)
{
	bool* feasible_num_set = new bool[10];
	feasible_num_set[0] = false;
	for (int i = 1; i < 10; i++)
	{
		feasible_num_set[i] = true;
	}

	for (int k = 0; k < 9; k++)
	{
		if (i == j) // check diagonal line
		{
			if ((*matrix)[k][k] != 0) feasible_num_set[(*matrix)[k][k]] = false;
		}
		if (i == 8 - j) // check secondary diagonal line 
		{ 
			if ((*matrix)[k][8-k] != 0) feasible_num_set[(*matrix)[k][8-k]] = false;
		}
		if ((*matrix)[k][j] != 0) feasible_num_set[(*matrix)[k][j]] = false;
		if ((*matrix)[i][k] != 0) feasible_num_set[(*matrix)[i][k]] = false;
	}
	// check sub-block
	int sub_block_i = floor(i / 3) * 3; // e.g. 0,1,2-->0; 3,4,5--> 3; 6,7,8-->6
	int sub_block_j = floor(j / 3) * 3;
	for (int m = 0; m < 3; m++)
	{
		for (int n = 0; n < 3; n++)
		{
			if ((*matrix)[sub_block_i + m][sub_block_j + n] != 0)
				feasible_num_set[(*matrix)[sub_block_i + m][sub_block_j + n]] = false;
		}
	}
	
	return feasible_num_set;
}


ostream& sudoku::operator<< (ostream& out, const Sudoku& s)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if ((*(s.matrix))[i][j] == 0) out << "* ";
			else  out << (*(s.matrix))[i][j] << " ";
		}
		out << endl;
	}
	return out;
}