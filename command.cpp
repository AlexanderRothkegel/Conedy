

#include "command.h"


#include "fullNetwork.h"
#include<iostream>
#include<fstream>
#include <sstream>

//#include <unistd.h>


#define MAXTIMESTEPS 1000


//#include <direct> // for getcwd
#include <stdlib.h>  // for MAX_PATH
#include <iostream> // for cout and cin

// function to return the current working directory
// this is generally the application path

using namespace std;
using namespace conedy;


map < string, int > command::varType;
map < string, baseType* > command::baseTypeVar;
map < string, bool* > command::boolVar;
map < string, int* > command::intVar;
map < string, networkTemplate* >command::networkVar;
map < string, nodeBlueprint** >command::nodeVar;
map < string, edgeBlueprint** >command::edgeVar;


map < string, string* >command::stringVar;
vector < string > command::inputFiles;






void emptyFunction () {}



		void command::declare(string s, int type)	// Meldet s als Variablenname an ( so wie bei  "double d;")
		{

			if (varType.count(s) != 0)
			{
	//			cout << "String:" << s << endl;
	//			cout << "VarType:" << varType[s] << endl;
	//			throw "Doppelt declared.";
			}
			varType [s] = type;
			if (type == _network_)
			{
				networkTemplate n;
				networkVar[s] = new networkTemplate(n);

			}
			if (type == _node_)
			{
				dynNode **n =new dynNode*();

//				node *nod = new node();
//				n = & nod;
				nodeVar[s] = n;
			}

			if (type == _edge_)
			{
				edgeBlueprint **n =new edgeBlueprint*();

//				edge *nod = new node();
//				n = & nod;
				edgeVar[s] = n;
			}



			if (type == _string_)
			{
				stringVar[s] = new string();
			}


			if (type == _baseType_) baseTypeVar[s] = new baseType();
			if (type == _bool_) boolVar[s] = new bool();
			if (type == _int_) intVar[s] = new int();
		};

void command::clear()

{

//	node::theNodes.clear();
	containerNode<baseType,1>::clear();
	containerNode<baseType,2>::clear();
	containerNode<baseType,3>::clear();
	containerNode<baseType,4>::clear();




			map <string,networkTemplate*>::iterator itnet, ienet;
			itnet = networkVar.begin();
			ienet = networkVar.end();
			for (;itnet != ienet; itnet++)
				itnet->second->clear();



}


void command::finalize()
{

			map <string,networkTemplate*>::iterator itnet, ienet;
			itnet = networkVar.begin();
			ienet = networkVar.end();
			for (;itnet != ienet; itnet++)
				delete itnet->second;

			map <string,edgeBlueprint**>::iterator itedge, ieedge;
			itedge = edgeVar.begin();
			ieedge = edgeVar.end();
			for (;itedge != ieedge; itedge++)
				delete itedge->second;

			map <string,nodeBlueprint**>::iterator itnode, ienode;
			itnode = nodeVar.begin();
			ienode = nodeVar.end();
			for (;itnode != ienode; itnode++)
				delete itnode->second;

/*			map <string,double*>::iterator itd, ied;
			itd = doubleVar.begin();
			ied = doubleVar.end();
			for (;itd != ied; itd++)
				delete itd->second;

			map <string,int*>::iterator iti, iei;
			iti = intVar.begin();
			iei = intVar.end();
			for (;iti != iei; iti++)
				delete iti->second;

*/

		}



	template <>
 baseType *  command ::retrieve<baseType> (string s)
{
	if (!contextCheck (s, _baseType_))
		throw (s +"ContextError!").c_str();
	else
		return baseTypeVar[s];

}

template <>  bool *  command ::retrieve<bool> (string s)
{
	if (!contextCheck (s, _bool_))
		throw (s +"ContextError!").c_str();
	else
		return boolVar[s];

}

template <>  int *  command ::retrieve<int> (string s)
{
	if (!contextCheck (s, _int_))
		throw (s +"ContextError!").c_str();
	else
		return intVar[s];

}

	template <>
 string *  command ::retrieve<string> (string s)
{
	if (!contextCheck (s, _string_))
		throw (s +"ContextError!").c_str();
	else
		return stringVar[s];

}


	template <>
 networkTemplate *  command::retrieve<networkTemplate> (string s)
{
	if (!contextCheck (s, _network_))
	{
		cout << "ContextError!" << endl;
		cout << "NetworkTemplate:" << s << endl;
		exit(1);
	}
	else
		return networkVar[s];

}

	template <>
 nodeBlueprint**  command::retrieve<nodeBlueprint *> (string s)
{
	if (!contextCheck (s, _node_))
	{

		cout << "ContextError!" << endl;
		cout << "NodeTemplate:" << s << endl;
		exit(1);
	}
	else
		return nodeVar[s];

}

	template <>
 edgeBlueprint**  command::retrieve<edgeBlueprint *> (string s)
{
	if (!contextCheck (s, _edge_))
	{

		cout << "ContextError!" << endl;
		cout << "EdgeTemplate:" << s << endl;
		exit(1);
	}
	else
		return edgeVar[s];

}




//template networkTemplate* command::retrieve<networkTemplate> (string s);







