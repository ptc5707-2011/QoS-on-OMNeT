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

#include "FileInputNode.h"

Define_Module(FileInputNode);




void FileInputNode::initialize()
{

	//Abrir o arquivo
	filename = par("filename").stringValue();
	inputFile.open(filename);
	if(!inputFile.is_open()) {
		error("%s could not be opened", filename);
	}



}

void FileInputNode::handleMessage(cMessage *msg)
{

}

void FileInputNode::finish() {

	//Fechar o arquivo
	if(inputFile.is_open()) {
		inputFile.close();
	}
}
