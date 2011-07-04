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
	next = new cMessage("Next");
}

void BufferedRouter::handleMessage(cMessage *msg)
{
	cGate *outGate = gate("out1");
	cChannel *txChannel = outGate->getChannel();
	//Se Ž um pacote
	if(msg->isPacket()) {
		//Se o canal est‡ livre
		if(!txChannel->isBusy()) {
			//Envia a mensagem
			send(msg, outGate);
			//Agenda o pr—ximo envio para o instante que o envio atual terminar
			scheduleAt(simTime()+txChannel->calculateDuration(msg), next);
		}
		//Se o canal n‹o est‡ livre
		else {
			queue.insert(msg);
		}
	}
	//Se Ž uma mensagem interna
	else{
		//Se h‡ alguma mensagem na fila
		if(!queue.isEmpty()) {
			//Retira a mensagem da fila (FIFO)
			QoSMessage *QoSMessageToSend = (QoSMessage *)queue.pop();
			//Envia a mensagem
			send(QoSMessageToSend, outGate);
			//Agenda o pr—ximo envio para o instante que o envio atual terminar
			scheduleAt(simTime()+txChannel->calculateDuration(QoSMessageToSend), next);
		}
	}
}
