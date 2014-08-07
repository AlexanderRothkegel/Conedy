

#ifndef sdeNode_h
#define sdeNode_h sdeNode_h

#include <cmath>



//#include "gslNoise.h"
#include "containerNode.h"


using namespace std;

//#include "dynNode.h"
namespace conedy
{

	//! --------------------------Node for Stochastic Differential Equations------------------------ //
	class sdeNode : public containerNode<baseType, 4> {


		public:


			sdeNode ( networkElementType n, unsigned int dim) : containerNode<baseType, 4> ( n, dim ) { }

			virtual void sdgl(baseType x[], baseType  dxdt[], baseType s[], baseType dsdx []) {throw "differential equation not defined for";};

			void operator()(baseType x[], baseType  dxdt[], baseType s[], baseType dsdx []) {fullSdgl(x,dxdt,s,dsdx);} 


			static void fullSdgl (baseType *x, baseType* dxdt, baseType *s, baseType *dsdx )

			{
				list<containerNode<baseType,4>*>::iterator it;
				for ( it = nodeList.begin(); it != nodeList.end();++it)
					 ((sdeNode *)*it )->sdgl ( &x[ ( *it )->startPosInContainerNodeArray], 
						 								&dxdt[ ( *it )->startPosInContainerNodeArray] , 
														&s[ (*it)->startPosInContainerNodeArray], 
														&dsdx[ (*it)->startPosInContainerNodeArray] ) ;
			}


			



	};

}
#endif
