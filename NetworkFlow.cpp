/*
	Zachary Ables
	Project 5: Letter Dice
	This program will use network flow to determine if a set of dice with random letters can spell a specific word
*/

#include<iostream>
#include<vector>
#include<fstream>
#include<queue>

using namespace std;

class Node
{
	public:
		string name;
		vector<class Edge *> edges;
		Node *backedge;
};
class Edge
{
	public:
		Node *a;
		Node *b;
};

class graph
{
	public:
		void readIn(char *,char *);
		void solver();
		vector<Node *> dice;
		vector<Node *> letters;
		vector<string> words;
		void graphCreator(int j);
		Node *bfs();
		void zeroBackedge();
		Node *source;
		Node *sink;
		void destroy();
};
//this function reads in all the necessary information for the graphs
void graph::readIn(char* f1, char * f2)
{
	Node *a;
	ifstream f;
	string name;
	f.open(f1);
	while(getline(f,name))
	{
		a = new Node();
		a->name = name;
		dice.push_back(a);
	}
	f.close();

	f.open(f2);
	
	while(getline(f,name))
		words.push_back(name);
	f.close();
}
void graph::graphCreator(int j)
{
	//sets the source nodes and sink nodes
	source = new Node();
	source->name = "Source";
	sink = new Node();
	sink->name = "Sink";

	for(int i = 0; i < words[j].size();i++)
	{
		Node *b = new Node();
		b->name = words[j][i];
		letters.push_back(b);
	}
	//for each dice
	for(int i = 0; i < dice.size();i++)
	{
		Node *a = dice[i];
		//for each letter in dice
		for(int j = 0; j < a->name.size();j++)
		{
			//for each letter in the word
			for(int k = 0; k < letters.size();k++)
			{
				//if a letter on the dice = a letter on the word
				if(a->name[j] == letters[k]->name[0])
				{
					bool flag = 0;
					//for each edge on dice
					for(int l = 0; l < a->edges.size();l++)
					{
						//if from nodes edge = a letter set flag = 1
						if(a->edges[l]->b == letters[k])
							flag = 1;
					}
					//creates an edge if one does not exist yet
					if(flag != 1)
					{
						Edge *e = new Edge();
						e->a = a;
						e->b = letters[k];
						a->edges.push_back(e);
					}
				}
			}
		}
	}
	//creates an edge from the source node to each die
	for(int i = 0; i < dice.size();i++)
	{
		Edge *e = new Edge();
		e->a = source;
		e->b = dice[i];
		source->edges.push_back(e);
	}
	//creates an edge from each letter to its sink
	for(int i = 0; i < letters.size();i++)
	{
		Edge *e = new Edge();
		e->a = letters[i];
		e->b = sink;
		letters[i]->edges.push_back(e);
	}
}
//this function sets all the backedges in the graph to zero
void graph::zeroBackedge()
{
	for(int i = 0; i < letters.size();i++)
		letters[i]->backedge = NULL;

	for(int i = 0; i < dice.size();i++)
		dice[i]->backedge = NULL;

	source->backedge = NULL;
	sink->backedge = NULL;
}
Node *graph::bfs()
{
	zeroBackedge();
	queue <Node *> q;
	q.push(source);
	while(!q.empty())
	{
		//this pops off the front of the queue and saves the information in a node pointer
		Node *front = q.front();
		q.pop();

		for(int i = 0; i < front->edges.size();i++)
		{
			Node* n2 = front->edges[i]->b;		
			
			//this will push the node the edge is going to onto the queue if its not the source and it doesnt have a backedge
			if(n2->backedge == NULL && n2 != source)
			{
				q.push(n2);
				n2->backedge = front;
			}
			//will return the sink's backedge if it made it
			if(n2 == sink)
				return sink->backedge;
		}
	}
	//will return null if it doesnt make it to the end
	return sink->backedge;
}
void graph::solver()
{
	//for each word being checked with each set of dice
	for(int j = 0; j < words.size();j++)
	{
		graphCreator(j);
		while(bfs() != NULL)
		{
			//until it reaches the end of the backedges
			for(Node *n = sink; n->backedge != NULL; n = n->backedge)
			{
				//create a new edge in the reverse of the direction that the original edge had
				Edge *e = new Edge;
				e->a = n;
				e->b = n->backedge;
				n->edges.push_back(e);
				
				//finds the original edge and deletes it
				for(int i = 0; i < n->backedge->edges.size();i++)
					if(n->backedge->edges[i]->a == n->backedge && n->backedge->edges[i]->b == n)
						n->backedge->edges.erase(n->backedge->edges.begin()+i);
			}
		}
		//this is for printing
		if(sink->edges.size() >= words[j].size())
		{
			for(int i = 0; i < letters.size();i++)
			{
				for(int k = 0; k < dice.size(); k++)
				{
					if(letters[i]->edges[0]->b == dice[k])
					{
						cout << k;
						if(i <letters.size()-1)
							cout << ",";
					}
					
				}
			}
			cout <<": "<< words[j] << endl;
		}
		else
			cout << "Cannot spell "<< words[j] << endl;
		//clears the vectors so that it can run it again
		destroy();
	}
}
void graph::destroy()
{
	//destroys all edges
	for(int i = 0; i < dice.size();i++)
		dice[i]->edges.clear();
	for(int i = 0; i < letters.size(); i++)
		letters[i]->edges.clear();
	letters.clear();
	sink->edges.clear();
	source->edges.clear();
}
int main(int argc, char* argv[])
{
	graph woot;
	woot.readIn(argv[1], argv[2]);
	woot.solver();
}
