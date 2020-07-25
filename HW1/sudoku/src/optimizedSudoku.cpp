#include "optimizedSudoku.h"
#include <algorithm>
#include <iterator>

//#define DEBUG
using namespace sudoku;

OptimizedSudoku::OptimizedSudoku(vector<vector<int>>* input_matrix)
	: Sudoku(input_matrix)
{
	// initialize zero_position and feasible_set_matrix
	zero_position = new vector<position>();
	feasible_set_matrix = new vector<vector<bool*>>();
	degree_matrix = new vector<vector<int>>();
	degree_matrix->resize(9);
	feasible_set_matrix->resize(9);

	for (int i = 0; i < 9; i++)
	{
		(*feasible_set_matrix)[i].resize(9);
		(*degree_matrix)[i].resize(9);
		for (int j = 0; j < 9; j++)
		{
			if ((*input_matrix)[i][j] == 0)
			{
				(*feasible_set_matrix)[i][j] = get_feasible_set(i, j);

				int degree = 0;
				for (int k = 1; k < 10; k++)
				{
					if ((*feasible_set_matrix)[i][j][k]) degree++;
				}
				(*degree_matrix)[i][j] = degree;
				zero_position->push_back({ degree, {i,j} });
			}
			else (*feasible_set_matrix)[i][j] = nullptr;
		}
	}
}


OptimizedSudoku::OptimizedSudoku(OptimizedSudoku* parent, vector<vector<int>>* input_matrix, int i, int j, int num, vector<position>* zero_position, 
								vector<vector<bool*>>* feasible_set_matrix, vector<vector<int>>* degree_matrix)
	: Sudoku(parent, input_matrix, i, j, num, this->zero_pos)
{
	this->zero_position = zero_position;
	this->feasible_set_matrix = feasible_set_matrix;
	this->degree_matrix = degree_matrix;
}

// expanding using degree-heuristic
vector<OptimizedSudoku*>* OptimizedSudoku::expand()
{
	vector<OptimizedSudoku*>* expand_nodes = new vector<OptimizedSudoku*>();

	if (zero_position->size() == 0 || !forward_check() ) return expand_nodes;

	// using degree-heuristic
	sort(zero_position->begin(), zero_position->end());

	auto new_zero_pos = zero_position->back(); // struct position with lowest degree
	int i = get<0>(new_zero_pos.pos);
	int j = get<1>(new_zero_pos.pos);
	auto feasible_num_set = (*feasible_set_matrix)[i][j];

	// according to feasible_num_set (a bool[10]) to generate new nodes 
	for (int it = 1; it < 10; it++)
	{
		if (feasible_num_set[it])
		{
			OptimizedSudoku* new_node = new OptimizedSudoku(this, this->matrix, i, j, it, zero_position, feasible_set_matrix, degree_matrix);
			expand_nodes->push_back(new_node);
		}
	}
	return expand_nodes; // return new nodes 
}

void OptimizedSudoku::update()
{
	// update this->matrix and this->feasible_set_matirx
	(*matrix)[origin_i][origin_j] = num;
	(*feasible_set_matrix)[origin_i][origin_j][num] = false;

	origin_pos = zero_position->back();
	zero_position->pop_back();

	for (int k = 0; k < zero_position->size(); k++)
	{
		position* position = &(*zero_position)[k]; // »á±»sort
		auto pos = position->pos;
		int x = get<0>(pos);
		int y = get<1>(pos);

		if ((*feasible_set_matrix)[x][y][num]) // if the new added num is in feasible_set 
		{
			if ((origin_i == origin_j && x == y) ||
				(origin_i == 8 - origin_j && x == 8 - y) ||
				(x == origin_i || y == origin_j) ||
				(int(x / 3) == int(origin_i / 3) && int(y / 3) == int(origin_j / 3)))
			{
				(*degree_matrix)[x][y] -= 1;
				position->degree = (*degree_matrix)[x][y];
				(*feasible_set_matrix)[x][y][num] = false;
#ifdef DEBUG
				if (get<0>(pos) == 5 && get<1>(pos) == 3) 
					cout << "false (5,3): " << num << endl;
#endif // DEBUG
				changed_position.push_back({x,y});
			}
		}
	}
}

void OptimizedSudoku::restore()
{
	// before return false, need to restore the zero in matrix
	(*matrix)[origin_i][origin_j] = 0;
	(*feasible_set_matrix)[origin_i][origin_j][num] = true;
	zero_position->push_back(origin_pos);

	// restore feasible_set_matrix
	for (int k = 0; k < changed_position.size(); k++)
	{
		auto pos = changed_position[k];
		int x = get<0>(pos);
		int y = get<1>(pos);
		(*degree_matrix)[x][y] += 1;
#ifdef DEBUG
		if (get<0>(pos) == 5 && get<1>(pos) == 3) 
			cout << "(5,3): " << num << endl;
#endif // DEBUG		
		(*feasible_set_matrix)[x][y][num] = true;
	}

	for (int k = 0; k < zero_position->size(); k++)
	{
		auto pos = (*zero_position)[k].pos;
		(*zero_position)[k].degree = (*degree_matrix)[get<0>(pos)][get<1>(pos)];
	}
}

vector<vector<int>>* OptimizedSudoku::backtracking_search(int depth, int& node_num)
{
	node_num++;

	if (depth > 0) update();

#ifdef DEBUG
	if (1)
	{
		cout << "now at depth" << depth << endl;
		cout << *this << endl;
		cout << "====================================" << endl;
		int count = 0;
		for (int i = 1; i < 10; i++)
		{
			if ((*feasible_set_matrix)[5][3][i]) count++;
		}
		cout << count << endl;
	}
#endif // DEBUG
	auto expand_nodes = expand();
	for (int i = 0; i < (*expand_nodes).size(); i++)
	{
		auto matrix_ptr = (*expand_nodes)[i]->backtracking_search(depth + 1, node_num);
		if (matrix_ptr!= nullptr) return matrix_ptr;
		else continue;
	}
	if (zero_position->size() == 0)
	{
		cout << "depth: " << depth << ", solution found: " << endl;
		cout << *this << endl;
		return this->matrix;
	}
	
	restore();
	return nullptr;
}

bool OptimizedSudoku::forward_check()
{
	for (int i = 0; i < zero_position->size(); i++)
	{
		auto pos = (*zero_position)[i];
		if (pos.degree == 0) return false;
	}
	return true;
}