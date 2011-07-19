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
simple BufferedRouter
{
    parameters:
        bool isFinite;
        int bufferSize @unit(byte);
        @display("i=abstract/router");
        
        //Estatísticas
        @statistic[sent_packet_length] (title="CBR Sent Packet Length";record=vector);
        @statistic[dropped_t1_seq] (title="Dropped from T1 Sequence Number";record=vector);
        @statistic[dropped_t2_seq] (title="Dropped from T2 Sequence Number";record=vector);
        @statistic[sent_r1_seq] (title="Sent to R1 Sequence Number";record=vector);
        @statistic[sent_r2_seq] (title="Sent to R2 Sequence Number";record=vector);
        @statistic[dropped_t1_length] (title="Dropped from T1 Byte Length";record=vector);
        @statistic[dropped_t2_length] (title="Dropped from T2 Byte Length";record=vector);
        @statistic[sent_t1_length] (title="Sent to R1 Byte Length";record=vector);
        @statistic[sent_t2_length] (title="Sent to R2 Byte Length";record=vector);
        @statistic[free_buffer] (title="Free space in buffer (bytes)";record=vector);
        
        
    gates:
        input in1;
        input in2;
        output out1;
}
