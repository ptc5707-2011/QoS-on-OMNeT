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

import ned.IdealChannel;


module ControlledRouter
{
    parameters:
        int bufferSize @unit(byte);
        string routingFile;
        @display("bgb=545,363;i=abstract/router;bgl=2");

    gates:
        input in[];
        output out[];

    submodules:
        queue[sizeof(out)]: FifoQueue {
            @display("p=440,111,c");
            buffer_length = bufferSize;
        }

        queueController[sizeof(out)]: QueueController {

            @display("p=440,206");
        }

        router: FileRouter {
            @display("p=107,111");
            routingFile = routingFile;
        }

    connections:
        for i=0..sizeof(out)-1 {
            router.out++ --> queue[i].in;
            queue[i].out --> out[i];
            queueController[i].out --> queue[i].ctrl;
        }

        for i=0..sizeof(in)-1 {
            in[i] --> router.in++;
        }


}
