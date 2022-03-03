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
#pragma region tempRegion
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
#pragma endregion tempRegion

namespace Parser {

    //this needs to be recursive to catch states inside other states, but also has to get the data to read
    inline std::vector<datatypes*> parseFile(std::string file) {
        int index = 0;
        std::vector<std::string> myVec = {};

        //split file by delimiter/command

        return recursiveParse(myVec, index);
    }

    std::vector<datatypes*> recursiveParse(std::vector<std::string> file, int& index, int state) {
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
        return {};
    }

    void removeComments(std::vector<std::string>& project) {
        std::smatch m;
        std::regex r("(?://[[:print:]]+$)|(?:/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*/)");
        std::regex r1("^[ \t]*$");
        for (std::string &file : project) {
            while (std::regex_search(file, m, r)) file.erase(m.position(), m.length());
            while (std::regex_search(file, m, r1)) (m.position() + m.length() + 1 >= file.size()) ? file.erase(m.position()-1, 1) : file.erase(m.position(), m.length()+1);
        }
    }

    void parseProject(std::string directory) {
        //read in all source code files in the directory and all subdirectories
        std::vector<std::string> project;
        for (const auto& entry : fs::recursive_directory_iterator(directory))
            if (entry.path().extension() == ".h" || entry.path().extension() == ".cpp" || entry.path().extension() == ".hpp") {
                std::fstream inputStream(entry.path());
                if (inputStream.is_open()) {
                    std::string content((std::istreambuf_iterator<char>(inputStream)), (std::istreambuf_iterator<char>()));
                    project.push_back("#filename: " + entry.path().filename().string() + "\n" + content);
                    inputStream.close();
                }
            }
        removeComments(project);
        preprocessor(project);
        //std::vector<datatypes*> myData;
        //for (std::string file : project) {
        //    std::vector<datatypes*> tempData = parseFile(file);
        //    std::copy(tempData.begin(), tempData.end(), myData.end()); //TODO: Optimize
        //}
        //std::cout << project[0];
    }

    //option to directly pass the parsed data?
}

//TEMP (for testing)
int main() {
    Parser::parseProject("C:/Users/EthanKelly/Desktop/Code/CParser");
}