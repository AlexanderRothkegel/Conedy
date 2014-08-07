#ifndef baseType_h
	#define baseType_h baseType_h




	#define PRECALCULATEWEIGHTSUM


	//#define CALENDARQUEUE


	#ifdef DOUBLE
		typedef double baseType;

	#elif defined LDOUBLE
		typedef long double baseType;
		#define NOGSL

	#elif defined FLOAT
		typedef float baseType;
		#define NOGSL

	#else
		#define DOUBLE
		typedef double baseType;

	#endif

#endif

