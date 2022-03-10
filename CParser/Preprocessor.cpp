#include "Preprocessor.h"

#include <assert.h> //TODO: fix this system
#include <regex>

//TEMP (testing)
#include <iostream>

namespace Parser {

    void preprocessor(std::vector<std::string>& project) {
        std::map<std::string, dependency> myTree = constructDeps(project);
        
        //for (std::pair<std::string, dependency> x: myTree) findCycles(x.second, x.second);

        for (std::string &file : project) { //handle defines
            define myDefines = findDefines(&myTree.find(file.substr(11, file.find_first_of("\n") - 11))->second);
            std::smatch(m);
            for (std::pair<std::string, std::string> x : myDefines) while (std::regex_search(file, m, std::regex("^([^#\\n]*?)\\s(" + x.first + ")\\s+?"))) file.replace(m.position() + m[1].length() + 1, m[2].length(), x.second);
        }
    }

    std::map<std::string, dependency> constructDeps(std::vector<std::string>& project) {
        std::map<std::string, dependency> myTree;
        for (std::string file : project) myTree.insert({ file.substr(11, file.find_first_of("\n") - 11), {{}, {}} });
        std::smatch m;

        for (std::string file : project) {
            std::string filename = file.substr(11, file.find_first_of("\n") - 11); //TODO: make this regex or something
            std::vector<dependency*> deps;

            while (std::regex_search(file, m, std::regex("^[ \t]*#[ \t]*include[ \t]+\"([[:graph:]]+)\""))) { //find all valid file includes in a file (not libraries)
                if (myTree.find(m[1]) != myTree.end()) deps.push_back(&myTree.find(m[1])->second);
                file.erase(m.position(), m.length());
            }
            define defines;
            while (std::regex_search(file, m, std::regex("^[ \t]*#[ \t]*define[ \t]+([[:graph:]]+)[ \t]+([[:print:]]+$)"))) { //add all defines
                defines.push_back({ m[1], m[2] }); //add each valid define in the file
                file.erase(m.position(), m.length());
            }
            myTree.find(filename)->second.fillDep(deps, defines);
        }
        return myTree;
    }

    //is this actually needed? Fix for cyclic headers
    /*void findCycles(std::map<std::string, dependency> current, std::string match) {
        for (dependency x : current.dependencies) {
            assert(x != match); //TODO: find a replacement
            findCycles(x, match);
        }
    }*/

    define findDefines(dependency* x) {
        define defines = x->defines; //immediate defines
        for (dependency* y : x->dependencies) {
            define newDefines = findDefines(y); //TODO: Optimize this
            defines.insert(defines.end(), newDefines.begin(), newDefines.end());
        }
        return defines;
    }
}