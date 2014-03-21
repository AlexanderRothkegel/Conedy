#ifndef integrator_h
#define integrator_h integrator_h



#include "baseType.h"
#include "gslNoise.h"

using namespace std;


#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif


class odeIntegrator
{
	protected:
		baseType * dxdt;

	public:
		odeIntegrator (unsigned int size) {
			dxdt = (baseType *) calloc (size, sizeof(baseType));
			for (unsigned int i = 0; i < size; i++)
				dxdt[i] = 0;
		}
		virtual ~ odeIntegrator () {
			free (dxdt);
		}	

};



class sdeIntegrator
{
	protected:
		baseType *dxdt;
		baseType *s;
		baseType *dsdx;

	public:
		sdeIntegrator ( unsigned int size) {
			dxdt = (baseType *) calloc (size, sizeof(baseType));
			s = (baseType *) calloc (size, sizeof(baseType));
			dsdx = (baseType *) calloc (size, sizeof(baseType));

			for (unsigned int i = 0; i < size; i++)
			{
				s[i] = 0;
				dsdx[i] = 0;
			}

		}
		virtual ~sdeIntegrator ()	{
			free (dxdt);
			free (s);
			free (dsdx);
		}

};



class euler : public odeIntegrator
{
	public:
	euler (unsigned int size) : odeIntegrator(size) {}

	template <typename dgl>
		void step (double dt, baseType *state, dgl &func, unsigned int size)
		{
			func.dgl(state,dxdt);
			for (unsigned int i = 0; i < size; i++)
				state[i] += dt * dxdt[i];
		}
};



class rk4 : public odeIntegrator
{
	protected:
		baseType * dxdt2; baseType * dxdt3; baseType * dxdt4;
		baseType * intermediateState;; 

	public:
	rk4 (unsigned int size) : odeIntegrator(size)
		{
			dxdt2 = (baseType *) calloc (size, sizeof(baseType));
			dxdt3 = (baseType *) calloc (size, sizeof(baseType));
			dxdt4 = (baseType *) calloc (size, sizeof(baseType));
			intermediateState = (baseType *) calloc (size, sizeof(baseType));
		}
		virtual ~ rk4 () {
			free (dxdt2); free (dxdt3); free (dxdt4);
			free (intermediateState);
		}

	template <typename dgl>
		void step (const double dt, baseType * const state, dgl &func, unsigned int size)
		{
			func.dgl(state,dxdt);
			
			for (unsigned int i = 0; i < size; i++)
				intermediateState[i] = state[i] + dt * dxdt[i] / 2.0;
			func.dgl(intermediateState,dxdt2);
			
			for (unsigned int i = 0; i < size; i++)
				intermediateState[i] = state[i] + dt * dxdt2[i] / 2.0;
			func.dgl(intermediateState,dxdt3);
			
			for (unsigned int i = 0; i < size; i++)
				intermediateState[i] = state[i] + dt * dxdt3[i];
			func.dgl(intermediateState,dxdt4);

			for (unsigned int i = 0; i < size; i++)
				state[i] += dt * ( dxdt[i] + 2.0*dxdt2[i] + 2.0*dxdt3[i] + dxdt4[i] ) / 6.0;
		}
};



class rkf45 : public odeIntegrator
{
	protected:
		baseType * dxdt2; baseType * dxdt3; baseType * dxdt4; baseType * dxdt5; baseType * dxdt6;
		baseType * intermediateState;
		baseType * dy;

	public:
	rkf45 (unsigned int size) : odeIntegrator(size)
		{
			dxdt2 = (baseType *) calloc (size, sizeof(baseType));
			dxdt3 = (baseType *) calloc (size, sizeof(baseType));
			dxdt4 = (baseType *) calloc (size, sizeof(baseType));
			dxdt5 = (baseType *) calloc (size, sizeof(baseType));
			dxdt6 = (baseType *) calloc (size, sizeof(baseType));
			intermediateState = (baseType *) calloc (size, sizeof(baseType));
			dy = (baseType *) calloc (size, sizeof(baseType));
		}
		virtual ~ rkf45 () {
			free (dxdt2); free (dxdt3); free (dxdt4); free (dxdt5); free (dxdt6);
			free (intermediateState); free (dy);
		}

	template <typename dgl>
		void step (
					const double maxdt, baseType * const state, dgl &func, const unsigned int size,
					bool adaptive,
					const baseType absError = 0.0, const baseType relError = 0.0,
					baseType * const stepSize = NULL,
					baseType * const time = NULL,
					const baseType minStepSize = 0.0000001
				)
		{
			baseType maxerrorrel;
			baseType dt = maxdt;
			baseType errorrel;
			
			func.dgl(state,dxdt);
			
			do
			{
				if (adaptive)
				{
					dt = fmin(*stepSize, maxdt);
					if (maxdt - dt < minStepSize)
						dt = maxdt;
				}
				
				for (unsigned int i = 0; i < size; i++)
					intermediateState[i] = state[i]
								+ dt * dxdt[i] / 4.0;
				func.dgl(intermediateState,dxdt2);
				
				for (unsigned int i = 0; i < size; i++)
					intermediateState[i] = state[i] + dt * (
								  dxdt [i] * 3
								+ dxdt2[i] * 9) / 32.0;
				func.dgl(intermediateState,dxdt3);
				
				for (unsigned int i = 0; i < size; i++)
					intermediateState[i] = state[i] + dt * (
								  dxdt [i] * 1932
								- dxdt2[i] * 7200
								+ dxdt3[i] * 7296) / 2197.0;
				func.dgl(intermediateState,dxdt4);
				
				for (unsigned int i = 0; i < size; i++)
					intermediateState[i] = state[i] + dt *(
								  dxdt [i] * 439 / 216.0
								- dxdt2[i] * 8
								+ dxdt3[i] * 3680 / 513.
								- dxdt4[i] * 845 / 4104.0);
				func.dgl(intermediateState,dxdt5);
				
				for (unsigned int i = 0; i < size; i++)
					intermediateState[i] = state[i] + dt *(
								- dxdt [i] * 8 / 27.
								+ dxdt2[i] * 2
								- dxdt3[i] * 3544 / 2565.
								+ dxdt4[i] * 1859 / 4104.
								- dxdt5[i] * 11 / 40.);
				func.dgl(intermediateState,dxdt6);
				
				for (unsigned int i = 0; i < size; i++)
				dy[i] = dt * (
								  dxdt [i] * 25 / 216.
								+ dxdt3[i] * 1408 / 2565.
								+ dxdt4[i] * 2197 / 4104.
								- dxdt5[i] * 1 / 5.);
				
				
				if (adaptive)
				{
					maxerrorrel = 0.0;
					
					for (unsigned int i = 0; i < size; i++)
					{
						errorrel = fabs((dy[i] - dt * (
											  dxdt [i] * 16 / 135.
											+ dxdt3[i] * 6656 / 12825.
											+ dxdt4[i] * 28561 / 56430.
											- dxdt5[i] * 9 / 50.
											+ dxdt6[i] * 2 / 55.)
											)/(absError + relError * (state[i]+dy[i])));
						
						if (errorrel > maxerrorrel)
							maxerrorrel = errorrel;
					}
					
					if (maxerrorrel > 1.1)
						*stepSize *= fmax( 0.9 / pow(maxerrorrel, 0.25), 0.2 );
					else if ((maxerrorrel < 0.5) and (maxdt > *stepSize))
					{
						*stepSize *= fmax(1.0, fmin( 0.9 / pow(maxerrorrel, 0.2), 5.0 ) );
						*stepSize = fmin(maxdt, *stepSize);
					}
				}
			}
			while(adaptive and (maxerrorrel>1.1));
			
			for (unsigned int i = 0; i < size; i++)
				state[i] += dy[i];
			
			if (time != NULL)
				*time += dt;
		}
};



class eulerMaruyama : public sdeIntegrator
{
	public:
		eulerMaruyama( unsigned int size) : sdeIntegrator (size)	{}


		template <typename dgl>
			void step (double dt, baseType *state, dgl &func, unsigned int size)			{
				func.dgl (state, dxdt, s, dsdx);
				for (unsigned int i = 0; i < size ; i++)
					state[i] = state[i] + dxdt[i] * dt + s[i] * sqrt (dt) * gslNoise::getGaussian();
			}
};


class milsteinIto	: public sdeIntegrator
{
	public:
		milsteinIto( unsigned int size) : sdeIntegrator (size){}

		template <typename dgl>
			void step (double dt, baseType *state, dgl &func, unsigned int size)		{
				double W;
				func.dgl (state, dxdt, s, dsdx);
				for (unsigned int i = 0; i < size ; i++)			{
					W = gslNoise::getGaussian();
					state [i] = state [i] + dxdt[i] * dt + s[i] * sqrt ( dt) * W + 0.5 * s[i] * dsdx[i] * ( dt * W *W - dt);
				}
			}
};


class milsteinStrato	: public sdeIntegrator
{
	public:

		milsteinStrato( unsigned int size) : sdeIntegrator (size)	{}
		template <typename dgl>
			void step (double dt, baseType *state, dgl &func, unsigned int size)
			{
				double W;
				func.dgl (state, dxdt, s, dsdx);
				for (unsigned int i = 0; i < size ; i++)
				{
					W = gslNoise::getGaussian();
					state [i] = state [i] + dxdt[i] * dt + s[i] * sqrt ( dt) * W + 0.5 * s[i] * dsdx[i] * ( dt * W *W );

				}

			}
};








class strongTaylor : public sdeIntegrator
{
	baseType dW, dZ;
	baseType zeta;
	baseType dt2, sqdt, sqdt2, a10, rho;
	baseType * tmp2, * dxdt, *dyt, *dym, *dydW;


	const unsigned int approxOrder;

	public:
	strongTaylor (unsigned int size) : sdeIntegrator (size), approxOrder(20) {

		tmp2 = (baseType *) calloc (size, sizeof(baseType));	
		dxdt = (baseType *) calloc (size, sizeof(baseType));	
		dyt = (baseType *) calloc (size, sizeof(baseType));	
		dym = (baseType *) calloc (size, sizeof(baseType));	
		dydW = (baseType *) calloc (size, sizeof(baseType));	


		double r2, pi2=1.0/(2.0*pow(M_PI, 2)), tp1=0.0;
		rho=1.0/12.0;
		for (unsigned short i = 1; i <= approxOrder; i++) {
			r2 = pow(1.0/(double)i, 2.0);
			tp1 += r2;
		}
		rho   -= pi2*tp1;

	}


	template <typename dgl>
		void step (double dt, baseType *state, dgl &func, unsigned int size)	{
			dt2=dt/2.0; sqdt=sqrt(dt); sqdt2 = sqdt/2.0;

			//first step

			zeta = gslNoise::getGaussian();
			dW = sqdt*zeta;

			double r;
			// Berrechne a10 und b1
			a10=0.0;
			for (unsigned short j = 0; j < approxOrder; j++) {
				r=(double)(j+1);
				a10 -= gslNoise::getGaussian()/r;
			}
			a10 = a10*sqrt(2.0*dt)/M_PI - 2*sqrt(dt*rho)*gslNoise::getGaussian();
			dZ = 0.5*dt*(dW + a10);

			func.dgl(state, dxdt, dydW);


			for (unsigned int i = 0; i < size; i++)
				tmp2[i] = state[i] + dxdt[i]*dt + dydW[i]*sqdt;                        

			//second step

			func.dgl			(tmp2, dyt, dydW);
			for (unsigned int i = 0; i < size; i++)	                                    
				tmp2[i] = state[i] + dxdt[i]*dt - dydW[i]*sqdt;                        

			//third step

			func.dgl(tmp2, dym, dydW);
			for (unsigned int i = 0; i < size; i++)                                    
				state[i] += dydW[i]*dW + (dyt[i] - dym[i])/((baseType)2.0*sqdt)*dZ + (dyt[i] + dym[i] + (baseType)2.0*dxdt[i])/(baseType)4.0*dt;	
		}
};

#endif
