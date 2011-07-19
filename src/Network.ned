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

import ned.DelayChannel;
import ned.DatarateChannel;
import ned.IdealChannel;


network Network
{
    parameters:
        @display("bgb=901,567");
        double link_delay @unit(s);
        double link_datarate @unit(bps);

        string applicationNodeType;
        string interferenceNodeType;

    submodules:
        ROU1: BufferedRouter {
            @display("p=179,216");
        }
        ROU2: IdealRouter {
            @display("p=533,216");
        }
        T1: <applicationNodeType> like SourceNode {
            @display("p=72,65");
            from = "T1";
            to = "R1";
        }
        T2: <interferenceNodeType> like SourceNode {
            @display("p=72,365");
            from = "T2";
            to = "R2";
        }
        R1: Receiver {
            @display("p=639,88;i=device/laptop");
        }
        R2: Receiver {
            @display("p=639,365;i=block/network2");
        }
    connections:

        T1.out1 --> IdealChannel --> ROU1.in1;
        T2.out1 --> IdealChannel --> ROU1.in2;
        ROU2.out1 --> IdealChannel --> R1.in1;
        ROU2.out2 --> IdealChannel --> R2.in1;
        ROU1.out1 --> DatarateChannel {  delay = link_delay; datarate = link_datarate; } --> ROU2.in1;
}
//
// TODO documentation
//
// @author hugo
//
network FileInputTest
{
    
    parameters:
        string senderType;
    
    submodules:
        T2: <senderType> like SourceNode {
            @display("p=88,88");
        }
        R2: Receiver {
            @display("p=189,88");
        }
    connections:
        T2.out1 --> IdealChannel --> R2.in1;
}

