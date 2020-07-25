#include "sudoku.h"
#include "optimizedSudoku.h"
#include <chrono>
#include <ctime> 
#include <fstream>
#include <sstream>

using namespace std;
using namespace sudoku;


int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		cout << "usage: " << argv[0] << " <input file path> <output file path> <optimized:0 non-optimized: 1. default: optimized>" << endl;
		return	0;
	}

	// read input data from file 
	//***************************************/
	ifstream data_file(argv[1]);
	string line; 
	vector<vector<int>> input_matrix(9);
	for (int i = 0; i < 9; i++)
	{
		input_matrix[i] = vector<int>(9);
	}
	int i = 0;
	while (getline(data_file, line))
	{
		istringstream iss(line);
		for (int j = 0; j < 9; j++)
		{
			iss >> input_matrix[i][j];
		}
		i++;
 	}

	//***************************************/
	int node_num = 0;
	vector<vector<int>>* result_matrix;

	if (string(argv[3]) == string("1") ) 
	{
		Sudoku s(&input_matrix);
		cout << s << endl;
		auto start_t = chrono::system_clock::now();
		result_matrix = s.backtracking_search(0,node_num);
		auto end_t = chrono::system_clock::now();

		chrono::duration<double> elapsed_seconds = end_t - start_t;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end_t);

		cout << "finished computation at " << ctime(&end_time)
			<< "elapsed time: " << elapsed_seconds.count() << "s\n";

		cout << "searched node num:" << node_num << endl;
	}

	else
	{
		OptimizedSudoku opt_s(&input_matrix);
		cout << opt_s << endl;

		auto start_t = chrono::system_clock::now();
		result_matrix = opt_s.backtracking_search(0, node_num);
		auto end_t = chrono::system_clock::now();

		chrono::duration<double> elapsed_seconds = end_t - start_t;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end_t);

		cout << "finished computation at " << ctime(&end_time)
			<< "elapsed time: " << elapsed_seconds.count() << "s\n";

		cout << "searched node num:" << node_num << endl;
	}

	// write to file 
	/**************************/
	if (result_matrix != nullptr)
	{
		ofstream output_f;
		output_f.open(argv[2]);
		if (!output_f.is_open())
		{
			cout << "open output file failed. " << endl;
		}
		else
		{
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					output_f << (*result_matrix)[i][j] << " ";
				}
				output_f << endl;
			}
		}
		output_f.close();
		cout << "Result is written to output file. " << endl;
	}

	return 0;
}