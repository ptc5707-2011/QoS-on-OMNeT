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

#include "ARIMAInterferenceNode.h"

Define_Module(ARIMAInterferenceNode);


double ARIMAInterferenceNode::getPacket()
{

	double new_yt = 0.0;

	//Cálculo da parcela AR
	for(unsigned int i = 0; i < phi.size(); i++) {
		new_yt += phi[i]*yt[i];
	}

	//Cálculo da parcela MA
	for(unsigned int i = 0; i < rho.size(); i++) {
		new_yt -= rho[i]*wt[i];
	}
	double inov = dblrand();
	new_yt += inov;

	//Atualização dos vetores de estado
	wt.insert(wt.begin(), inov);
	wt.erase(wt.end()-1);

	yt.insert(yt.begin(), new_yt);
	yt.erase(yt.end()-1);

	return new_yt;

}

void ARIMAInterferenceNode::initialize()
{
	//Obter os parâmetros
	const char *rho_string = par("rho_params").stringValue();
	const char *phi_string = par("phi_params").stringValue();
	phi = cStringTokenizer(rho_string).asDoubleVector();
	rho = cStringTokenizer(phi_string).asDoubleVector();
	p = phi.size();
	q = rho.size();
	d = par("d").longValue();

	EV << "Phi: ";
	for(unsigned int i = 0; i < phi.size(); i++) {
		EV << phi[i] << " ";
	}
	EV << "\n";

	EV << "Rho: ";
	for(unsigned int i = 0; i < rho.size(); i++) {
		EV << rho[i] << " ";
	}
	EV << "\n";

	//Alocação inicial
	yt.resize(p,0);
	wt.resize(q,0);

	//Preenchimento com números aleatórios.
	for(unsigned int i = 0; i< wt.size(); i++) {
		wt[i] = dblrand();
	}


}



void ARIMAInterferenceNode::handleMessage(cMessage *msg)
{

}
