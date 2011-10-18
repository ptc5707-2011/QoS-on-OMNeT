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

	//Se o módulo não está dentro de um módulo composto
	if(outGate->getChannel()) {
		if(outGate->getChannel()->isTransmissionChannel()) {
			outChannel = outGate->getTransmissionChannel();
		} else {
			outChannel = outGate->getChannel();
		}
	} else if(outGate->getNextGate()->getChannel()) { //se o módulo está dentro de um módulo composto
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
	queueSizeSignal = registerSignal("queueSize");

}

void FifoQueue::controlBuffer(QueueControllerMessage *ctrlMsg) {

	double forecasted_load = ctrlMsg->getLoad();
	double window_time = ctrlMsg->getWindowTime();

	EV << "incoming load: " << forecasted_load << ", datarate: " << outChannel->getNominalDatarate();

	double datarate = outChannel->getNominalDatarate();


	long recommendation;
	if(forecasted_load > datarate) {
		recommendation = round((forecasted_load-datarate)*window_time);
	} else {
		recommendation = -round(datarate*window_time);
	}

	EV << "recommendation: " << recommendation;

	if(recommendation > 0)  { //Aumentar o tamanho do buffer
		queueBufferLength += recommendation;
	} else { //Diminuir o tamanho do buffer

		if(queueBufferLength +recommendation > usedQueueBufferLength) { //Se o novo valor do buffer for maior que o buffer utilizado no momento
			queueBufferLength += recommendation;
		} else {
			queueBufferLength = usedQueueBufferLength;
		}
	}

	delete ctrlMsg;

}

void FifoQueue::handleMessage(cMessage *msg)
{

	QoSMessage *pkt;

	//Acabou a transmissão de um pacote
	if(msg == endedTransmission) {
		if(queue.empty()) {

			busy = false;

			emit(bytesInQueueSignal, 0);
			emit(queueSizeSignal, queueBufferLength);
			emit(packetsInQueueSignal, 0);
			emit(busySignal, 0);

		} else {
			pkt = (QoSMessage *) queue.pop();
			usedQueueBufferLength -= pkt->getByteLength();

			busy = true;

			emit(bytesInQueueSignal, usedQueueBufferLength);
			emit(queueSizeSignal, queueBufferLength);
			emit(packetsInQueueSignal, queue.length());
			emit(busySignal, 1);

			simtime_t transmission_time = outChannel->calculateDuration(pkt);
			messageBeingTransmitted = pkt;
			scheduleAt(simTime()+transmission_time, endedTransmission);
			send(pkt, outGate);

		}

	} else { //chegou um pacote ou um comando

		if(msg->arrivedOn("in")) { //Se chegou um pacote
			pkt = (QoSMessage*)msg;

			if(!busy) {

				busy = true;

				emit(bytesInQueueSignal, 0);
				emit(queueSizeSignal, queueBufferLength);
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
					emit(queueSizeSignal, queueBufferLength);
					emit(packetsInQueueSignal, queue.length());
				}

			}

		} else { //Se chegou um comando
			controlBuffer((QueueControllerMessage *) msg);
		}

	}

}

FifoQueue::~FifoQueue() {
	//delete messageBeingTransmitted;
	//cancelAndDelete(endedTransmission);
}
