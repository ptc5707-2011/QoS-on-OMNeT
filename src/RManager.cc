//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "RManager.h"

RManager::RManager() {

}

void RManager::vector2file(vector<double> input_vector, const char* output_filename) {

	ofstream output_file;
	output_file.open(output_filename);

	vector<double>::iterator it;

	for(it = input_vector.begin(); it < input_vector.end(); it++) {
		output_file << *it << endl;
	}

	output_file.close();

}

vector<double> RManager::file2vector(const char* filename) {



	ifstream input_file;
	string input_line;
	vector<double> output_vector;

	input_file.open(filename);

	if(input_file.is_open()) {
		while(input_file.good()) {
			getline(input_file, input_line);
			output_vector.push_back(atof(input_line.c_str()));
		}
		input_file.close();
	} else {
		cout << "Impossível abrir arquivo para file2vector";
	}

	return output_vector;
}

void RManager::source_file(const char* filename) {


	stringstream command_input (stringstream::in | stringstream::out);

	command_input << "R --no-save --no-restore < " << filename;

	int command_return = system(command_input.str().c_str());

}


RManager::~RManager() {
	// TODO Auto-generated destructor stub
}
