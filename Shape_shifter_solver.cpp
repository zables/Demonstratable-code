//zachary Ables
//solver
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class ShapeShifter 
{
	struct shape 
	{
		//Constructors
		shape();
		shape(const string&);
		
		//Variables
		vector< vector<bool> > grid;
		unsigned int width, height;
		string text;
	};
		
	public:
		//Constructors
		ShapeShifter();
		ShapeShifter(const vector<string>&);
		
		//Dealing with shapes
		void add_shape(const string&);
		bool apply_shape(unsigned int, unsigned int, unsigned int);
		
		//Solving functions
		bool all_one();
		bool find_solution(uint);
		void print_solution();
		
	private:
		//All shapes to be inserted into the grid
		vector<shape> shapes;
		
		//Game Grid
		vector< vector<bool> > grid;
		
		//(x, y) points for each shape when the solution is found.
		list< pair<unsigned int, unsigned int> > solution;
		
		//Game grid size
		unsigned int width, height; 
};
int main(int argc, char *argv[])
{
	bool solution_found;
	vector<string> rows;
	string line;
	string temp = "";
	
	/* Generate the rows vector with argv[1..N] */
	for(int i = 1; i < argc;i++)
		rows.push_back(argv[i]);
	/* Set up the game. Pass "rows" in via the constructor. */
	ShapeShifter game(rows);
	
	/* Set up all shapes via stdin and game.add_shape() by using getline() */
	while(getline(cin,temp))
		game.add_shape(temp);
	/* Play the game */
	solution_found = game.find_solution(0);
	game.print_solution();
	/* If solution_found is "true", print out the shapes and the coordinates of the solution */
}
ShapeShifter::shape::shape(const string& line)
{
	vector<string> temp;
	string input = "";
	text = line;
	istringstream inSS(line);
	while(inSS >> input)
		temp.push_back(input);
	height = temp.size();
	width = temp[0].size();
	grid.resize(height);
	for(int i = 0; i < height;i++)
		grid[i].resize(width);
	for(int i = 0; i < height;i++)
	{
		for(int j = 0; j < width;j++)
		{
			if(temp[i][j] == '1')
				grid[i][j] = 1;
			if(temp[i][j] == '0')
				grid[i][j] = 0;
		}
	}
	
}
ShapeShifter::ShapeShifter(const vector<string>& rows)
{
	height = rows.size();
	width = rows[0].size();
	grid.resize(height);
	for(int i = 0; i < height;i++)
		grid[i].resize(width);

	for(int i = 0; i < height;i++)
	{
		for(int j = 0;j < width;j++)
		{
			if(rows[i][j] == '1')
				grid[i][j] = 1;
			else if(rows[i][j] == '0')
				grid[i][j] = 0;
		}
	}
}
void ShapeShifter::add_shape(const string& line)
{
	shape addedShape(line);
	shapes.push_back(addedShape);
}
bool ShapeShifter::apply_shape(unsigned int shape_id,unsigned int x,unsigned int y)
{
	if((x + shapes[shape_id].width > width) || (y + shapes[shape_id].height > height))
		return false;
	for(int i = y; i < shapes[shape_id].height + y;i++)
	{
		for(int j = x; j < shapes[shape_id].width+x; j++)
		{
			if(grid[i][j] == 0 && shapes[shape_id].grid[i-y][j-x] == 1)
				grid[i][j] = 1;
			else if(grid[i][j] == 1 && shapes[shape_id].grid[i-y][j-x] == 1)
				grid[i][j] = 0;
		}
	}
	return true;
}
bool ShapeShifter::all_one()
{
	for(int i = 0; i < grid.size();i++)
	{
		for(int j = 0; j < grid[i].size();j++)
		{ 
			if(grid[i][j] == 0)
				return false;
		}
	}
	return true;
}
bool ShapeShifter::find_solution(unsigned int shape_id)
{
	for(int i = 0; i < height;i++)
	{
		for(int j = 0; j < width;j++)
		{
			bool success = apply_shape(shape_id,j,i);
			if(shape_id < shapes.size()-1 && success)
				find_solution(shape_id+1);
			if(all_one() && success)
			{
				solution.push_front(make_pair(j,i));
				return true;
			}
			apply_shape(shape_id,j,i);
		}
	}
	return false;
	
}
void ShapeShifter::print_solution()
{
	list<pair<unsigned int, unsigned int> >::iterator iter;;
	int i = 0;
	for(iter = solution.begin();iter != solution.end();iter++)
	{
		cout << shapes[i].text << " " << iter->second << " " << iter->first << endl;
		i++;
	}


}
