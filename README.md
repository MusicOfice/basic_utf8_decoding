# basic_utf8_decoding
Reads a UTF8 encoding file or C++ string and turns unicode characters to" U+ Hexvalue" 

# How to download
This project only contains a header and C++ file download files and add it to your project

# How to use
Header file includes one class. All functions get used with this class.

**Creating class**
```cpp
utf8read test;
```

There are few function can get called.


1- **std::string stringIn(std::string)**

Takes a std::string as input and returns a std::string. Assumes input string in utf8.
    
```cpp
test..stringIn("a∎ text with❼ random ⟕ unicode characters ⚅");
```
this function will return "a U+220E  text with U+277C  random  U+27D5  unicode characters  U+2685".


2- **std::string fileIn(std::string filePath)**

Tries to open and read file that given path. Assumes file in utf8.

```cpp
test.fileIn("Example/path);
```
	
if file have "☿ asl 𐍈 afa " function will return " U+263F  asl  U+10348  afa" as result.


3- **void removeUnicode(bool)**

If set true utf8read class will ignore unicode data.
	
```cpp
test.removeUnicode(true);
test.stringIn("a∎ text with❼ random ⟕ unicode characters ⚅");
```
	
this code will return "a text with random  unicode characters".


4- **void errChange(Args... args)**

Change error writing of the error types. Can accept USINGERROR. FILEERROR, INTERNALERROR and UNDEFINEDDATA. For more information look [Link Text](#error-messages)
    
```cpp
test.errChange(USINGERROR, FILEERROR);
test.setOutput(BOTH);
test.fileIn("nonExistinsgFile");
 ```
wthout errChange() fumction this program will throw "Using error unicode:You can't use setOutput(enum) for BOTH or OUTFILE, use setOutput(enum, string) to set an output file"
and "File error unicode: Couldn't opened file: nonExistinsgFile" errors. 
But because of with test.errChange(USINGERROR, FILEERROR) we silinced both use and file errors([Link Text](#error-messages)) program couldn't throw any error. 

## Basic use example
```cpp
    int main(){
        utf8read test;  // create class
        std::cout << test.fileIn("files/testFile") << std::endl;  // read "files/tesFile" and write it to another file
        test.removeUnicode(true);  // class will remove unicode untlill told otherwise
        std::cout << test.stringIn("a∎ text with❼ random ⟕ unicode characters ⚅") << std::endl;  // read string and remove unicode
        return 0;
    }
```

## Error messages
Class can write some errors to stderr. This error fall under 4 cathagory
+ USINGERROR	// using class wrongly like setOutput(BOTH)
+ FILEERROR		// file couldnt found or opened
+ INTERNALERROR	// if library itself make a mistake
+ UNDEFINEDDATA	// data couldnt identified

this errors can be shutdown with using test.errChange(enum1,enum2...) // can be entered any amout of only accept enums written above
