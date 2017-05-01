#ifndef WRITE_H
#define WRITE_H 1

#include <vector>
#include <string>
#include <fstream>

template<typename T>
void write_txt(std::vector<T> v, std::string file_name) {
	std::ofstream file(file_name);
	for(auto& t : v) file << t << std::endl;
	file.close();
}

#endif
