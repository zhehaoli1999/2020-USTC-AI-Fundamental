#include "Astar.h"
#include <chrono>
#include <ctime> 
#include <fstream>
#include <sstream>

using namespace std;
using namespace astar;
int main(int argc, char* argv[])
{
	if (argc !=5 )
	{
		cout << "usage: " << argv[0] << " <input file path> <answer file path> <output file path> <0 or 1(A*: 0, IDA*:1), default: 0> " << endl;
		return	0;
	}
	ifstream data_file(argv[1]);
	ifstream answer_file(argv[2]);
	string output_fname = argv[3];

	// read data and answer 
	/*****************************************/
	string line;
	vector<vector<int>> input_matrix(7);
	vector<vector<int>> answer_matrix(7);
	for (int i = 0; i < 7; i++)
	{
		input_matrix[i] = vector<int>(7);
		answer_matrix[i] = vector<int>(7);
	}

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			answer_matrix[i][j] = -1;
			input_matrix[i][j] = -1;
		}
	}

	int i = 1;
	char tmp;
	while (getline(answer_file, line))
	{
		istringstream iss(line);
		for (int j = 1; j <= 5; j++)
		{
			iss >> answer_matrix[i][j];
			if (j < 5) iss >> tmp;
		}
		i++;
	}
	
	i = 1;
	while (getline(data_file, line))
	{
		istringstream iss(line);
		for (int j = 1; j <= 5; j++)
		{
			iss >> input_matrix[i][j];
			if (j < 5) iss >> tmp;
		}
		i++;
	}
	/*****************************************/


	Astar a(&input_matrix, &answer_matrix, output_fname);
	
	cout << *a.root_node; 

	auto start_t = chrono::system_clock::now();

	Node* final_node;
	if (string(argv[4]) == string("1"))
		final_node = a.Iterative_Astar_search();	
	else
		final_node = a.Astar_search();

	auto end_t = chrono::system_clock::now();

	chrono::duration<double> elapsed_seconds = end_t - start_t;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end_t);

	cout << "finished computation at " << ctime(&end_time)
		<< "elapsed time: " << elapsed_seconds.count() << "s\n";

	// write to file
	a.output(final_node);
	
}
