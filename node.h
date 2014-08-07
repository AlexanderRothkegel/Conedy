#ifndef node_h
#define node_h node_h

#include <iostream>
#include <vector>
#include <stdint.h>

#include "networkConstants.h"
#include "baseType.h"


namespace conedy
{
	using namespace std;


	//! identifier for nodes, determines the maximum number of nodes.
	typedef uint32_t nodeDescriptor;	
	//! node kinds correspond to different kinds of dynamical systems or to special nodes for file input and output 
	typedef uint8_t nodeKind; 
	
	const nodeKind  _inNode_ = 1 << 0;
	const nodeKind  _outNode_ = 1 << 1;
	const nodeKind  _dynNode_ = 1 << 2;
	const nodeKind  _ode_ = 1 << 3;
	const nodeKind  _sde_ = 1 << 4;
	const nodeKind  _pco_ = 1 << 5;
	const nodeKind  _pcoDelay_ = 1 << 6;
	const nodeKind  _mapNode_ = 1 << 7;


	typedef uint8_t edgeKind;   // edge kinds determine if an edge is weighted or not.

	const edgeKind	_weighted_	= 1 << 1;
	const edgeKind	_polynomial_	= 1 << 2;  


	//! info-object which is returned by all nodes.
	struct nodeInfo
	{	// a unique identifier for the node type
		networkElementType theNodeType;
		// a bit mask for booleans which specify the kind of the node. inNode | outNode | dynNode
		nodeKind  theNodeKind;
		// the node name. Printed by printNodeStatistics 
		string theNodeName;
	};

	//! info-object which is returned by all edges
	struct edgeInfo
	{	// unique identifier for the edgetype
		networkElementType theEdgeType;
		// bit mask with the node type,
		edgeKind theEdgeKind;
		// the edge name. Printed by printNodeStatistics
		string theEdgeName;
	};


	class node;
	class dynNode;


	//! base class for all edges. Has no virtual functions. Specialized edges should be defined as template, such that the template parameter is the base class. In this way both edges with and without virtual functions can use the same code.
	//! The inheritance looks as following. For static edges:
	//!	 edge -> edge1 -> edge2 -> edge...
	//! 	edge1, edge2 are here different 'features' for edges like coupling weights, edges which point to specific components of nodes, etc.
	//! 	Filter chains can also be implemeted in this way, but is not at the moment.
	//! For virtual edges:
	//		 edge -> edgeVirtual -> edge1 -> dege2 -> edge...
	//		 Class which contain the edge features edge1, edge2, ... are the same as for static edges. However, functions like getWeight, getTarget and getTargetSTate are  differently interpreted by the compiler. For the static approach they are  overloadad funtcions, for the virtual approach they are virtual functions. This allows
	class edge
	{

		public:
			edge() : targetNumber(-1) {};
			edge (nodeDescriptor t) :targetNumber ( t ) {};

			//! Return pointer to the target node.
			nodeDescriptor getTarget();// {return targetNumber;} 
			//! Returns the state of the target node.
			baseType getTargetState();
	
			void setParameter(vector < baseType > parameter)  { }
			void getParameter(vector < baseType > &parameter)  { }

			//! Return info-object for the edge.
			const edgeInfo getEdgeInfo() {edgeInfo ei = {_edge_,0, "edge"}; return ei;}

			//! print information about the edge to os. 
			ostream& printStatistics ( ostream &os, int edgeOptVerbosity, int theEdgeKind, string theEdgeName, baseType weight);

			//! Return the weight of the edge, 1 for unweighted edges.
			baseType getWeight() { return (baseType)1; }
			//! Set the weight of the edge, throw an exception for unweighted edges.
			void setWeight(baseType newWeight) { };


			//! Number of the target node. Pointer to the node is store in node::thenodes
			nodeDescriptor targetNumber;

	};




	//! base class for edges with virtual functions.
	class edgeVirtual 
	{
		public:
			nodeDescriptor targetNumber;
			virtual baseType getTargetState();

         edgeVirtual() : targetNumber(-1) { };
			edgeVirtual ( nodeDescriptor t) :targetNumber(t ) {};
			virtual void setParameter(vector < baseType > parameter)  { }
			virtual void getParameter(vector < baseType > &parameter)  { }
			virtual nodeDescriptor getTarget() { return targetNumber;}
			virtual const edgeInfo getEdgeInfo() {edgeInfo ei = {_edgeVirtual_,0, "edgeVirtual"}; return ei;}
			virtual ostream& printStatistics ( ostream &os, int edgeOptVerbosity, int theEdgeKind, string theEdgeName, baseType weight);
			virtual ostream& printStatistics ( ostream &os, int edgeOptVerbosity);
			virtual edgeVirtual *construct() { return new edgeVirtual ( *this ); };
			virtual ~edgeVirtual() {}
			virtual baseType getWeight() { return (baseType)1; }
			virtual void setWeight(baseType newWeight) { throw "Trying to set weight of unweighted edge!";}	
	};






	/*!
	  \Brief  Base class for all nodes.

	  Contains a list of edges and supplies functions like link and unlink, which change this list.
	  All these functions are only interface functions and are given two different implementations by nodeVirtualEdges and nodeTemplateEdges.
	  Also contains a vector of pointer to all nodes, which serves as lookup table. In this way edges need only memorize a 32 bit node number instead of a 64 bit pointer.

*/
	class node
	{
		public:

			//! Identifizierungs-Objekt für die Edges des Knotens XXX obsolete ?
			typedef unsigned int edgeDescriptor;
			
			//! returns the number of connections 
			virtual unsigned int degree() = 0; 

			//! returns a description object for the node. The object consists of an identifying integer, a bitmask for the node kind and a string for the node name
			virtual const nodeInfo getNodeInfo() { nodeInfo n = {_node_,0, "node"};  return n;};

			//!  Copy-constructor. Constructed nodes are inserted into the lookup table node::theNodes
			node( const node &b )	{ theNodes.push_back ( this );   number = theNodes.size() - 1;}

			//! Construktor
			node ();
	
			//! returns a copy of this node instance. All nodes which are added to networks are created by such a call. Nodes which are created by standard constructors serve as blueprints only.
			virtual node *construct()  = 0; 

			virtual ~node();

			//! return the description object for the edge.
			virtual edgeInfo getEdgeInfo (edgeDescriptor) = 0;

			//! sets the weight of the edge ed to w.
			virtual void setWeight (edgeDescriptor ed, baseType w) = 0; 

			//! returns the target of edge ed
			virtual nodeDescriptor getTarget(edgeDescriptor ed) = 0; 

			//! returns the weight of edge ed.
			virtual baseType getWeight (edgeDescriptor ed) = 0;

			//! returns the state of the target of edge ed, slow.

			//! return a blueprint for edge ed 
			virtual edgeVirtual *getEdgeBlueprint (edgeDescriptor ed)  = 0; 


			// manage connections
			
			//! removes all edges pointing to targetNumber. Warning: removing edges invalidates edgeDescriptors!
			virtual bool unlink (nodeDescriptor targetNumber) = 0; 

			//! remove edge e.
			virtual void removeEdge (edgeDescriptor e)  = 0; 

			virtual void removeEdges () = 0; 

			//! adds a link which points targetNumber with weight weight.
			virtual void link (nodeDescriptor targetNumber, baseType weight) = 0; 

			//! adds a link which is copied from l.
			virtual void link (nodeDescriptor targetNumber, edgeVirtual *l) = 0; 

			//! returns true if there is at least one link which points to target
			virtual bool isLinked ( nodeDescriptor target ) = 0; 

			//! returns the weight of a link which points to target. Returns 0 if no such link exists.
			virtual baseType linkStrength ( nodeDescriptor target ) = 0; 

			//! 
			virtual void normalizeInWeightSum(baseType d)  = 0; 

			// Statistikkram

			//! debug-function which prints all connections and their type to os, depending on nodeVerbosity and edgeVerbosity
			virtual void printStatistics(ostream &os, int nodeVerbosity, int edgeVerbosity);

			virtual void printEdgeStatistics(ostream &os, int edgeVerbosity = 1);

			//! gibt die Summe der Verbindungsgewichte aller ausgehende Verbindungen zurück TODO move to statisticNetwork.
			virtual baseType weightSum()  = 0; 

			//! gibt die Summer der Verbindungsgewichte aller eingehenden Verbindungen zurück TODO move to statisticNetwork.
			virtual baseType inWeightSum()  = 0; 

			//! gibt die Nummer im statischen Vector theNodes zurück, andessen Stelle ein Zeiger auf den  momentanen Knotens gespeichert ist.
			virtual nodeDescriptor getNumber() { return number; }

			virtual void clean () {};

		protected:
			//! Statischer Vector mit Zeigern zu allen Knoten, die mit construct erzeugt wurden.
			static vector<node* > theNodes;
		private:
			//! die Nummer vom Knoten.  theNodes[number] = this!
			nodeDescriptor number;


	};


	inline nodeDescriptor edge::getTarget() { return  targetNumber;};


	bool match (nodeDescriptor l, nodeDescriptor r); 
	bool match (nodeKind l, nodeDescriptor r) ;
	bool match (nodeDescriptor l, nodeKind r) ;
	bool match (nodeDescriptor l, networkElementType r) ;
	bool match (networkElementType l, nodeDescriptor r) ;


}



#endif
