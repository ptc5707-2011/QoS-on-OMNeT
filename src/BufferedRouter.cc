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

#include "BufferedRouter.h"

Define_Module(BufferedRouter);

void BufferedRouter::initialize()
{
	endTxMsg = new cMessage("Transmission ended, ready to transmit.");
}

void BufferedRouter::handleMessage(cMessage *msg)
{

	if(msg == endTxMsg) {
		if(!queue.isEmpty()) {
			send((cMessage *)queue.pop(), "out1");
		}
	} else {
		cChannel *txChannel = gate("out1")->getTransmissionChannel();
		simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
		if(txFinishTime <= simTime()) {
			send(msg, "out1");
		} else {
			queue.insert(msg);
			scheduleAt(txFinishTime, endTxMsg);
		}
	}
}
