//Author: E. Kelly
//Date Started: 24/02/2022
//Ver. 0.1
//Proj Description: A simple syntax parser library for C/C++

//Forewarning: I hate REGEX. Never make me touch it again

#include "Parser.h"

#include "lib\include\Debugger.h"

#include <regex>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;


#include <iostream> //TEMP (for testing)


namespace Parser {

    //this needs to be recursive to catch states inside other states, but also has to get the data to read
    std::vector<datatypes*> parseFile(std::string file) {
        std::vector<datatypes*> myData;

        return myData;
    }

    //if you're thinking of messing with this function: don't. Just trust me
    void removeComments(std::vector<std::string>& project) {
        //MSVC has a bug with regex that makes it both possible and easy to stack overflow regex on large matches. Apparently other compilers don't have this problem.
        //I solve this by breaking up the file into chunks that fit within match to search for matches, guaranteeing they won't stack overflow. I then manually deal with
        //large matches. I clocked this in as actually *faster* than regex on the entire file in many cases.
#ifdef _MSC_VER
        std::smatch m;
        for (std::string &file : project) {
            int pos = 0, openComment = 0, saveSub = 0;
            while (pos < file.size()) {
                int sub = 0;

                int range;
                std::string temp;
                if (file.size() - pos >= 2500) { //break the file into chunks small enough to fit match (no stack overflow)
                    range = 2500;
                    temp = file.substr(pos, range);
                    std::regex_search(temp, m, std::regex("\\n(?:.(?!\\n))+$")); //limit to last newline
                    range = m.position();
                    temp = file.substr(pos, range);
                } else {
                    range = file.size() - pos + 1;
                    temp = file.substr(pos, range);
                }

                if (openComment) {
                    if (std::regex_search(temp, m, std::regex("[\\s\\S]*?\\*\\/"))) {
                        //remove everything between openComment and m.pos
                        file.erase(openComment, saveSub + m.length());
                        pos += m.position() - saveSub - m.length();
                        saveSub = openComment = 0;
                    } else {
                        saveSub += range;
                        pos += range;
                    }
                    continue;
                }

                //find whole matches
                while (std::regex_search(temp, m, std::regex("(?://[[:print:]]+$)|(?:/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*/)"))) {
                    sub += m.length();
                    temp.erase(m.position(), m.length());
                }
                file.replace(pos, range, temp);

                //find partial matches
                if (std::regex_search(temp, m, std::regex("\\/\\*[\\s\\S]*"))) {
                    openComment = pos + m.position();
                    saveSub = m.length();
                }
                pos += range - sub;
            }
            while (std::regex_search(file, m, std::regex("^[ \t]*$"))) (m.position() + m.length() + 1 >= file.size()) ? file.erase(m.position() - 1, 1) : file.erase(m.position(), m.length() + 1);
        }
#else
        std::smatch m;
        for (std::string& file : project) {
            while (std::regex_search(file, m, std::regex("(?://[[:print:]]+$)|(?:/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*/)"))) file.erase(m.position(), m.length());
            while (std::regex_search(file, m, std::regex("^[ \t]*$"))) (m.position() + m.length() + 1 >= file.size()) ? file.erase(m.position() - 1, 1) : file.erase(m.position(), m.length() + 1);
        }
#endif
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
        std::vector<datatypes*> myData;
        for (std::string file : project) {
            std::vector<datatypes*> tempData = parseFile(file);
            std::copy(tempData.begin(), tempData.end(), myData.end()); //TODO: Optimize
        }
    }

    //option to directly pass the parsed data?
}

//TEMP (for testing)

int main() {
    Parser::parseProject("C:/Users/EthanKelly/Desktop/Code/CParser");
}