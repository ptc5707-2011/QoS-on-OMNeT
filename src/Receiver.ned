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

package qos;

//
// TODO auto-generated module
//
simple Receiver
{
    parameters:
        //Estatísticas
        @statistic[received_packet_length](title="Received packet length"; record=vector);
        @statistic[received_packet_seq](title="Received packet sequence number"; record=vector);
        @statistic[packet_delay](title="Received packet delay"; record=vector);
        @statistic[received_or_dropped](title="Packet received(1) or dropped(0)"; record=vector);
        @statistic[sent_timestamp](title="Creation time of received message"; record=vector);

        @display("i=device/pc2");
    gates:
        input in1;
}
