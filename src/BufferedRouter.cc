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
#include "BufferedRouter.h"

Define_Module(BufferedRouter);

void BufferedRouter::initialize()
{
	next = new cMessage("Buffered Router Next");
	isFinite = par("isFinite").boolValue();
	bufferSize = par("bufferSize").longValue();
	bufferedSize = 0;

	EV << "Buffer size: " << bufferSize << "\n";
	EV << "Is finite? " << isFinite << "\n";
	EV << "Buffered size: " << bufferedSize << "\n";
}

void BufferedRouter::handleMessage(cMessage *msg)
{
	cGate *outGate = gate("out1");
	cChannel *txChannel = outGate->getChannel();
	//Se não é uma mensagem interna (ou seja, é um pacote)
	if(!msg->isSelfMessage()) {
		//Se o canal está livre
		if(!txChannel->isBusy()) {
			//Envia a mensagem
			EV << "ROU1 encaminha mensagem imediatamente '"<< msg->getName() <<"'";
			send(msg, outGate);
			//Agenda o próximo envio para o instante que o envio atual terminar
			scheduleAt(simTime()+txChannel->calculateDuration(msg), next);
		}
		//Se o canal não está livre
		else {
			if(isFinite) {
				//Obter o tamanho da mensagem.
				long messageLength = ((QoSMessage *)msg)->getByteLength();
				//Se a mensagem for maior que o buffer disponível
				if(messageLength+bufferedSize > bufferSize) {
					EV << "Mensagem descartada em ROU1:" << msg->getName() << ". messageLength: " << messageLength << "; bufferedSize: " << bufferedSize << "; bufferSize: " << bufferSize;
					delete(msg);
				}
				//Se a mensagem couber no buffer disponível
				else {
					//Atualizar o contador de buffer
					bufferedSize = bufferedSize + messageLength;
					EV << "ROU1 adicionando mensagem ao buffer finito:" << msg->getName() << ". messageLength: " << messageLength << "; bufferedSize: " << bufferedSize << "; bufferSize: " << bufferSize;
					//Armazenar a mensagem no buffer
					queue.insert(msg);
				}
			}
			else {
				//Armazenar a mensagem no buffer
				EV << "ROU1 adicionando mensagem ao buffer infinito:" << msg->getName();
				queue.insert(msg);
			}
		}
	}
	//Se é uma mensagem interna
	else{
		//Se há alguma mensagem na fila
		if(!queue.isEmpty()) {
			//Retira a mensagem da fila (FIFO)
			QoSMessage *QoSMessageToSend = (QoSMessage *)queue.pop();

			//Se o buffer é finito
			if(isFinite) {
				//Obter o tamanho da mensagem
				long messageLength = ((QoSMessage *)QoSMessageToSend)->getByteLength();
				//Atualizar o contador de buffer
				bufferedSize = bufferedSize - messageLength;
			}

			//Envia a mensagem
			EV << "ROU1 encaminha mensagem retirada do buffer '"<< QoSMessageToSend->getName() <<"'";
			send(QoSMessageToSend, outGate);
			//Agenda o próximo envio para o instante que o envio atual terminar
			scheduleAt(simTime()+txChannel->calculateDuration(QoSMessageToSend), next);
		}
	}
}
