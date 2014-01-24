#ifndef stdOdeIntegrator_h
#define stdOdeIntegrator_h stdOdeIntegrator_h

#include "integrator.h"
#include "odeNode.h"
#include "networkConstants.h"
#include "globals.h"
#include <valarray>

namespace conedy {

	//! implements two simple ode solver  (euler and runge-kutta 4th order) mainly for test purposes.
	class stdOdeIntegrator : public odeNode, private globals
	{
		public:

		valarray <baseType> tmp2, dydt, dyt, dym;
		odeIntegrator *integ;

		stdOdeIntegrator (networkElementType n, unsigned int dim) : odeNode (n, dim)    {}

		stdOdeIntegrator ( const stdOdeIntegrator &b ): odeNode (b)
		{
			tmp2.resize((&b)->dimension());
			dydt.resize((&b)->dimension());
			dyt.resize((&b)->dimension());
			dym.resize((&b)->dimension());

			odeNodeTmp = ( baseType* ) calloc ( ( &b )->dimension(),sizeof ( baseType ) );

		}

		static unsigned int stepType_int;

		static void registerStandardValues()
		{
			registerGlobal<string>("odeStepType", "euler");
			registerGlobal<baseType>("odeStepSize", 0.00001);
		}

		virtual void swap()
		{
			for ( unsigned int i = 0; i < this->dimension(); i++ )
				this->x[i] = odeNodeTmp [i];
		}


//		virtual void swap(short i) { state=x[i]; }
		virtual void clean()
		{
			if (amIFirst())
			{
				string stepType = getGlobal<string>("odeStepType");
				if (stepType == "euler")
				{
					stepType_int = 0;
					integ = new euler (containerDimension() );
				}
				else if (stepType == "rk4")
				{
					stepType_int = 1;
					integ = new rk4 (containerDimension() );
				}
				else
				throw "unknown steptype for odeStepType!";
			}
		}



		virtual void evolve(baseType timeTilEvent)
		{
			unsigned int stepCount = timeTilEvent/getGlobal<baseType>("odeStepSize") + 1.0 - 1e-8;
			double dt = timeTilEvent / stepCount;

			for (unsigned int i = 0; i < stepCount; i++)
			{
				if (stepType_int)
					((rk4 *) integ)->step (dt, dynamicVariablesOfAllDynNodes, *this, containerDimension());
				else
					((euler *) integ)->step (dt, dynamicVariablesOfAllDynNodes, *this, containerDimension());
			}
		}
	};

}

#endif
