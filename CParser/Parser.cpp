//Author: E. Kelly
//Date Started: 24/02/2022
//Ver. 0.1
//Proj Description: A simple syntax parser library for C/C++

//Forewarning: I hate REGEX. Never make me touch it again

#include "Parser.h"

#include <regex>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

//TEMP (for testing)
#include <iostream>
#define FILENAME = "example.txt"

//returns the demangled type name of the variable x
//call `type_name<decltype(x)>()`
template <class T> std::string type_name() {
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own(
#ifndef _MSC_VER
        abi::__cxa_demangle(typeid(TR).name(), nullptr, nullptr, nullptr),
#else
        nullptr,
#endif
        std::free);
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value) r += " const";
    if (std::is_volatile<TR>::value) r += " volatile";
    if (std::is_lvalue_reference<T>::value) r += "&";
    else if (std::is_rvalue_reference<T>::value) r += "&&";
    return r;
}
//END TEMP

namespace Parser {

    enum parserState {
        VOID,
        NAMESPACE,
        CLASS,
        FUNCTION,
        ENUM,
        VARIABLE
    };

    void preprocessor(std::vector<std::string> files) {

    }

    //this needs to be recursive to catch states inside other states, but also has to get the data to read
    inline std::vector<datatypes> parseFile(std::string filename) {
        std::fstream inputStream(filename);
        if (inputStream.is_open()) {
            std::string content((std::istreambuf_iterator<char>(inputStream)), (std::istreambuf_iterator<char>()));
            //act on defines
            //split by command? -> push_back to vector<string>
            std::vector<std::string> file;

            std::smatch m;
            std::regex r;
            r = "^[ \t]*#[ \t]*define[ \t]+[[:graph:]]+[ \t]+[[:print:]]+$";
            std::vector<int> positions;
            while (std::regex_search(content, m, r)) positions.push_back(m.position());

            inputStream.close();
            int index = 0;
            return recursiveParse(file, index);
        }
    }

    std::vector<datatypes> recursiveParse(std::vector<std::string> file, int& index, int state = VOID) {
        //REGEX pattern matching to determine next state
        //throw out anything after regex comment match


        //state machine
        switch (state) {
        case (NAMESPACE):
            break;
        case (CLASS):
            break;
        case (FUNCTION):
            break;
        case (ENUM):
            break;
        case (VARIABLE):
            break;
        default:
            //Also includes VOID
            break;
        }
    }

    inline void outputFile(std::vector<datatypes> myData) {

    }

    void parseProject(std::string directory) {
        for (const auto& entry : fs::directory_iterator(directory)) {
            //if regex match .cpp, .h, or .hpp
            std::cout << entry.path();
            outputFile(parseFile(entry.path().string()));
        }
    }

    //option to directly pass the parsed data?
}

//TEMP (for testing)
int main() {
    std::string temp2 = "\n      #define something something&Else";
    std::smatch m;
    std::regex r;
    r = "^[ \t]*#[ \t]*define[ \t]+[[:graph:]]+[ \t]+[[:print:]]+$";
    if (std::regex_search(temp2, m, r)) std::cout << "match found\n";


}