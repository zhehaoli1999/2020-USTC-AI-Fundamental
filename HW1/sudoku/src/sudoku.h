// sudoku.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <cassert>

using namespace std;

namespace sudoku
{
	class Sudoku
	{
	public:
		Sudoku(vector<vector<int>>* input_matrix);
		Sudoku(Sudoku*, vector<vector<int>>* input_matrix, int i, int j , int num, vector<pair<int, int>>* zero_pos);

		vector<vector<int>>* backtracking_search(int depth, int& node_num);
	    vector<Sudoku*>* expand();
		friend ostream& operator<< (ostream& out, const Sudoku& s);
		bool* get_feasible_set(int i, int j);

	protected:
		Sudoku* parent_node;
		vector<vector<int>>* matrix;
		vector<pair<int, int>>* zero_pos;
		int origin_i, origin_j, num;
	};
}