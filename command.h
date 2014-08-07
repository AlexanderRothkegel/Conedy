#ifndef command_h
#define command_h command_h



#include "baseType.h"
#include <map>
#include <boost/function.hpp>
//#include "fullNetwork.h"
#include "node.h"

#define _baseType_ 1
#define _network_ 2
#define _node_ 3
#define _int_ 4
#define _string_ 5
#define _bool_ 6


using namespace std;

//typedef dynNode nodeBlueprint;
//typedef edgeVirtual edgeBlueprint;

void emptyFunction();

namespace conedy {

class networkTemplate;

}
// is called by conedyCondor instead of network functions



typedef conedy::dynNode nodeBlueprint;
typedef conedy::edgeVirtual edgeBlueprint;
typedef conedy::networkTemplate networkTemplate; 




//! Alle Befehle erben von Command. Außerdem werden hier die Variablen gehandhabt.
class command
{
	protected:

		//! statische Zuordnung von Strings zum Type, der in einem Conedyskript deklariert wurde
		static map < string, int > varType;

		static map < string, baseType* > baseTypeVar;
		static map < string, bool* > boolVar;
		static map < string, int* > intVar;
		static map < string, networkTemplate * > networkVar;
		static map < string, nodeBlueprint ** > nodeVar;
		static map < string, edgeBlueprint ** > edgeVar;
		static map < string, string * > stringVar;
		static map < string, conedy::nodeDescriptor*> nodeDescriptorVar;

		static vector <string> inputFiles;


		static bool contextCheck (string s, int type)
		{ if ((varType.count(s) == 0 )|| (varType[s] != type)) return 0; else return 1;} // Überprüft, ob s als Variablenname vom Type type angemeldet wurde

	public:


		static void clear ();

		static int getType(string s) {return varType[s]; }

		static void addInputFile(string s) { inputFiles.push_back(s);};
		static  int addInputFileInt(string s) { inputFiles.push_back(s);  return 0;};


		//! delete all variables in the interpreter.
		static void finalize();

		template <typename T>
			static T * retrieve(string s) { throw "retrieve-Error"; }

		static void declare(string s, vector<baseType> *d)
		{
#ifdef DEBUG
			cout << "vector<doubles> are not supproted at the moment in the interpreter." << endl;
#endif
			//throw "vector<doubles> sind im interpreter atm not implemented!";
		}

		static void declare(string s, bool *d)
		{
			if (varType.count(s) != 0)
			{
				cout << "String:" << s << endl;
				cout << "VarType:" << varType[s] << endl;
				throw "declared twice.";
			}
			varType[s] = _bool_;
			boolVar[s] = d;
		}

		static void declare(string s, int *d)
		{
			if (varType.count(s) != 0)
			{
				cout << "String:" << s << endl;
				cout << "VarType:" << varType[s] << endl;
				throw "declared twice.";
			}
			varType[s] = _int_;
			intVar[s] = d;
		}

		static void declare(string s, baseType *d)
		{
			if (varType.count(s) != 0)
			{
				cout << "String:" << s << endl;
				cout << "VarType:" << varType[s] << endl;
				throw "declared twice.";
			}
			varType[s] = _baseType_;
			baseTypeVar[s] = d;
		}

		static void declare(string s, string *d)
		{
			if (varType.count(s) != 0)
			{
				cout << "String:" << s << endl;
				cout << "VarType:" << varType[s] << endl;
				throw "declared twice.";
			}
			varType[s] = _string_;
			stringVar[s] = d;
		}
		
		static void printAll()
		{
			map <string, int>::iterator it;
			for (it = varType.begin(); it != varType.end(); it++)
				cout << it->first << " " << it->second << endl;
		}

		static void declare(string s, int type);	// Meldet s als Variablenname an ( so wie bei  "double d;")
		command() {}
		virtual ~command() {};
};

class instruction : public command
{
	public:
		virtual void execute() {};
};


//! Basis-Klasse für Ausdrücke im Parser-Baum vom Typ T


template <> baseType *  command ::retrieve<baseType> (string s);

template <> bool *  command ::retrieve<bool> (string s);
template <>  int *  command ::retrieve<int> (string s);
template <>  string *  command ::retrieve<string> (string s);
template <>  networkTemplate *  command::retrieve<networkTemplate> (string s);
template <>  nodeBlueprint**  command::retrieve<nodeBlueprint *> (string s);
template <>  edgeBlueprint**  command::retrieve<edgeBlueprint *> (string s);


#endif
