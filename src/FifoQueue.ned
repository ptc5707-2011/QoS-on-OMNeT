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
simple FifoQueue
{
    parameters:
        int buffer_length @unit(byte);

        @statistic[droppedPacketLength](title="Dropped packets length"; record=vector);
        @statistic[droppedPacketSeq](title="Dropped packets sequence number"; record=vector);
        @statistic[busy](title="Busy channel"; record=vector);
        @statistic[packetsInQueue](title="Number of packets in queue"; record=vector);
        @statistic[bytesInQueue](title="Number of bytes in queue"; record=vector);
        @statistic[queueSize](title="Queue Size"; record=vector);
        

        @display("i=block/boundedqueue");
    gates:
        input in;
        output out;

        input ctrl @loose;

}
