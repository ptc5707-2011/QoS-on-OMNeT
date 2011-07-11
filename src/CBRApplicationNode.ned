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
simple CBRApplicationNode like ApplicationNode
{
    parameters:
        string from;
        string to;
        int bytesPerPacket @unit(B) = default(64B);
        double timeBetweenPackets @unit(s) = default(100ms);
        
        //Estatísticas
        @statistic[length] (title="CBR Length";record=vector);

    gates:
        output out1;   
}
