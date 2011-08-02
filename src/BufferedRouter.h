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

#ifndef __QOS_BUFFEREDROUTER_H_
#define __QOS_BUFFEREDROUTER_H_

#include <omnetpp.h>
#include "QoSMessage_m.h"
#include <iostream>
#include <fstream>
#include <string>

/**
 * TODO - Generated class
 */
class BufferedRouter : public cSimpleModule
{

	private:
		cQueue queue;
		cMessage *next;
		bool isFinite;
		long bufferSize;
		long bufferedSize;
		std::map<std::string, std::string> routingTable;
		std::map<std::string, std::string>::iterator it;


		cGate* getGateFromTable(QoSMessage *pkt);
		void fillTableFromFile();

		//Sinais
		simsignal_t droppedFromT1SeqID; 	//Seq number de drop de pacotes de T1 pra R1
		simsignal_t droppedFromT2SeqID;		//Seq number de drop de pacotes de T2 para R2
		simsignal_t sentToR1SeqID;			//Seq number de pacotes enviados de T1 para R1
		simsignal_t sentToR2SeqID;			//Seq number de pacotes enviados de T2 para R2
		simsignal_t droppedFromT1LengthID;	//Comprimento em bytes de drop de pacotes de T1 para R1
		simsignal_t droppedFromT2LengthID;	//Comprimento em bytes de drop de pacotes de T2 para R2
		simsignal_t sentToR1LengthID;		//Comprimento em bytes de pacotes enviados de T1 para R1
		simsignal_t sentToR2LengthID;		//Comprimento em bytes de pacotes enviados de T2 para R2
		simsignal_t freeBufferID;			//Espaço livre no buffer em bytes.


	protected:
		virtual void initialize();
		virtual void handleMessage(cMessage *msg);

};

#endif
