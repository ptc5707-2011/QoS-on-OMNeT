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

#include "InterfenceNode.h"

Define_Module(InterfenceNode);

void InterfenceNode::initialize()
{
	scheduleAt(simTime()+par("timeBetweenPackets"), new cMessage);
}

void InterfenceNode::handleMessage(cMessage *msg)
{
	QoSMessage *pkt = new QoSMessage("Interference message");
	pkt->setByteLength(par("packetLength").longValue());
	pkt->setFrom(par("from"));
	pkt->setTo(par("to"));
	send(pkt, "out1");
	scheduleAt(simTime()+par("timeBetweenPackets"), msg);
}
