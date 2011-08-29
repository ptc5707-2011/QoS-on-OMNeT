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

#include "Receiver.h"

Define_Module(Receiver);

void Receiver::initialize()
{
    //Registrar sinais
	lengthSignalID = registerSignal("received_packet_length");
	seqSignalID = registerSignal("received_packet_seq");
	delaySignalID = registerSignal("packet_delay");
}

void Receiver::handleMessage(cMessage *msg)
{
	EV << this->getName() << " recebeu '" << msg->getName() << "'";
	QoSMessage *pkt = (QoSMessage *)msg;

	emit(delaySignalID, (pkt->getArrivalTime()-pkt->getCreationTime()));
	emit(lengthSignalID, (unsigned long)pkt->getByteLength());
	emit(seqSignalID, (unsigned long)pkt->getSeqCount());

	delete(msg);
}
