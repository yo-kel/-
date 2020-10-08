#ifndef FILEIOH
#define FILEIOH

#pragma once
#include "global.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <cstdarg>


int WriteToFile(std::string dir, std::string fileName, std::string content);
int ReadFromFile(std::string dir, std::string fileName, std::string & content);
int CreateDir(std::string path);
std::string SplicePathString(int count, ...);
void AllFileNames(std::string path, std::vector<std::string>& S);



#endif // !FILEIOH

