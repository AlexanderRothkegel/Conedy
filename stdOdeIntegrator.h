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
		bool adaptable;
		static baseType * absError;
		static baseType * relError;
		static baseType * stepSize;
		static baseType * minStepSize;

		static void registerStandardValues()
		{
			registerGlobal<string>("odeStepType", "rkf45");
			registerGlobal<baseType>("odeRelError", 0.00001);
			registerGlobal<baseType>("odeAbsError", 0.0);
			registerGlobal<baseType>("odeStepSize", 0.001);
			registerGlobal<baseType>("odeMinStepSize", 0.0000001);
			registerGlobal<bool>("odeIsAdaptive", true);
			absError = getPointerToGlobal<baseType>("odeAbsError");
			relError = getPointerToGlobal<baseType>("odeRelError");
			stepSize = getPointerToGlobal<baseType>("odeStepSize");
			minStepSize = getPointerToGlobal<baseType>("odeMinStepSize");
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
					adaptable = false;
				}
				else if (stepType == "rk4")
				{
					stepType_int = 1;
					integ = new rk4 (containerDimension() );
					adaptable = false;
				}
				else if (stepType == "rkf45")
				{
					stepType_int = 2;
					integ = new rkf45 (containerDimension() );
					adaptable = true;
				}
				else
				throw "unknown steptype for odeStepType!";
			}
		}



		virtual void evolve(baseType timeTilEvent)
		{
			if (getGlobal<bool>("odeIsAdaptive"))
			{
				if (adaptable)
				{
					// with stepsize control
					baseType time = 0.0;
					while (time < timeTilEvent)
					{
						if (stepType_int == 2)
							((rkf45 *) integ)->step (
									timeTilEvent-time, dynamicVariablesOfAllDynNodes, *this, containerDimension(),
									true,
									*absError, *relError,
									stepSize,
									&time,
									*minStepSize
								);
						
						if (*stepSize < *minStepSize)
							throw "Stepsize crossed specified minimum (odeMinStepSize). Aborting!";
					}
				}
				else
					throw "Chosen integrator is not adaptable. Aborting!";
			}
			else
			{
				unsigned int stepCount = timeTilEvent/getGlobal<baseType>("odeStepSize") + 1.0 - 1e-8;
				double dt = timeTilEvent / stepCount;

				for (unsigned int i = 0; i < stepCount; i++)
				{
					switch (stepType_int)
					{
						case 0:
							((euler *) integ)->step (dt, dynamicVariablesOfAllDynNodes, *this, containerDimension());
							break;
						case 1:
							((rk4 *) integ)->step (dt, dynamicVariablesOfAllDynNodes, *this, containerDimension());
							break;
						case 2:
							((rkf45 *) integ)->step (dt, dynamicVariablesOfAllDynNodes, *this, containerDimension(), false);
							break;
					}
				}
			}
		}
	};
}

#endif
