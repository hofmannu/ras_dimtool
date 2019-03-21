/*
 * File:		file_handling.h
 * Author: 		Urs Hofmann
 * Date: 		19. Okt 2017
 * Version: 	0.1
 * 
 * This library handels the library files and reads / writes data for the off-the-shelf-
 * components to *.dat files. Comments in the *.dat file are allowed and start with a "%"
 * 
*/

#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class file_handling
{
private:
	fstream my_file;
	string path_to_file;
	string temp_line;
	vector<float> temp_vector;
	vector<vector<float>> dataset;
	
	float temp;
	
	
public:
	
	void define_path_to_file(const string& _path_to_file);
	vector<vector<float>> read_file(const string& _path_to_file);
	
};

#endif