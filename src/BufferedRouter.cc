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



void BufferedRouter::fillTableFromFile(){

	std::ifstream inputFile;
	std::stringstream filename_string;
	std::string line;

	const char *token;

	//Obter o nome do arquivo de roteamento a partir do nome do módulo
	filename_string << "input/" << this->getName() << ".routing";

	//Abrir o arquivo
	inputFile.open(filename_string.str().c_str());
	if(!inputFile.is_open()) {
		error("Routing file %s for %s could not be opened", filename_string.str().c_str(), this->getName());
	}

	//Pegar a primeira linha, deve conter o caminho padrão
	if(getline(inputFile, line)) {

		cStringTokenizer tokenizer = cStringTokenizer(line.c_str());
		token = tokenizer.nextToken();
		//Primeira palavra da primeira linha deve ser default
		if(strcmp(token, "default") == 0) {
			if(tokenizer.hasMoreTokens()) {
				routingTable["default"] = tokenizer.nextToken();
			}
			else {
				error("No routing information provided for default gate");
			}

		} else {
			error("Default gate not defined in file %s", filename_string.str().c_str());
		}
	} else {
		error("Could not read first line of %s", filename_string.str().c_str());
	}

	//Processar o arquivo.
	while(getline(inputFile, line)) {
		cStringTokenizer tokenizer = cStringTokenizer(line.c_str());
		if(tokenizer.hasMoreTokens()) {
			token = tokenizer.nextToken();
			if(tokenizer.hasMoreTokens()) {
				routingTable[token] = tokenizer.nextToken();
			} else {
				error("Could not parse line %s", line.c_str());
			}
		} else {
			error("Could not parse line %s", line.c_str());
		}
	}

	//Fechar o arquivo
	inputFile.close();
}


cGate* BufferedRouter::getGateFromTable(QoSMessage *pkt) {

	cGate *outGate = gate("out1");

	const char *destination = pkt->getTo();
	const char *next_hop;

	//Obter o próximo hop, consultando a tabela de roteamento.
	it = routingTable.find(destination);
	if(it != routingTable.end()) {
		next_hop = (it->second).c_str();
	} else {
		next_hop = (routingTable["default"]).c_str();
	}

	EV << next_hop << "\n";



	return outGate;
}

void BufferedRouter::initialize()
{
	next = new cMessage("Buffered Router Next");
	isFinite = par("isFinite").boolValue();
	bufferSize = par("bufferSize").longValue();
	bufferedSize = 0;

	EV << "Buffer size: " << bufferSize << "\n";
	EV << "Is finite? " << isFinite << "\n";
	EV << "Buffered size: " << bufferedSize << "\n";

	//Obter tabela de roteamento a partir de um arquivo
	fillTableFromFile();

	//Registro de sinais
	droppedFromT1SeqID = registerSignal("dropped_t1_seq");
	droppedFromT2SeqID = registerSignal("dropped_t2_seq");
	sentToR1SeqID = registerSignal("sent_r1_seq");
	sentToR2SeqID = registerSignal("sent_r2_seq");
	droppedFromT1LengthID = registerSignal("dropped_t1_length");
	droppedFromT2LengthID = registerSignal("dropped_t2_length");
	sentToR1LengthID = registerSignal("sent_t1_length");
	sentToR2LengthID = registerSignal("sent_t2_length");
	freeBufferID = registerSignal("free_buffer");

}

void BufferedRouter::handleMessage(cMessage *msg)
{
	cGate *outGate;
	cChannel *txChannel;
	QoSMessage *pkt;

	//Se não é uma mensagem interna (ou seja, é um pacote)
	if(!msg->isSelfMessage()) {

		//Transforma a mensagem cMessage em pacote QoSMessage;
		pkt = (QoSMessage *)msg;
		//Obtem o gate de envio a partir da tabela de roteamento
		outGate = getGateFromTable(pkt);
		//Obtem o canal de envio a partir do gate de envio.
		txChannel = outGate->getChannel();

		//Se o canal está livre
		if(!txChannel->isBusy()) {
			//Envia a mensagem
			EV << "ROU1 encaminha mensagem imediatamente '"<< msg->getName() <<"'";
			send(msg, outGate);

			//Envio de sinais indicando envio de mensagens
			if(strcmp(pkt->getTo(), "R1")==0) {
				emit(sentToR1SeqID, (unsigned long)pkt->getSeqCount());
				emit(sentToR1LengthID, (unsigned long)pkt->getByteLength());
			} else if(strcmp(pkt->getTo(), "R2")==0) {
				emit(sentToR2SeqID, (unsigned long)pkt->getSeqCount());
				emit(sentToR2LengthID, (unsigned long)pkt->getByteLength());
			}

			//Agenda o próximo envio para o instante que o envio atual terminar
			scheduleAt(simTime()+txChannel->calculateDuration(msg), next);
		}
		//Se o canal não está livre
		else {
			if(isFinite) {
				//Obter o tamanho da mensagem.
				long messageLength = pkt->getByteLength();
				//Se a mensagem for maior que o buffer disponível
				if(messageLength+bufferedSize > bufferSize) {
					EV << "Mensagem descartada em ROU1:" << msg->getName() << ". messageLength: " << messageLength << "; bufferedSize: " << bufferedSize << "; bufferSize: " << bufferSize;

					//Envio de sinais indicando perda de pacotes.
					if(strcmp(pkt->getFrom(), "T1")==0) {
						emit(droppedFromT1SeqID, (unsigned long)pkt->getSeqCount());
						emit(droppedFromT1LengthID, (unsigned long)pkt->getByteLength());
					} else if(strcmp(pkt->getFrom(), "T2")==0) {
						emit(droppedFromT2SeqID, (unsigned long)pkt->getSeqCount());
						emit(droppedFromT2LengthID, (unsigned long)pkt->getByteLength());
					}

					delete(msg);

				}
				//Se a mensagem couber no buffer disponível
				else {
					//Atualizar o contador de buffer
					bufferedSize = bufferedSize + messageLength;
					EV << "ROU1 adicionando mensagem ao buffer finito:" << msg->getName() << ". messageLength: " << messageLength << "; bufferedSize: " << bufferedSize << "; bufferSize: " << bufferSize;
					//Armazenar a mensagem no buffer
					queue.insert(msg);
					emit(freeBufferID, (unsigned long)(bufferSize-bufferedSize));
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
				emit(freeBufferID, (unsigned long)(bufferSize-bufferedSize));
			}

			//Obtem o gate de envio a partir da tabela de roteamento
			outGate = getGateFromTable(QoSMessageToSend);
			//Obtem o canal de envio a partir do gate de envio.
			txChannel = outGate->getChannel();
			//Envia a mensagem
			EV << "ROU1 encaminha mensagem retirada do buffer '"<< QoSMessageToSend->getName() <<"'";
			send(QoSMessageToSend, outGate);

			//Envio de sinais indicando envio de mensagens
			QoSMessage *pkt = QoSMessageToSend;
			if(strcmp(pkt->getTo(), "R1")==0) {
				emit(sentToR1SeqID, (unsigned long)pkt->getSeqCount());
				emit(sentToR1LengthID, (unsigned long)pkt->getByteLength());
			} else if(strcmp(pkt->getTo(), "R2")==0) {
				emit(sentToR2SeqID, (unsigned long)pkt->getSeqCount());
				emit(sentToR2LengthID, (unsigned long)pkt->getByteLength());
			}
			//Agenda o próximo envio para o instante que o envio atual terminar
			scheduleAt(simTime()+txChannel->calculateDuration(QoSMessageToSend), next);
		}
	}
}
