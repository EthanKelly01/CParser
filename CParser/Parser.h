#pragma once

#include "Structs.h"

namespace Parser {
	inline std::vector<datatypes> parseFile(std::string filename);

	inline void outputFile(std::vector<datatypes> myData);

	void parseProject(std::string directory);
}