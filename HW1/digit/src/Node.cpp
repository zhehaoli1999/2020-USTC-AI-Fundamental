#include "Node.h"

using namespace astar;
using namespace std;

Node::Node(vector<vector<int>>* parent_num_matrix, vector<vector<int>>* answer_matrix,
	size_t* answer_hash, num_pos_map* answer_map, node_hash* searched_set)
{
	this->parent_node = nullptr; // for root node, its parent_node ptr = nullptr
	this->answer_matrix = answer_matrix;
	this->answer_hash = answer_hash;
	this->answer_map = answer_map;
	this->g = 0;
	this->num_matrix = vector<vector<int>>(7); // 5 + 2 
	this->zero_pos = vector<pos>();
	this->searched_set = searched_set;

	int count_7 = 0; // count the occurence of 7 
	for (int i = 0; i < 7; i++)
	{
		this->num_matrix[i].resize(7);
		for (int j = 0; j < 7; j++)
		{
			this->num_matrix[i][j] = (*parent_num_matrix)[i][j];
			if ((*parent_num_matrix)[i][j] == 0) 
			{
				zero_pos.push_back({ i, j });
			}
			if ((*parent_num_matrix)[i][j] == 7)
			{
				count_7++;
				if (count_7 == 2) this->seven_pos = {i,j};
			}
		}
	}
	assert(this->zero_pos.size() == 2);

	this->h = calculate_h();
}

Node::Node(Node* parent, node_hash* searched_set, vector<vector<int>>* parent_num_matrix, vector<vector<int>>* answer_matrix,
	size_t* answer_hash, num_pos_map* answer_map, int parent_g, Movement parent_mv, pos parent_seven_pos, vector<pos> parent_zero_pos)
{
	this->parent_node = parent;
	this->g = parent_g + 1;
	this->parent_mv = parent_mv;
	this->answer_matrix = answer_matrix;
	this->answer_hash = answer_hash;
	this->answer_map = answer_map;

	this->searched_set = searched_set;
	// TODO optimize
	this->num_matrix = vector<vector<int>>(7); // 5 + 2 
	for (int i = 0; i < 7; i++)
	{
		this->num_matrix[i].resize(7);
		for (int j = 0; j < 7; j++)
			this->num_matrix[i][j] = (*parent_num_matrix)[i][j];
	}

	// use parent_movement to generete new num_matrix 
	int mv_num = parent_mv.num;
	int dir = parent_mv.dir;
	
	this->seven_pos = parent_seven_pos;
	this->zero_pos = parent_zero_pos;

	//	7 7 0    0 0   0 7 7   7 7
	// 	  7 0    7 7     0 7   0 7 
	//		       7             0  
	//    (1)    (2)    (3)    (4)
	if (mv_num == 7)
	{
		int x7 = get<0>(parent_seven_pos);
		int y7 = get<1>(parent_seven_pos);

		switch (dir)
		{
		case UP: // (2) -> (4)
			this->seven_pos = { x7 - 1, y7 };
			this->zero_pos = { {get<0>(seven_pos)+1, get<1>(seven_pos)-1},
								{get<0>(seven_pos) +2, get<1>(seven_pos)} };
			break;
		case DOWN: // (4) -> (2)
			this->seven_pos = { x7 + 1, y7 };
			this->zero_pos = { {get<0>(seven_pos)-1, get<1>(seven_pos) - 1},
							{get<0>(seven_pos)-1, get<1>(seven_pos)} };
			break;
		case LEFT: // (3) -> (1)
			this->seven_pos = { x7, y7 -1 };
			this->zero_pos = { {get<0>(seven_pos), get<1>(seven_pos) + 1},
					{get<0>(seven_pos) + 1, get<1>(seven_pos)+1} };
			break;
		case RIGHT:// (1) -> (3)
			this->seven_pos = { x7, y7 + 1 };
			this->zero_pos = { {get<0>(seven_pos), get<1>(seven_pos)-2},
					{get<0>(seven_pos) + 1, get<1>(seven_pos) - 1} };
			break;
		default:
			break;
		}
		this->num_matrix[get<0>(seven_pos)][get<1>(seven_pos)] = 7;
		this->num_matrix[get<0>(seven_pos)+1][get<1>(seven_pos)] = 7;
		this->num_matrix[get<0>(seven_pos)][get<1>(seven_pos)-1] = 7;
	}
	else
	{
		int num_origin_x = get<0>(parent_mv.num_origin_pos);
		int num_origin_y = get<1>(parent_mv.num_origin_pos);

		int zero_idx = parent_mv.zero_idx;
		this->zero_pos[zero_idx] = { num_origin_x , num_origin_y };
		
		switch (dir)
		{
		case LEFT:
			this->num_matrix[num_origin_x][num_origin_y - 1] = mv_num;
			break;
		case RIGHT:
			this->num_matrix[num_origin_x][num_origin_y + 1] = mv_num;
			break;
		case UP:
			this->num_matrix[num_origin_x-1][num_origin_y] = mv_num;
			break;
		case DOWN:
			this->num_matrix[num_origin_x+1][num_origin_y] = mv_num;
			break;
		default:
			break;
		}

	}

	// update zeros in num_matrix
	this->num_matrix[get<0>(zero_pos[0])][get<1>(zero_pos[0])] = 0;
	this->num_matrix[get<0>(zero_pos[1])][get<1>(zero_pos[1])] = 0;

	// calculate heuristic 
	this->h = calculate_h();
}


vector<Node*> Node::expand()
{
	vector<Node*> expand_nodes;
	
	auto seven_mv = get_seven_move_direction();
	for (int mv_dir : seven_mv)
	{
		// assemble new nodes
		Node* new_node = new Node(this, this->searched_set, &this->num_matrix, this->answer_matrix, this->answer_hash, 
			this->answer_map, this->g, {7, this->seven_pos, 0, mv_dir}, this->seven_pos, this->zero_pos);
		if (is_node_searched(new_node) == false)
		{
			this->searched_set->insert(new_node->num_matrix);
			expand_nodes.push_back(new_node);
		}
		else delete new_node;
	}
	auto nonseven_mv = get_nonseven_movement();
	for (auto mv : nonseven_mv)
	{
		Node* new_node = new Node(this, this->searched_set, &this->num_matrix, this->answer_matrix, this->answer_hash, 
			this->answer_map, this->g, mv, this->seven_pos, this->zero_pos);
		if (is_node_searched(new_node) == false)
		{
			this->searched_set->insert(new_node->num_matrix);
			expand_nodes.push_back(new_node);
		}
		else delete new_node;
	}

	return expand_nodes;
}

vector<int> Node::get_seven_move_direction()
{
	vector<int> mv_direction;
	int x7 = get<0>(seven_pos);
	int y7 = get<1>(seven_pos);

	int x0[2] = { get<0>(zero_pos[0]),get<0>(zero_pos[1]) };
	int y0[2] = { get<1>(zero_pos[0]),get<1>(zero_pos[1]) };

//	7 7 0    0 0   0 7 7   7 7
// 	  7 0    7 7     0 7   0 7 
//		       7             0  
//    (1)    (2)    (3)    (4)

	if ( // (1)
		(x0[0] == x7 && y0[0] == y7 + 1 && x0[1] == x7 + 1 && y0[1] == y7 + 1) ||
		(x0[1] == x7 && y0[1] == y7 + 1 && x0[0] == x7 + 1 && y0[0] == y7 + 1)
		)
		mv_direction.push_back(RIGHT);
	else if  // (2)
		(
		 (x0[0] == x7-1 && y0[0] == y7-1 && x0[1] == x7-1 && y0[1] == y7) ||
		 (x0[1] == x7 - 1 && y0[1] == y7 - 1 && x0[0] == x7 - 1 && y0[0] == y7)
		)
		mv_direction.push_back(UP);
	else if // (3)
		(
		 (x0[0] == x7  && y0[0] == y7 - 2 && x0[1] == x7+1 && y0[1] == y7-1) ||
		 (x0[1] == x7  && y0[1] == y7 - 2 && x0[0] == x7+1 && y0[0] == y7-1)
		)
		mv_direction.push_back(LEFT);
	else if // (4)
		(
			(x0[0] == x7+1 && y0[0] == y7 - 1 && x0[1] == x7+2 && y0[1] == y7) ||
			(x0[1] == x7 + 1 && y0[1] == y7 - 1 && x0[0] == x7 + 2 && y0[0] == y7)
		)
		mv_direction.push_back(DOWN);

	return mv_direction;
}

vector<Node::Movement> Node::get_nonseven_movement()
{
	vector<Node::Movement> nonseven_mv;
	for (int i = 0; i < 2; i++)
	{
		int x0 = get<0>(zero_pos[i]);
		int y0 = get<1>(zero_pos[i]);

		// NOTE: in num_matrix, broundary is -1 
		// up
		auto num_to_up = num_matrix[x0 + 1][y0];
		if (num_to_up > 0 && num_to_up != 7)
		{
			nonseven_mv.push_back({ num_to_up , {x0+1, y0}, i, UP });
		}
		// down
		auto num_to_down = num_matrix[x0 - 1][y0];
		if (num_to_down > 0 && num_to_down != 7)
		{
			nonseven_mv.push_back({ num_to_down ,{x0 - 1, y0}, i,DOWN });
		}
		// left
		auto num_to_left = num_matrix[x0][y0 + 1];
		if (num_to_left > 0 && num_to_left != 7)
		{
			nonseven_mv.push_back({ num_to_left ,{x0, y0+1}, i, LEFT });
		}
		// right
		auto num_to_right = num_matrix[x0][y0 - 1];
		if (num_to_right > 0 && num_to_right != 7)
		{
			nonseven_mv.push_back({ num_to_right ,{x0, y0-1}, i, RIGHT});
		}
	}

	return nonseven_mv;
}

// TODO 
bool Node::check_goal()
{
	// if hash of num_matrix is equal to the hash of answer_matrix 
	if (get<0>(this->seven_pos) != 2 || get<1>(this->seven_pos) != 2) return false;
	for (int i = 1; i <= 5; i++)
	{
		for (int j = 1; j <= 5; j++)
		{
			if (this->num_matrix[i][j] != (*this->answer_matrix)[i][j]) return false;
		}
	}
	return true;
}

double Node::calculate_h()
{
	// use 3 * mahaton distance
	double h = 0;
	for (int i = 1; i <= 5; i++)
	{
		for (int j = 1; j <= 5; j++)
		{
			int num = this->num_matrix[i][j];
			pos answer_pos = (*this->answer_map)[num];
			h += 3 * ( abs( i - get<0>(answer_pos)) + abs(j - get<1>(answer_pos))) ;
		}
	}
	//for (int i = 1; i <= 5; i++)
	//{
	//	for (int j = 1; j <= 5; j++)
	//	{
	//		h += abs(this->num_matrix[i][j] - (*this->answer_matrix)[i][j]);
	//	}
	//}
	return h;
}

bool Node::is_node_searched(Node* node)
{
	int count = this->searched_set->count(node->num_matrix);
	if ( count == 0) return false;
	else return true;
}

ostream& astar::operator<< (ostream& out, const Node& a)
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (a.num_matrix[i][j] == 0) out << "* ";
			else  out << a.num_matrix[i][j] << " ";
		}
		out << endl;
	}
	out << "========================" << endl;
	out << "h:" << a.h << " g:" << a.g  << " f:" << a.h + a.g << endl;

	//for (int i = 0; i < 7; i++)
	//{
	//	for (int j = 0; j < 7; j++)
	//	{
	//		if ((*a.answer_matrix)[i][j] == 0) out << "* ";
	//		else  out << (*a.answer_matrix)[i][j] << " ";
	//	}
	//	out << endl;
	//}
	return out;
}