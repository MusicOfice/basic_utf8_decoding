#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <initializer_list>
#include <unordered_map>


enum errortypes{ // this line used in comment (line 17) be carefull when moving //
	USINGERROR,	// using class wrongly like setOutput(BOTH)
	FILEERROR,		// file couldnt found
	INTERNALERROR,	// if library itself make a mistake
	UNDEFINEDDATA	// data couldnt identified
};


class utf8read{
private:
	enum masks{
		BYTE_2,
		BYTE_3,
		BYTE_4,
		FOLLOWUPBYTE,
		UNDEFINED
	};


	masks unicodeType;					// for add() function
	unsigned int unicodeData;			// for add() function
	int strafe;							// for add() function

	char maskReturn(masks maskSelect);	// return mask for add() function
	int strafeReturn(masks firstChar);	// return strafe amount for add() function

	std::ofstream outFile;				// if file opened file pointer will stored here

	std::string returnString;			// string willl get returned from stringIn() or fileIn

	std::unordered_map<errortypes, bool> errors = {{USINGERROR, true}, {FILEERROR, true}, {INTERNALERROR, true}, {UNDEFINEDDATA, true}};	// for storing errors

	void insert(unsigned char insert);	// check is data unicode or ascii
	void add(unsigned char input);		// take unicode data and turn it into hex data

	void print(std::string print);		// take a string and send it to stdout, return string or a file

	void errPrint(std::string error, errortypes type);			// for error printing

	void errChange(std::initializer_list<errortypes> values);	// for controlling error printing

	bool willRemoveUnicode;

public:
	utf8read();

	std::string stringIn(std::string input);	// take a string and send it to insert()
	std::string fileIn(std::string filePath);	// take a file and send it to insert()

	void removeUnicode(bool value);

	template<std::same_as<errortypes>... Args>		// takes the variables a a list and send them to function
	void errChange(Args... args) {					// only use varibles that in errortypes enum(line 17)
		errChange({ args... });
	}
};

/*
1-)   take input with stringIn() and fileIn() function -> sent data to insert() function as unsigned chars
2-)   insert function takes unsigned chars and send them directly print() if they are ascii else send them to add()
2.b-) add() function take unicode chars and combine their bodies in to one and send them to add() when declared unicode ended
3-)   print() funtion takes strings from insert() and add() functions and write them to selected places by outputMethods enum(line 10)
*/