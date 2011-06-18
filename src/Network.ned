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

network Network
{
    @display("bgb=581,401");
    submodules:
        R2: Receiver {
            @display("p=469,303;i=device/laptop");
        }
        R1: Receiver {
            @display("p=469,77;i=device/laptop");
        }
        T1: Transmitter {
            @display("p=58,77;i=device/pc2");
        }
        T2: Transmitter {
            @display("p=58,303;i=device/pc2");
        }
        ROU2: Router {
            @display("p=358,196;i=abstract/router");
        }
        ROU1: Router {
            @display("p=193,196;i=abstract/router");
        }



    connections:

        T1.out --> Link --> ROU1.in_port++;
        T2.out --> Link --> ROU1.in_port++;
        ROU1.out_port++ --> Link --> ROU2.in_port++;
        ROU2.out_port++ --> R1.in;
        ROU2.out_port++ --> R2.in;


}
