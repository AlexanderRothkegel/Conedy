#ifndef globals_h
#define globals_h globals_h

#include <map>
#include <iostream>
#include <string>
#include "baseType.h"

#include "command.h"




using namespace std;

// Maps of Types to ints for identification and to strings for error texts.

template <class T> int typeInteger();
template <class T> int typeInteger() {return -1;};

class globals
{
	private:

		string static typeString(int type)
		{
			switch ( type )
			{
				case 0:
					return "int";
				case 1:
					return "double";
				case 2:
					return "string";
				case 3:
					return "bool";
				default:
					throw "You are using a non-predefined type. To predefine this type, modify globals.h.";
			}
		}

		static map <string, void*> value;
		static map <string, int> type;

	public:

		template <typename T> static void registerGlobal(string name, T v)
		{
			type[name] = typeInteger<T>();
			value[name] = new T (v);
			command::declare(name, (T*)value[name]);
		}

		template <typename T> static T getGlobal(string name)


	
		{
			if (type[name] == typeInteger<T>())
				return * ((T*) value[name]) ;
			cerr <<  "Type mismatch: You try to get " << name	<<  " as " << typeString(typeInteger<T>()) << ". However, " << name <<	" is of type " << typeString(type[name]);
			throw "\n";
		}

		template <typename T> static T* getPointerToGlobal(string name)
		{
			if (type[name] == typeInteger<T>())
				return ((T*) value[name]) ;
			cerr <<  "Type mismatch: You try to get " << name	<<  " as " << typeString(typeInteger<T>()) << ". However, " << name <<	" is of type " << typeString(type[name]);
			throw "\n";
		}

		template <typename T> static void setGlobal(string name, T d)
		{
			if (type [name] == typeInteger<T>())
				* ((T*)value[name]) = d;
			else
			{
				cerr << "Type mismatch: " << name << " is of type " << typeString(type[name]) << ". But you try to set it as a " << typeString(typeInteger<T>());
				throw "\n";
			}
		}
};


#endif