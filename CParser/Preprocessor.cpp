#include "Preprocessor.h"

#include <assert.h> //TODO: fix this system
#include <regex>

//TEMP (testing)
#include <iostream>

namespace Parser {

    void preprocessor(std::vector<std::string>& project) {
        std::map<std::string, dependency> myTree;
        myTree = constructDeps(project);
        //for (std::pair<std::string, dependency> x: myTree) findCycles(x.second, x.second);

        for (std::string &file : project) {
            std::string filename = file.substr(11, file.find_first_of("\n") - 11); //TODO: make this more robust
            define myDefines = findDefines(&myTree.find(filename)->second);
            //handle defines
            /*for (std::pair<std::string, std::string> x : myDefines) {
                std::smatch m;
                std::regex r("^([^#]*)\s" + x.first + "\s"); //exclude defines and strings
                while (std::regex_search(file, m, r)) if (std::count(m[1].str().begin(), m[1].str().end(), '\"') % 2 == 0) file.replace(m.position(), m.length(), x.second);
            }*/
        }
    }

    /*void constructDeps(std::vector<std::string>& project, std::map<std::string, dependency>& myTree, std::vector<std::string>& filesDone, int index) {
        //TODO: Resolve circular dependency issues. Temp dependency?
        if (index > project.size()) {
            //std::cout << "Something went wrong.\n";
            return;
        }
        std::smatch m;
        std::regex r;

        bool fileSkipped = 0;
        for (std::string file : project) {
            bool curFileSkipped = 0;
            std::string filename = file.substr(11, file.find_first_of("\n") - 11); //TODO: make this regex or something
            if (std::find(filesDone.begin(), filesDone.end(), filename) != filesDone.end()) continue;
            //std::cout << "File started: " << filename << "\n";
            std::vector<dependency> deps;

            r = "^[ \t]*#[ \t]*include[ \t]+\"([[:graph:]]+)\""; //find all valid includes in a file
            while (std::regex_search(file, m, r)) {
                if (std::find(filesDone.begin(), filesDone.end(), m[1]) == filesDone.end()) {
                    fileSkipped = true;
                    curFileSkipped = true;
                    //std::cout << "File skipped: " << filename << "\n";
                    break;*//*
                }
                deps.push_back(myTree.find(m[1])->second);
                //std::cout << file.substr(m.position(), m.length()) << "\n";
                file.erase(m.position(), m.length());
            }
            if (!curFileSkipped) {
                define defines;
                r = "^[ \t]*#[ \t]*define[ \t]+([[:graph:]]+)[ \t]+([[:print:]]+$)"; //add all defines
                while (std::regex_search(file, m, r)) {
                    defines.push_back({ m[1], m[2] }); //add each valid define in the file
                    file.erase(m.position(), m.length());
                }

                myTree.insert({ filename, { filename, deps, defines } });
                filesDone.push_back(filename);
                //std::cout << filename << " file done\n";
            } else for (dependency x : deps) file += "#include \"" + x.filename + "\"\n";
        }
        if (fileSkipped) constructDeps(project, myTree, filesDone, index + 1);
    }*/

    std::map<std::string, dependency> constructDeps(std::vector<std::string>& project) {
        //New strat: at a blank dependency for each file, run through each file once to collect deps and defines, then add to blank file
        //Side note: holy **** was my previous solution bad

        std::map<std::string, dependency> myTree;
        for (std::string file : project) myTree.insert({ file.substr(11, file.find_first_of("\n") - 11), {{}, {}} });
        std::smatch m;
        std::regex r;

        for (std::string file : project) {
            std::string filename = file.substr(11, file.find_first_of("\n") - 11); //TODO: make this regex or something
            std::vector<dependency*> deps;

            r = "^[ \t]*#[ \t]*include[ \t]+\"([[:graph:]]+)\""; //find all valid file includes in a file (not libraries)
            while (std::regex_search(file, m, r)) {
                deps.push_back(&myTree.find(m[1])->second);
                file.erase(m.position(), m.length());
            }
            define defines;
            r = "^[ \t]*#[ \t]*define[ \t]+([[:graph:]]+)[ \t]+([[:print:]]+$)"; //add all defines
            while (std::regex_search(file, m, r)) {
                defines.push_back({ m[1], m[2] }); //add each valid define in the file
                file.erase(m.position(), m.length());
            }

            myTree.insert({ filename, { deps, defines } });
            //std::cout << filename << " file done\n";
        }
        return myTree;
    }

    //is this actually needed?
    /*void findCycles(std::map<std::string, dependency> current, std::string match) {
        for (dependency x : current.dependencies) {
            assert(x != match); //TODO: find a replacement
            findCycles(x, match);
        }
    }*/

    define findDefines(dependency* x) {
        define defines = x->defines;
        for (dependency* y : x->dependencies) {
            define newDefines = findDefines(y); //TODO: Optimize this
            defines.insert(defines.end(), newDefines.begin(), newDefines.end());
        }
        return defines;
    }
}