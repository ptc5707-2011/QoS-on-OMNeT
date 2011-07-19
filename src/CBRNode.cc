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

#include "CBRNode.h"

Define_Module(CBRNode);

void CBRNode::initialize()
{
	bytesPerPacket = par("bytesPerPacket").longValue();
	timeBetweenPackets = par("timeBetweenPackets").doubleValue();
	seqcounter = 0;

	next = new cMessage("CBR Next");
	scheduleAt(simTime(), next);

	//Registrar sinais
	lengthSignalID = registerSignal("sent_packet_length");
	seqSignalID = registerSignal("sent_packet_seq");
}

void CBRNode::handleMessage(cMessage *msg)
{
	seqcounter++;


	QoSMessage *pkt = new QoSMessage();
	pkt->setFrom("T1");
	pkt->setTo("R1");
	pkt->setByteLength(bytesPerPacket);
	pkt->setSeqCount(seqcounter);

	std::stringstream messageName;
	messageName << "CBR message, seq: " << seqcounter;
	pkt->setName(messageName.str().c_str());
	EV << "T1 criou mensagem '"<< pkt->getName() <<"'";

	emit(lengthSignalID, (unsigned long)pkt->getByteLength());
	emit(seqSignalID, (unsigned long)pkt->getSeqCount());

	send(pkt, "out1");
	scheduleAt(simTime()+timeBetweenPackets, next);
}
