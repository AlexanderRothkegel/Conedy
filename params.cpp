

#include "params.h"


using namespace boost;

namespace conedy
{
			template <typename T> networkElementType params<T>::getNodeTypeFromString(string s)
				{

					if (adress.count(s) == 0)
						throw "String does not correspond to node parameter!";
					int standardAdress = adress[s].first;
					map<networkElementType, int>::iterator ia,ie;
					ia = standardParam.begin();
					ie = standardParam.end();
					for (; ia != ie; ia++)
					{
						if (ia->second == standardAdress)
							return  ia->first;
					}
					throw "String does not correspond to node parameter!";
					return _undefinedNodeType_;
				}
template <typename T>
				void params<T>::randomizeParameter(string s, networkElementType n, function<T ()> r)   // require: Nodetype ist richtig ! 
				{
					unsigned int place = (unsigned int )adress[s].second;
					if (isStandard())
					{
						vector <T*> newVec(param[row].size());
						param.push_back(newVec); 
						//				param[Nparams].resize( param[row].size());

						for (unsigned int i = 0; i < param[row].size(); i++)
						{
							if (i != place)
								param[Nparams][i] = new T(*(param[row][i]));
							else
								param[Nparams][i] = new T(r());
						}

						row = Nparams;
						Nparams++;
						usageCounter.push_back(1);

					}
					else
					{
						//		delete param[row][place];
						*(param[row][place]) = r();
					}
				}

//			template <typename T>
//				void params<T>::randomizeParameter( string parameterString, function<double ()> r)
//				{
//					params<T>::randomizeParameter(parameterString, getNodeTypeFromString(parameterString), r);
//				}
//
				//static void initialise( void  (*callback) (string,double *))
			template <typename T>
				void params<T>::initialise( void  (*callback) (string,T *))
				{ 
					map <string, pair<int, int> >::iterator it;
					for (it = adress.begin(); it != adress.end(); it++)
					{
						(*callback)(it->first, param[it->second.first][it->second.second]);
					}
				}

				//! Funktion zum Erstellen individueller Parameter aus einer Blaupause heraus. Neuer Speicherplatz wird hier zur VerfÃ¼gung gestellt.
				//


			template <typename T>
				void params<T>::rerouteParams (vector <T>  argList)           // am Besten nur einmal aufrufen solange die Parameter noch an der Standardstelle stehen
				{
					if (argList.size() != param[row].size())
					{
						throw "Fehler. falsche Argumenten Anzahl fÃ¼r Node";

					}
					else
					{
						if (usageCounter.at(row)>1)
							usageCounter.at(row)--;
						vector <T*> newVec; param.push_back(newVec); row = Nparams;
						usageCounter.push_back(1);
						Nparams++;
						param[row].resize( argList.size());
						for (unsigned int i = 0; i < argList.size(); i++)
							param[row][i] = new T(argList[i]);

					}
				}	
				//! Definiert die Parameter vom Momentanen sheet um.
			template <typename T>
				void params<T>::setSheet (unsigned int theRow, vector <T> argList)
				{
					if (argList.size() != param[theRow].size())
					{
						throw "Fehler. falsche Argumenten Anzahl fÃ¼r Node";

					}
					else
					{
						for (unsigned int i = 0; i < argList.size(); i++)
							*param[theRow][i] = argList[i];
					}





				}



			template <typename T>
				bool params<T>::compareSheets (unsigned int sheet1, unsigned int sheet2)
				{
					if (param [sheet1].size() != param [sheet2].size())
						return 0;

					for (unsigned int i = 0 ; i < param[sheet1].size() ; i++)
					{
						if (param[sheet1][i] != param [sheet2][i])
							return 0;
					}
					return 1;

				}


				//! Setzt alle Parameter, im lokalen sheet
			template <typename T>
				void params<T>::setSheet( vector <T> argList)
				{
					if (argList.size() != param[row].size())
					{
						throw "Fehler. falsche Argumenten Anzahl fÃ¼r Node";

					}
					else
					{
						for (unsigned int i = 0; i < argList.size(); i++)
							*param[row][i] = argList[i];
					}




				}




				//		void setParams(inStream& in) { for (unsigned short i = 0; i < param[row].size(); i++) in >> param[row][i]; }

				//! obsolete ?

			template <typename T>
				void params<T>::setParam(string parameterName, T value)
				{
					if (isStandard())
					{
						vector <T> newParams;
						unsigned int toChange = adress[parameterName].second;
						for (unsigned int i = 0; i < param[row].size(); i++)
						{
							if (toChange == i)
								newParams.push_back(value);
							else
								newParams.push_back(*param[row][i]);
						}
						rerouteParams(newParams);
					}






					else 
						*param[row][adress[parameterName].second] = value;
				}

				//		virtual valarray<T>* getParams() { return &param; }




				//! Setzt Standardparameter fest fÃ¼r den nodeType theNodeType. MitÃ¼bergeben wird der Name des Parameters (Im Parser), ein Standarwert und eine laufende Nummer
			template <typename T>
				void params<T>::registerStandard(networkElementType theNodeType, string s, unsigned int paramNumber, T value) 
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

					if (paramNumber +1 > param[standardParam[theNodeType]].size())
						param[standardParam[theNodeType]].resize(paramNumber+1,0);

					adress[s] = make_pair(standardParam[theNodeType], paramNumber);
					name[ make_pair(standardParam[theNodeType], paramNumber)] = s;
					param[standardParam[theNodeType]][paramNumber] = new T (value);




				}

			template <typename T>
				void params<T>::setStandard(string name, T d) 
				{
					if (adress.count (name) != 0 )
						*param[adress[name].first][adress[name].second] = d;
					else 
						throw "setting with unknown string!";
				}
				//		static void copyStandardSheet(networkElementType 
	
	template class params <double>;
	template class params <string>;
	template class params <bool>;
	template class params <vector<double> >;


}
