#pragma once

#include <iostream>
#include <string>
#include <vector>

class ObjImporter
{
public:
	void import_from_file(const std::string inputfile);
	std::vector<float> vertexData;
	std::vector<int> indices;
};

