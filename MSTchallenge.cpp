// Challenge 7: Minimum Spanning Tree
//Zachary Ables
//This traverses a graph, and creates a minimum spanning tree. An MST is a tree that traverses each node on the graph at least once with the smallest weights possible.
#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<string>
using namespace std;

class Node
{
	public:
		char id;
		multimap<Node*,int> edges;
		int distance = -1;
		bool visited  = 0;
		multimap<int, Node *>::iterator mapIter;
};
void ReadIn(vector<Node*> &nodes, int numNodes);
void algo(vector<Node*> &nodes);
// Main Execution
int main(int argc, char *argv[])
{
	vector<Node*> nodes;
	int numNodes;
	int i = 0;
	while(cin >> numNodes)
	{
		if(i != 0)
			cout <<endl;
		else 
			i++;
		ReadIn(nodes,numNodes);
		algo(nodes);
		nodes.clear();
	}
	return (0);
}
//reads in values
void ReadIn(vector<Node*> &nodes, int numNodes)
{
	//reads in the number of nodes
	int edge;
	//for each row set a new node
	for(int i = 0; i < numNodes;i++)
	{
		Node *a = new Node();
		a->id = 'A'+i;
		nodes.push_back(a);
	}
	//adds the edges to the nodes
	for(int i= 0; i < numNodes;i++)
	{
		for(int j = 0; j< numNodes;j++)
		{
			cin>>edge;
			if(edge!=-1)
				nodes[i]->edges.insert(make_pair(nodes[j], edge));
		}
	}
}
void algo(vector<Node*> &nodes)
{
	multimap<int, pair<Node*, Node*> > m;
	set<Node*> nodeSet;
	set<string> edgeSet;
	Node* a = nodes[0];
	Node *b;
	//this puts Node A's edges onto the map
	for(multimap <Node *,int>::iterator mit  = a->edges.begin(); mit != a->edges.end();mit++)
		m.insert(make_pair(mit->second,make_pair(a,mit->first)));

	nodeSet.insert(a);
	int totalDistance= 0;
	while(nodeSet.size() < nodes.size())
	{
		//sets a and b to the from node and to node respectively
		a = m.begin()->second.first;
		b = m.begin()->second.second;

		int distance = m.begin()->first;
		//pops m.begin() off the map 
		m.erase(m.begin());
		if(nodeSet.find(b)==nodeSet.end())
		{
			totalDistance += distance;
			string s;
			//makes sure the correct order of things is being printed
			if(a->id < b->id)
			{
				s.push_back(a->id);
				s.push_back(b->id);
			}
			else
			{
				s.push_back(b->id);
				s.push_back(a->id);
			}
			edgeSet.insert(s);
			nodeSet.insert(b);
			multimap<Node*,int>::iterator iter;
			//inserts b's edges onto the multimap
			for(iter = b->edges.begin(); iter != b->edges.end();iter++)
				m.insert(make_pair(iter->second,make_pair(b, iter->first)));
		}
	}

	//prints the solution
	cout << totalDistance << endl;
	set<string>::iterator iter;
	for(iter = edgeSet.begin();iter!=edgeSet.end();iter++)
		cout << *iter << endl;

	//This deletes all of the pointer nodes
	for(int i = 0; i < (int)nodes.size();i++)
	{
		a = nodes[i];
		delete a;
	}
}
