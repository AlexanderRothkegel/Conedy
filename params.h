

#ifndef params_h
#define params_h params_h

#include <map>
#include "networkConstants.h"

#include <iostream>
#include <vector>
//#include "node.h"
#include <boost/function.hpp>

namespace conedy
{
	using namespace std;

	/*!
	  \Brief  Class for storing parameters.

	  Via registerStandard parameters are registered with standard values. Parameters are stored in sheets which allows nodes to share parameters to conserve memory. Each node only remembers the sheet number of its parameters.




*/


	template <typename T>
		class params
		{
			protected:
				//! Ablage für alle Parameter. Eine Node/Edge belegt eine Reihe (row) Speicherplatz
				static vector<vector<T*> > param;

				//! Anzahl der Standartparameter (Wird beim Start von Conedy hochgezählt)
				static unsigned int numberOfStandardParameterSheets;

				//! Counter for the number of nodes, which use a certain sheet. 
				static vector <unsigned int> usageCounter;

				//! Anzahl der bisher belegten Reihen
				static unsigned int Nparams;

				//! Zuordnung von "nodetype" zu Reihennummer mit Standardparametern
				static map<networkElementType, int> standardParam;  

				//! Zuordnung von string zu (standardadresse, parameternummer)
				static map <string, pair<int, int> > adress;    

				static map <pair<int,int>, string > name;



			public:
				//! The parameter sheet number of this instance. 
				unsigned int row;

				//! Prints all prameters to the console for debug reasons. 
				void printStatistics();

				static T getStandardParameter(networkElementType n, unsigned int number) {	return * param[standardParam[n]][number]; }

				// ###### KONSTRUKTOREN ######
				//! Copyconstructor: Copies the sheet-number for the new node.
				params ( const params<T> &b )
				{
					row = b.row;
					if (!isStandard())
						usageCounter.at(row)++;



				}

//				params(const unsigned int N) {  vector <T> newVec(N); param.push_back(newVec); row = Nparams; Nparams++; }

				//! Copy-Konstructor, der f�r das erstellte Objekt, Standard-Parameter verwendet (Ist das sinnvoll?) TODO
				params(networkElementType theNodeType); 


				//! Destruktor: Verringert den usageCounter um 1. Wenn usageCounter==1 wird der Parameter gelöscht.
				virtual ~params() 
				{

					if (usageCounter.at(row) == 1)
					{
						if (!isStandard())
						{
							for (unsigned int i = 0; i < param[row].size(); i++)
								delete param[row][i];


						}

						usageCounter.at(row)=0;
					}
					else if (usageCounter.at(row)>1)
						usageCounter.at(row)--;
				};


				//	virtual void registerStandardValues() { throw "ERROROROR";};

				//! Funktion zum Test, ob ein Parameter Std oder individuell ist.
				bool isStandard() { return row < numberOfStandardParameterSheets;}

				vector<T*> getParameters()	{ return param[row]; }


				/*!
				  Funktion zum Abfragen der NodeType

				  \param string s
				  \return networkElementType
				  */
				static networkElementType getNodeTypeFromString(string s);
				void randomizeParameter(string s, networkElementType n, boost::function<T ()> r);   // require: Nodetype ist richtig ! 
				static void initialise( void  (*callback) (string,T *));
				//! Funktion zum Erstellen individueller Parameter aus einer Blaupause heraus. Neuer Speicherplatz wird hier zur Verfügung gestellt.
				//


				void rerouteParams (vector <T>  argList);           // am Besten nur einmal aufrufen solange die Parameter noch an der Standardstelle stehen
				//! Definiert die Parameter vom Momentanen sheet um.
				void setSheet (unsigned int theRow, vector <T> argList);


				bool compareSheets (unsigned int sheet1, unsigned int sheet2);

				//! Setzt alle Parameter, im lokalen sheet
				void setSheet( vector <T> argList);



				//! einfacher Zugriff auf die Parameter
				void setParams(const unsigned short i, T value) { *param[row][i]=value; }

				//! obsolete ?
				T getParams(const unsigned short which)  const { return *param[row][which]; };
				void setParam(string parameterName, T value);
				T& getParam(string name) const { return *param[row][adress[name].second]; }
				T * getParamPointer (const unsigned short which) { return param[row][which];} 





				//! Setzt Standardparameter fest für den nodeType theNodeType. Mitübergeben wird der Name des Parameters (Im Parser), ein Standarwert und eine laufende Nummer
				static void registerStandard(networkElementType theNodeType, string s, unsigned int paramNumber, T value);
				static void setStandard(string name, T d); 


		};



	template <typename T>
		vector<vector<T*> >  params<T>::param;

	template <typename T>
		unsigned int params<T>::Nparams;


	template <typename T>
		unsigned int params<T>::numberOfStandardParameterSheets = 0;

	template <typename T>
		map<networkElementType, int> params<T>::standardParam;

	template <typename T>
		map <string, pair<int, int> > params<T>::adress;


	template <typename T>
		map <pair<int, int>,string > params<T>::name;


	template <typename T>
		vector <unsigned int> params<T>::usageCounter;



	template <typename T>
		void params<T>::printStatistics() 
		{	
			cout << "parameter:" << endl;
			if (isStandard())
				cout << "	standard parameter" << endl;
			//					cout << "	row: " << row << endl;
			for (unsigned int i = 0; i < param[row].size(); i++);
//				cout << "	" << name[make_pair(row, i)] << ": \t " << *param[row][i] << " " << endl;   TODO find a solution that works with vector<double>



		}

	template <typename T>
		params<T>::params(networkElementType theNodeType) 
		{ 

			if (standardParam.count(theNodeType) == 0)
			{
				vector <T*> newVec; param.push_back(newVec);
				usageCounter.push_back(0);



				standardParam[theNodeType] = Nparams;
				//usageCounter[Nparams] = 1;

				Nparams++;
				numberOfStandardParameterSheets++;

			}

			row = standardParam[theNodeType];
			// 		        usageCounter.push_back(1);
			//cout << theNodeType << " " << row << endl;
		}

}



#endif





