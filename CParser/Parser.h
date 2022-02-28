#pragma once

#include "Structs.h"

namespace Parser {
	void preprocessor(std::vector<std::string>& project);

	inline std::vector<datatypes> parseFile(std::string filename);

	std::vector<datatypes> recursiveParse(std::vector<std::string> file, int& index, int state);

	inline void outputFile(std::vector<datatypes> myData);

	void parseProject(std::string directory);
}