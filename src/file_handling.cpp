#include "file_handling.h"

using namespace std;

void file_handling::define_path_to_file(const string& _path_to_file)
{
	path_to_file = _path_to_file;
}

vector<vector<float>> file_handling::read_file(const string& _path_to_file){
	/* The function should read the file line by line. If the line starts with a "%", the
	 * line should be ignored and treated as a comment.
	 */

	 define_path_to_file(_path_to_file);

	 // Open file
	 my_file.open(path_to_file);
	 if(my_file.is_open() == 1)
	 {
		// Clear variable before reading the file
		 dataset.clear();

		while (getline(my_file, temp_line))
		{
			// Check if line is comment
			if(temp_line.at(0) != '%')
			{
				// Clear temp_vector before reading
				temp_vector.clear();
				//cout << "Read new line" << endl;
				istringstream iss(temp_line);
				while(iss >> temp)
				{
					temp_vector.push_back(temp);
				}
				//cout << temp_vector.size() << endl;

				// Push temp-vector to dataset
				dataset.push_back(temp_vector);
			}


		}
	}
	 else
		 cout << " E: File " << path_to_file << " could not be opened" << endl;

	my_file.close();
	return dataset;
}
