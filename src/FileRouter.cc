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

#include "FileRouter.h"

Define_Module(FileRouter);


void FileRouter::fillTableFromFile(){

	std::ifstream inputFile;
	const char *filename;
	std::string line;
	const char *token;

	//Obter o nome do arquivo de roteamento a partir do nome do módulo
	filename = par("routingFile").stringValue();

	//Abrir o arquivo
	inputFile.open(filename);
	if(!inputFile.is_open()) {
		error("Routing file %s for %s could not be opened", filename, this->getName());
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


cGate* FileRouter::getGateFromTable(QoSMessage *pkt) {

	const char *destination = pkt->getTo();
	const char *next_hop;
	cGate *outGate = NULL;

	//Obter o próximo hop, consultando a tabela de roteamento.
	it = routingTable.find(destination);

	if(it != routingTable.end()) {
		next_hop = (it->second).c_str();
	} else {
		//Não há rota definida para este destino.
		return NULL;
	}

	//Obter gate conectado ao next hop.
	for(int i = 0; i < gateSize("out"); i++) {
		cGate *gate_i = gate("out", i);

		//gate_i->getPathEndGate()->getOwnerModule()->gate("out")->getNextGate()->getNextGate()->getOwnerModule()->getName()
		//Se o próximo nó é uma queue
		if(strcmp(gate_i->getPathEndGate()->getOwnerModule()->getClassName(), "FifoQueue") == 0) {
			if(strcmp(gate_i->getPathEndGate()->getOwnerModule()->gate("out")->getNextGate()->getNextGate()->getOwnerModule()->getName(), next_hop) == 0) {
				outGate = gate_i;
				break;
			}
		} else {
			if(strcmp(gate_i->getPathEndGate()->getOwner()->getName(),next_hop) == 0) {
				outGate = gate_i;
				break;
			}
		}
	}

	return outGate;
}


void FileRouter::initialize()
{
	fillTableFromFile();
	droppedSeqSignalID = registerSignal("dropped_no_route_seq");
	droppedLenSignalID = registerSignal("dropped_no_route_len");

}

void FileRouter::handleMessage(cMessage *msg)
{
    QoSMessage *pkt;
    cGate *outGate;



    pkt = (QoSMessage *) msg;
    outGate = getGateFromTable(pkt);

	EV << this->getName() << " recebeu '" << msg->getName() << "': " << pkt->getSeqCount() << "\n";

    if(outGate) {
    	EV << this->getName() << " encaminhando mensagem por: " << outGate->getName() << "\n";
    	send(pkt, outGate);
    } else {
    	EV << this->getName() << " descartando mensagem pois não foi encontrada uma rota para " << pkt->getTo() << "\n";
    	emit(droppedSeqSignalID, pkt->getSeqCount());
    	emit(droppedLenSignalID, (unsigned long)pkt->getByteLength());
    	delete pkt;
    }



}
