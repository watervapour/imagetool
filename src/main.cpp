#include <iostream>
#include <fstream>
#include <filesystem>

#include "image.h"

int main(int argc, char** argv){
	std::cout << "argc: " << argc <<"\n";
	std::filesystem::path p;
	if(argc == 1) {
		// no commandline arguments
		std::cout << "Insufficent args, using default file.\n";
		p = "testfiles/imageCAT.ppm";
	} else {
		p = argv[1];
		std::cout << "path provided! \n";
	}

	// verify file exists
	bool fileExists = std::filesystem::exists(p);
	if(!fileExists){
		std::cerr << "No file, aborting.\n";
		return 1;
	}
	std::cout << p << "\n"; 
	std::cout << "Size: " << std::filesystem::file_size(p) << " bytes\n"; 

	
	std::filebuf buffer;
	if(!buffer.open(p, std::ios::binary|std::ios::in) ){
		std::cerr << "Failed to open file.\n";
		return 1;
	} else {
		std::cout << "File read success.\n";
	}
	
	ifPPM stuff;
	stuff.decode(&buffer);
	return 0;
}
