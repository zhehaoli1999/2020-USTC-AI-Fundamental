#include "Astar.h"
using namespace std;
using namespace astar;
//#define DEBUG

Astar::Astar(vector<vector<int>>* input_num_matrix, vector<vector<int>>* answer_matrix, string output_fname)
{
	this->output_fname = output_fname;
	std::hash<int> hasher;
	answer_hash = 0;
	int count_7 = 0;
	for (int i = 1; i <= 5; i++)
	{
		for (int j = 1; j <= 5; j++)
		{
			answer_hash ^= hasher((*answer_matrix)[i][j]) + 0x9e3779b9 + (answer_hash << 6) + (answer_hash >> 2);
			
			if ((*answer_matrix)[i][j] != 7) answer_map[(*answer_matrix)[i][j]] = { i, j };
			else
			{
				count_7++;
				if(count_7 == 2) answer_map[7] = { i, j };
			}
		}
	}
	// assemble root_node here
	searched_set = node_hash();
	root_node = new Node(input_num_matrix, answer_matrix, &answer_hash, &answer_map, &searched_set);
	fringe_queue = priority_queue<Node_ptr>();
}

Node* Astar::Astar_search()
{
	fringe_queue.push({ *root_node , root_node});
	int step = 0;
	while (!fringe_queue.empty())
	{
		step += 1;
		Node_ptr new_node_ptr = fringe_queue.top();
		fringe_queue.pop();

#ifdef DEBUG
		cout << new_node_ptr.node;
#endif // 

		if (new_node_ptr.node_ptr->check_goal())
		{
			// print path here 
			cout << "solution found!" << endl;
			cout << "step: " << step << endl;
			cout << *new_node_ptr.node_ptr;

			return new_node_ptr.node_ptr;
		}

		vector<Node*> expand_nodes = new_node_ptr.node_ptr->expand();
		for (Node* node : expand_nodes)
		{
			fringe_queue.push({*node, node});
		}
	}

	return nullptr;
}

Node* Astar::Iterative_Astar_search()
{
	int step = 0;
	int min_f = root_node->g + root_node->h;

	Node* final_node;
	//for(int i = 0; i < 999999999; i++)
	while(1) // NOTE: solution should exist !
	{
		final_node = f_limited_search(min_f, step);
		if (final_node != nullptr) break;
	}
	return final_node;
}

Node* Astar::f_limited_search(int& f_limit, int& step)
{
	fringe_queue = priority_queue<Node_ptr>();
	fringe_queue.push({ *root_node , root_node });
	searched_set.clear();

	int new_f_limit = -1;
	while (!fringe_queue.empty())
	{
		step += 1;
		Node_ptr new_node_ptr = fringe_queue.top();
		fringe_queue.pop();

#ifdef DEBUG
		cout << new_node_ptr.node;
#endif // DEBUG
		
		int f = new_node_ptr.node.g + new_node_ptr.node.h; // get current f 
		if (f > f_limit)
		{ 
			// update new_f_limit 
			if (new_f_limit < 0 || new_f_limit > f) new_f_limit = f;
 			continue;
		}

		if (new_node_ptr.node_ptr->check_goal())
		{
			// print path here 
			cout << "IDA* solution found!" << endl;
			cout << "step: " << step << endl;
			cout << "f limit: " << f_limit << endl;
			cout << *new_node_ptr.node_ptr;

			return new_node_ptr.node_ptr;
		}

		vector<Node*> expand_nodes = new_node_ptr.node_ptr->expand();
		for (Node* node : expand_nodes)
		{
			fringe_queue.push({ *node, node });
		}
	}

	f_limit = new_f_limit; // update f_limit 
	return nullptr;

}

void Astar::write_to_file(int& path_len, Node* node, ofstream& output_f)
{
	if (output_f.is_open())
	{
		path_len++;
		if (node->parent_node == nullptr) return;
		else
		{
			this->write_to_file(path_len, node->parent_node, output_f);
			output_f << "(" << node->parent_mv.num << ",";
			switch (node->parent_mv.dir)
			{
			case UP:
				output_f << "u);\n";
				break;
			case DOWN:
				output_f << "d);\n";
				break;
			case LEFT:
				output_f << "l);\n";
				break;
			case RIGHT:
				output_f << "r);\n";
				break;
			default:
				break;
			}
		}
	}
}

bool Astar::output(Node* final_node)
{
	// write_result
	if (final_node == nullptr)
	{
		cout << "Final node is null. Search failed." << endl;
		return false;
	}
	ofstream output_f;
	output_f.open(output_fname);
	if (output_f.is_open() == false)
	{
		cout << "cannot open output file." << endl;
		return false;
	}

	int path_len = 0;
	write_to_file(path_len, final_node, output_f);
	output_f.close();

	cout << "path_len: " << path_len << endl;
	cout << "write finished. " << endl;
	return true;
}

