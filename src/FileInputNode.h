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

#ifndef __QOS_FILEINPUTNODE_H_
#define __QOS_FILEINPUTNODE_H_

#include <omnetpp.h>
#include <iostream>
#include <fstream>
#include <string>

struct timestamp_length {
	simtime_t timestamp;
	long byteLength;
	bool stop;
};

/**
 * TODO - Generated class
 */
class FileInputNode : public cSimpleModule
{

	private:
		const char* filename;
		simtime_t last_timestamp;
		std::ifstream inputFile;


	protected:
		virtual void initialize();
		virtual void handleMessage(cMessage *msg);
		virtual void finish();
		struct timestamp_length getNextPacket();
};

#endif
