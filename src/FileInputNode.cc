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

#include "FileInputNode.h"

Define_Module(FileInputNode);


struct timestamp_length FileInputNode::parseLine(std::string line) {

	using namespace std;


	timestamp_length parsed_line;		//Resultado da linha interpretada
	char timestamp_char[17];			//Usado na leitura do timestamp sem 0x
	string timestamp;					//Usado para concatenar 0x ao timestamp
	unsigned long bytes;				//Usado para guardar o número de bytes do pacote
	unsigned long long timestamp_temp;	//Usado para transformar o valor de timestamp para unsigned int 64

	//Obter timestamp sem 0x e bytes da linha
    sscanf(line.c_str(),"%s\t%ld\n", timestamp_char, &bytes);

    //Adicionar 0x
    timestamp = timestamp_char;
    timestamp = "0x" + timestamp;

    //Transformar em uint64
    sscanf(timestamp.c_str(), "%llx", &timestamp_temp);

    //Guardar na struct parsed_line
    parsed_line.timestamp = timestamp_temp;
    parsed_line.byteLength = bytes;

    //Retornar o resultado
    return parsed_line;

}

struct packet_time FileInputNode::getPacketTimestamp() {

	using namespace std;

	struct packet_time next_packet_time;	//Struct para receber o tamanho do pacote atual e o timestamp do próximo
	struct timestamp_length parsed_line;		//Struct para receber uma linha interpretada
	string line;								//Variável que guarda uma linha do arquivo.


	if(getline(inputFile, line)) //Tentar pegar uma linha.
	{
		//Interpretar a linha
		parsed_line = parseLine(line);

		//Calcular o próximo timestamp
		next_packet_time.schedule_at = (parsed_line.timestamp - last_timestamp)*pow(2.0,-32);
		//Copiar o número de bytes de parsed_line para next_packet_time
		next_packet_time.byteLength = parsed_line.byteLength;
		//Como a leitura foi bem-sucedida, não parar de enviar.
		next_packet_time.stop = false;

		last_timestamp = parsed_line.timestamp;

	} else //Se não conseguir, o arquivo acabou
	{
		next_packet_time.schedule_at = 0;
		next_packet_time.byteLength = 0;
		next_packet_time.stop = true;


		//TODO: emitir um sinal que o arquivo acabou
		//pode ser usado por outros módulos para interromper o envio também.
	}

	return next_packet_time;

}


void FileInputNode::initialize()
{

	seqcounter = 0;

	QoSMessage *first_pkt = new QoSMessage();
	struct packet_time next_packet_timestamp;

	//Abrir o arquivo
	filename = par("filename").stringValue();
	inputFile.open(filename);
	if(!inputFile.is_open()) {
		error("%s could not be opened", filename);
	}

	//Registrar o sinal de envio de pacotes.
	lengthSignalID = registerSignal("sent_packet_length");

	//Tentar pegar a primeira linha para inicializar a variável last_timestamp
	//e enviar o primeiro pacote.
	std::string line;
	struct timestamp_length parsed_line;
	if(getline(inputFile, line))
	{
		//Interpretar a linha obtida
		parsed_line = parseLine(line);

		//Salvar o timestamp da primeira linha em last_timestamp
		last_timestamp = parsed_line.timestamp;

	} else
	{
		//Erro se não conseguir pegar a primeira linha.
		error("Could not get the first line of the file for parsing");
	}

	seqcounter++;
	//Construir o primeiro pacote e enviar
	first_pkt->setName("File message, seq: 0");
	first_pkt->setFrom("T2");
	first_pkt->setTo("R2");
	first_pkt->setSeqCount(1);
	first_pkt->setByteLength(parsed_line.byteLength);
	send(first_pkt,"out1");
	emit(lengthSignalID, (unsigned long)first_pkt->getByteLength());

	//Obter o próximo pacote e seu timestamp
	next_packet_timestamp = getPacketTimestamp();

	//Se o pacote existir (houver a linha no arquivo), agendar seu envio
	if(!next_packet_timestamp.stop) {
		seqcounter++;
		QoSMessage * next_pkt = new QoSMessage();
		next_pkt->setName("File message, seq: 2");
		next_pkt->setFrom("T2");
		next_pkt->setTo("R2");
		next_pkt->setSeqCount(seqcounter);
		next_pkt->setByteLength(next_packet_timestamp.byteLength);

		scheduleAt(simTime() + next_packet_timestamp.schedule_at, next_pkt);
	}
}

void FileInputNode::handleMessage(cMessage *msg)
{

	seqcounter++;
	send(msg, "out1");
	emit(lengthSignalID, (unsigned long)((QoSMessage *)msg)->getByteLength());

	struct packet_time next_packet_timestamp = getPacketTimestamp();

	if(!next_packet_timestamp.stop) {
		QoSMessage * next_pkt = new QoSMessage();

		std::stringstream messageName;
		messageName << "File message, seq: " << seqcounter;

		next_pkt->setName(messageName.str().c_str());
		next_pkt->setFrom("T2");
		next_pkt->setTo("R2");
		next_pkt->setSeqCount(seqcounter);
		next_pkt->setByteLength(next_packet_timestamp.byteLength);

		scheduleAt(simTime() + next_packet_timestamp.schedule_at, next_pkt);
		} else {
			//TODO: EMITIR SINAL PARA PARAR A SIMULAÇÃO
		}
}

void FileInputNode::finish() {

	//Fechar o arquivo
	if(inputFile.is_open()) {
		inputFile.close();
	}
}
