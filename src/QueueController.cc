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

#include "QueueController.h"

Define_Module(QueueController);

void QueueController::initialize()
{

	std::string filename;

	cGate* ctrlGate = gate("out");

	filename = ctrlGate->getPathEndGate()->getOwnerModule()->gate("out")->getPathEndGate()->getOwnerModule()->getFullPath();

	filename = "input/"+filename;

	//Obter parâmetros
	warmup_time = par("warmup_time").doubleValue();
	window_time = par("window_time").doubleValue();


	//Abrir o arquivo
	inputFile.open(filename.c_str());
	if(!inputFile.is_open()) {
		EV << "Input file " << filename.c_str() << " for " << this->getName() << " could not be opened, assuming uncontrolled buffer." << endl;
	} else {
		//Kickstart
		next = new cMessage("");
		scheduleAt(simTime()+warmup_time, next);
	}

}

void QueueController::handleMessage(cMessage *msg)
{
	unsigned long i;
	if(inputFile >> i) {

		QueueControllerMessage *ctrlMsg = new QueueControllerMessage("Control Message");
		double load = ((double)i)/window_time;
		ctrlMsg->setLoad(load);
		ctrlMsg->setWindowTime(window_time);
		send(ctrlMsg, "out");

		EV << "Sending control message with load = " << ctrlMsg->getLoad() << ", window_time = " << ctrlMsg->getWindowTime() << endl;
		scheduleAt(simTime()+window_time, msg);
	}
}

QueueController::~QueueController() {
//	if(inputFile.is_open()) {
//		inputFile.close();
//	}
//
//	delete next;
}
