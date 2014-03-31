#include "baseType.h"

#ifndef ode_h
	#define ode_h ode_h

	#ifdef NOGSL
		#include "stdOdeIntegrator.h"
		#define SUCCESS 1
		namespace conedy
		{
			typedef stdOdeIntegrator ode;
		}

	#else
		#include "gslOdeNode.h"
		#define SUCCESS GSL_SUCCESS 
		namespace conedy
		{
			typedef gslOdeNode ode;
		}

	#endif
#endif
