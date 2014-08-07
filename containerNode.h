



#ifndef containerNode_h
#define containerNode_h containerNode_h



#include "params.h"
#include <boost/function.hpp>
#include "dynNode.h"

#include <list>


// nodes	container number

// odeNode : 	1
// mapNode :	2
// gslOdeNode: 	3
// sdeNode : 	4


namespace conedy
{



	//! Container class, which puts nodes representing dynamical systems of the same kind (e.g. ODEs) into container. The second template is the container number. Nodes in the same container have consecutive memory for their state variables. Time evolution is handled by the first node in the container. 
	template <typename T, int N>
		class containerNode : public dynNode 
	{
		protected:
			static const unsigned int initialContainerSize = 4096;
			//! static pointer to the dynamical variables of all nodes in the container
			static T * dynamicVariablesOfAllDynNodes;
			//! the number of used indices, the smallest unused index
			static unsigned int usedIndices;
			//! the current size of the container
			static unsigned int sizeOfArray; 
			//! a list of all nodes in the container
			typedef  list<containerNode<T,N> *> containerNodeList;
			static containerNodeList nodeList;

		public:
			unsigned int startPosInContainerNodeArray;

			virtual baseType getState (unsigned int component) { return dynamicVariablesOfAllDynNodes[startPosInContainerNodeArray+component]; }

			static unsigned int containerDimension () { return usedIndices;}	
			containerNode (networkElementType n ) : dynNode ( n) 	{		};
			containerNode (networkElementType n, unsigned int dim ) : dynNode ( n, dim) 	{		};


			static void clear() {usedIndices = 0;nodeList.clear();}

			virtual bool timeEvolution ()   // only the first element of the container will be called for time evolution.
			{ 
				if ( (*nodeList.begin()) == this)
					return 1; 
				else
					return 0;
			}


			containerNode ( const containerNode & c ) : dynNode ( c )
			{

			free( this->x);

			if ( usedIndices == 0 )    // first node in the container. Reserve memory according two gslOdeNode_arraySize
			{
				dynamicVariablesOfAllDynNodes = ( T* ) calloc ( initialContainerSize,sizeof ( T ) );
				sizeOfArray = initialContainerSize; 
			}


			while ( usedIndices + (&c)->dimension() > sizeOfArray )   // memory space is empty 
			{ 
				realign();   // try to fill in the wholes in the array
				if ( usedIndices + (&c)->dimension() > sizeOfArray )  // still not enough space in the array, so copy the array to a new location.
				{
					unsigned int newSize = (unsigned int) sizeOfArray * 1.5;

					T* newArray = ( T* ) calloc ( newSize ,sizeof ( T ) );
					sizeOfArray =  newSize;
					T* pointer = newArray;

					usedIndices = 0;
					typename containerNodeList::iterator it;

					for (it = nodeList.begin(); it != nodeList.end(); it++)
					{

						for (unsigned int i = 0; i < (*it)->dimension(); i++)
							pointer[i] = (*it)->x [i] ;


						(*it)->x = pointer;
						(*it)->startPosInContainerNodeArray = usedIndices;
						pointer = pointer + (*it)->dimension();
						usedIndices = usedIndices + (*it)->dimension();
					}
					free (dynamicVariablesOfAllDynNodes);
					dynamicVariablesOfAllDynNodes = newArray;
				}
			}
			this->x =  &dynamicVariablesOfAllDynNodes[usedIndices];

			nodeList.push_back ( this );
			startPosInContainerNodeArray = usedIndices;
			usedIndices += (&c)->dimension();
		}



			//! Lücken füllen und zusammenschieben
			static void realign ()   
			{
				typename containerNodeList::iterator it;

				T* pointer = dynamicVariablesOfAllDynNodes;
				unsigned int offset = 0;
				usedIndices = 0;
				for (it = nodeList.begin(); it != nodeList.end(); it++)
				{

					while (usedIndices + offset != (*it)->startPosInContainerNodeArray)
						offset++;
					for (unsigned int i = 0; i < (*it)->dimension(); i++)
					{
						(*it)->x = pointer;
						pointer[i] = pointer[i + offset];
						usedIndices ++;
					}
					pointer = pointer + (*it)->dimension();
				}
			}


			virtual ~containerNode()
			{
				typename containerNodeList::iterator it;
				for (it = nodeList.begin(); it != nodeList.end(); it++)
					if (*it == this)
					{
						nodeList.erase(it);
						break;
					}
			}




			//! clean: wird vor der Integration aufgerufen und initialisiert diverse GSL-Parameter (Art der Stufenfunktion, Schrittweite usw.)
			virtual void clean ()
			{ dynNode::clean(); realign(); }

			bool amIFirst()	 { return ( (*nodeList.begin()) == this);  }




	};





	template < class T, int N>
		T*containerNode<T, N>::dynamicVariablesOfAllDynNodes;

	template < class T, int N>
		unsigned int containerNode<T,N>::usedIndices;

	template < class T, int N>
		std::list<containerNode<T,N>*> containerNode<T,N>::nodeList;

	template < class T, int N>
		unsigned int containerNode<T,N>::sizeOfArray;

	//template < class T>
	//double containerNode<T>::time;





}







#endif
