#pragma once
#include "sudoku.h"
#include <map>

using pos = pair<int, int>;

namespace sudoku 
{
	class OptimizedSudoku : public Sudoku
	{
		struct position
		{
			int degree;
			pair<int, int> pos;
			inline bool operator<(const position& b) const
			{
				return this->degree > b.degree;
			}
		};

	public:
		OptimizedSudoku(vector<vector<int>>* input_matrix);
		OptimizedSudoku(OptimizedSudoku*, vector<vector<int>>* input_matrix, int i, int j, int num, vector<position>* zero_position, vector<vector<bool*>>* feasible_set_matrix, vector<vector<int>>* degree_matrix);
		
		vector<OptimizedSudoku*>* expand();
		//int get_degree(int i, int j);
		vector<vector<int>>* backtracking_search(int depth, int& node_num);
		bool forward_check();

		void update();
		void restore();

	private:
		vector<position>* zero_position;
		position origin_pos;
		vector < pair<int, int> > changed_position;
		vector<vector<bool*>>* feasible_set_matrix;
		vector<vector<int>>* degree_matrix;
	};
}