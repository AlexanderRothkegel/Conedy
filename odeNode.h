#ifndef odeNode_h
#define odeNode_h odeNode_h

//#include "gslNoise.h"
//#include <valarray>
//#include <map>


#include "containerNode.h"

//#include "params.h"
#include "baseType.h"

using namespace std;


namespace conedy {
	//! base class for nodes representing an ODE
	class odeNode : public containerNode<baseType,1> {

		public:

			odeNode(networkElementType n) : // ngls: Anzahl der Gleichungen, NodeNumber,  h Schrittweite
				containerNode<baseType,1>(n) {}

			odeNode(networkElementType n, unsigned int dim) : // ngls: Anzahl der Gleichungen, NodeNumber,  h Schrittweite
				containerNode<baseType,1>(n, dim) {}

			virtual ~odeNode() {}

			virtual void dgl (const baseType  x[], baseType  dydx[]) = 0;

			static int fullDgl ( baseType t,const baseType y[], baseType f[], void *params );

         void operator()( const baseType x[], baseType dxdt[]) {fullDgl(dynNode::time, x , dxdt, NULL  );}

			static void fullDgl (baseType *y, baseType* f);
	};
}

#endif
