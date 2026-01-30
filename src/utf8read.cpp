#include "utf8read.h"

//
// private parts
//
char utf8read::maskReturn(masks maskSelect){
	switch(maskSelect){
	case BYTE_2:
		return 0b00011111;
		break;
	case BYTE_3:
		return 0b00001111;
		break;
	case BYTE_4:
		return 0b00000111;
		break;

	case FOLLOWUPBYTE:
		return 0b00111111;
		break;
	default:
		errPrint("Internal error unicode: Mask couldn't found", INTERNALERROR);
		return 0x00;
	}
}

int utf8read::strafeReturn(masks firstChar){
	switch(firstChar){
	case BYTE_2:
		return 6;
		break;
	case BYTE_3:
		return 12;
		break;
	case BYTE_4:
		return 18;
		break;
	default:
		errPrint("Internal error unicode: Strafe amount couldn't found", INTERNALERROR);
		return -1;
	}
}

void utf8read::insert(unsigned char insert){
	if (insert >> 7 == 0x00){
		print(std::string(1, static_cast<char>(insert)));
	}else if(!willRemoveUnicode){
		add(insert);
	}
}

void utf8read::add(unsigned char input){
	//found size of unicode
	if(input >> 6 == 0b10){unicodeType = FOLLOWUPBYTE;}
	else if(input >> 5 == 0b110){unicodeType = BYTE_2;}
	else if(input >> 4 == 0b1110){unicodeType = BYTE_3;}
	else if(input >> 3 == 0b11110){unicodeType = BYTE_4;}
	else{ // if unsigned char couldn't identified
		std::ostringstream oss;
		oss << std::hex << std::uppercase << static_cast<int>(input);
		errPrint(std::string("Undefined data error unicode: Couldn't identify 0x") += oss.str(), UNDEFINEDDATA);
		unicodeType = UNDEFINED;
		return;
	}

	if (unicodeType == UNDEFINED){
		return;
	}

	// if unsigned char is not followup byte start a new unicode
	if(unicodeType != FOLLOWUPBYTE){
		unicodeData = 0x00;
		strafe = strafeReturn(unicodeType);
		if(strafe < 0){
			std::ostringstream oss;
			oss << std::hex << static_cast<int>(input);
			errPrint(std::string("Internal error unicode: Couldn't found strafe amount for 0x") += oss.str(), INTERNALERROR); 
			return;
		}
		unicodeData = static_cast<unsigned int>(input & maskReturn(unicodeType)) << strafe;
		strafe -= 6;
	}

	// add followup byte to unicode data 
	if(unicodeType == FOLLOWUPBYTE && strafe >= 0){
		unicodeData = unicodeData | (static_cast<unsigned int>(input & maskReturn(unicodeType)) << strafe);
		strafe -= 6;
	}

	// if unicode data full print() data
	if(strafe < 0 && unicodeData != 0x00){
		std::ostringstream oss;
		oss << " U+" << std::hex << std::uppercase << unicodeData<< " ";
		std::string result = oss.str();
		print(result);
		unicodeData = 0x00;
	}else if(strafe < 0 && unicodeData == 0x00){ // if all unicode data readed but instead of new unicode start old unicode continues
		errPrint("Undefined data error unicode: Too many FOLLOWUPBYTE", UNDEFINEDDATA);
	}
};

void utf8read::print(std::string print){
	returnString += print;
};

void utf8read::errPrint(std::string error, errortypes type){
	if (errors.find(type)->second){
		std::cerr << error << std::endl;
	}
}

//
// public parts
//
utf8read::utf8read(){
	unicodeData = 0;
	strafe = 0;
	willRemoveUnicode = false;
}


std::string utf8read::stringIn(std::string input){
	returnString.clear();
	for(unsigned char i: input){
		insert(i);
	}
	return returnString;
};

std::string utf8read::fileIn(std::string filePath){
	// open and check file
	std::ifstream file;
	std::streampos fileEnd;
	file.open(filePath, std::ios::binary | std::ios::in);
	if(!file.is_open()){ errPrint("File error unicode: Couldn't opened file: " + filePath, FILEERROR ); return "";};
	file.seekg(0, std::ios::end);
	fileEnd = file.tellg();
	file.seekg(0, std::ios::beg);

	// read everythig in file
	returnString.clear();
	while(file.tellg() < fileEnd){
		unsigned char read;
		file.read(reinterpret_cast<char*>(&read), 1);
		insert(read);
	}
	file.close();
	return returnString;
}

void utf8read::errChange(std::initializer_list<errortypes> values){
	for(errortypes i: values){
		auto temp = errors.find(i);
		if (temp != errors.end()){
			temp->second = false;
		}
	}
}

void utf8read::removeUnicode(bool input){
	willRemoveUnicode = input;
}
