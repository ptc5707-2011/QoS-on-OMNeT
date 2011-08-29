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

#include "FifoQueue.h"

Define_Module(FifoQueue);

void FifoQueue::initialize()
{
    endedTransmission = new cMessage("Ended Transmission");

    busy = false;
    queueBufferLength = par("buffer_length");
    usedQueueBufferLength = 0;

	outGate = gate("out");

	//O módulo não está dentro de um módulo composto
	if(outGate->getChannel()) {
		if(outGate->getChannel()->isTransmissionChannel()) {
			outChannel = outGate->getTransmissionChannel();
		} else {
			outChannel = outGate->getChannel();
		}
	} else if(outGate->getNextGate()->getChannel()) { //o módulo está dentro de um módulo composto
		if(outGate->getNextGate()->getChannel()->isTransmissionChannel()){
			outChannel = outGate->getNextGate()->getTransmissionChannel();
		} else {
			outChannel = outGate->getNextGate()->getChannel();
		}

	} else {
		error("Could not find transmission channel");
	}


	//Sinais

	droppedPacketLengthSignal = registerSignal("droppedPacketLength");
	droppedPacketSeqSignal = registerSignal("droppedPacketSeq");
	busySignal = registerSignal("busy");
	packetsInQueueSignal = registerSignal("packetsInQueue");
	bytesInQueueSignal = registerSignal("bytesInQueue");

}

void FifoQueue::handleMessage(cMessage *msg)
{

	QoSMessage *pkt;

	//Acabou a transmissão de um pacote
	if(msg == endedTransmission) {
		if(queue.empty()) {

			busy = false;

			emit(bytesInQueueSignal, 0);
			emit(packetsInQueueSignal, 0);
			emit(busySignal, 0);

		} else {
			pkt = (QoSMessage *) queue.pop();
			usedQueueBufferLength -= pkt->getByteLength();

			busy = true;

			emit(bytesInQueueSignal, usedQueueBufferLength);
			emit(packetsInQueueSignal, queue.length());
			emit(busySignal, 1);

			simtime_t transmission_time = outChannel->calculateDuration(pkt);
			messageBeingTransmitted = pkt;
			scheduleAt(simTime()+transmission_time, endedTransmission);
			send(pkt, outGate);

		}

	} else { //chegou um pacote

		pkt = (QoSMessage*)msg;

		if(!busy) {

			busy = true;

			emit(bytesInQueueSignal, 0);
			emit(packetsInQueueSignal, 0);
			emit(busySignal, 1);

			simtime_t transmission_time = outChannel->calculateDuration(pkt);
			messageBeingTransmitted = pkt;
			scheduleAt(simTime()+transmission_time, endedTransmission);
			send(pkt, outGate);

		} else {


			if(pkt->getByteLength() + usedQueueBufferLength > queueBufferLength) {
				emit(droppedPacketLengthSignal, (unsigned long) pkt->getByteLength());
				emit(droppedPacketSeqSignal, pkt->getSeqCount());
				delete pkt;
			} else {
				usedQueueBufferLength += pkt->getByteLength();
				queue.insert(msg);

				emit(bytesInQueueSignal, usedQueueBufferLength);
				emit(packetsInQueueSignal, queue.length());
			}

		}

	}

}

FifoQueue::~FifoQueue() {
	//delete messageBeingTransmitted;
	//cancelAndDelete(endedTransmission);
}
