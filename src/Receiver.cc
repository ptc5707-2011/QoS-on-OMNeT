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

	next_seq = 1;
    //Registrar sinais
	sentTimeSignalID = registerSignal("sent_timestamp");
	lengthSignalID = registerSignal("received_packet_length");
	seqSignalID = registerSignal("received_packet_seq");
	delaySignalID = registerSignal("packet_delay");
	receivedOrDroppedSignalID = registerSignal("received_or_dropped"); //Received = 1, dropped = 0
}

void Receiver::handleMessage(cMessage *msg)
{


	QoSMessage *pkt = (QoSMessage *)msg;
	EV << this->getName() << " recebeu '" << msg->getName() << "\n";

	if(!(next_seq == pkt->getSeqCount())) {
		for(unsigned long i = next_seq; i < pkt->getSeqCount(); i++) {
			EV << this->getName() << " detectou perda do pacote '" << msg->getName() <<"\n";
			emit(sentTimeSignalID, 0);
			emit(delaySignalID, 0);
			emit(lengthSignalID, 0);
			emit(seqSignalID, i);
			emit(receivedOrDroppedSignalID, 0);
		}
	}

	emit(sentTimeSignalID, pkt->getCreationTime());
	emit(delaySignalID, (pkt->getArrivalTime()-pkt->getCreationTime()));
	emit(lengthSignalID, (unsigned long)pkt->getByteLength());
	emit(seqSignalID, (unsigned long)pkt->getSeqCount());
	emit(receivedOrDroppedSignalID, 1);

	next_seq = pkt->getSeqCount()+1;

	delete(msg);
}
