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
        double link_ber = default(0.0);
        double link_per = default(0.0);

        string applicationNodeType;
        string interferenceNodeType;

    submodules:
        ROU1: FileBufferedRouter {
            @display("p=179,216");
            @statistic[sent_seq_R2](title="Test Seq R2"; record=vector);
            @statistic[sent_seq_R1](title="Test Seq R1"; record=vector);
            @statistic[sent_len_R2](title="Test Len R2"; record=vector);
            @statistic[sent_len_R1](title="Test Len R1"; record=vector);
        }
        ROU2: FileBufferedRouter {
            @display("p=533,216");
        }
        T1: <applicationNodeType> like SourceNode {
            @display("p=72,65;i=device/pc3");
            from = "T1";
            to = "R1";
        }
        T2: <interferenceNodeType> like SourceNode {
            @display("p=72,365;i=block/network");
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

        ROU2.out++ --> IdealChannel --> R1.in1;
        ROU2.out++ --> IdealChannel --> R2.in1;
        T1.out1 --> IdealChannel --> ROU1.in++;
        T2.out1 --> IdealChannel --> ROU1.in++;
        ROU1.out++ --> DatarateChannel {  datarate = link_datarate; delay = link_delay; per = link_per; ber = link_ber; } --> ROU2.in++;
}

network ControlledNetwork
{
    parameters:
        @display("bgb=901,567");
        double link_delay @unit(s);
        double link_datarate @unit(bps);
        double link_ber = default(0.0);
        double link_per = default(0.0);

        string applicationNodeType;
        string interferenceNodeType;

    submodules:
        ROU1: ControlledRouter {
            @display("p=179,216");
            @statistic[sent_seq_R2](title="Test Seq R2"; record=vector);
            @statistic[sent_seq_R1](title="Test Seq R1"; record=vector);
            @statistic[sent_len_R2](title="Test Len R2"; record=vector);
            @statistic[sent_len_R1](title="Test Len R1"; record=vector);
        }
        ROU2: ControlledRouter {
            @display("p=533,216");
        }
        T1: <applicationNodeType> like SourceNode {
            @display("p=72,65;i=device/pc3");
            from = "T1";
            to = "R1";
        }
        T2: <interferenceNodeType> like SourceNode {
            @display("p=72,365;i=block/network");
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

        ROU2.out++ --> IdealChannel --> R1.in1;
        ROU2.out++ --> IdealChannel --> R2.in1;
        T1.out1 --> IdealChannel --> ROU1.in++;
        T2.out1 --> IdealChannel --> ROU1.in++;
        ROU1.out++ --> DatarateChannel {  datarate = link_datarate; delay = link_delay; per = link_per; ber = link_ber; } --> ROU2.in++;
}

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


network RoutingTest
{
    parameters:
        @display("bgb=693,373");
        double link_delay @unit(s);
        double link_datarate @unit(bps);


    submodules:
        ROU1: FileBufferedRouter {
            @display("p=79,175");
        }
        ROU5: FileBufferedRouter {
            @display("p=398,247");
        }
        ROU2: FileBufferedRouter {
            @display("p=209,93");
        }
        ROU3: FileBufferedRouter {
            @display("p=209,247");
        }
        ROU4: FileBufferedRouter {
            @display("p=398,93");
        }
        ROU6: FileBufferedRouter {
            @display("p=540,175");
        }
        APP1: CBRNode {
            @display("p=34,54");
        }
        APP2: CBRNode {
            @display("p=34,283");
        }
        SINK1: Receiver {
            @display("p=625,54");
        }
        SINK2: Receiver {
            @display("p=625,283");
        }
    connections:
        ROU1.out++ --> DatarateChannel {  datarate = link_datarate; delay = link_delay; } --> ROU2.in++;
        ROU1.out++ --> DatarateChannel {  datarate = link_datarate; delay = link_delay; } --> ROU3.in++;
        ROU2.out++ --> DatarateChannel {  datarate = link_datarate; delay = link_delay; } --> ROU5.in++;
        ROU3.out++ --> DatarateChannel {  datarate = link_datarate; delay = link_delay; } --> ROU4.in++;
        ROU2.out++ --> DatarateChannel {  datarate = link_datarate; delay = link_delay; } --> ROU4.in++;
        ROU3.out++ --> DatarateChannel {  datarate = link_datarate; delay = link_delay; } --> ROU5.in++;
        ROU4.out++ --> DatarateChannel {  datarate = link_datarate; delay = link_delay; } --> ROU6.in++;
        ROU5.out++ --> DatarateChannel {  datarate = link_datarate; delay = link_delay; } --> ROU6.in++;
        APP1.out1 --> IdealChannel --> ROU1.in++;
        APP2.out1 --> IdealChannel --> ROU1.in++;
        ROU6.out++ --> IdealChannel --> SINK1.in1;
        ROU6.out++ --> IdealChannel --> SINK2.in1;
}

network FifoQueueTest
{
    parameters:
        double link_delay @unit(s);
        double link_datarate @unit(bps);

        @display("bgb=364,204");
    submodules:
        FIFO: FifoQueue {
            @display("p=124,92");
        }
        CBR1: CBRNode {
            @display("p=29,92");
        }
        RECEIVER: Receiver {
            @display("p=237,92");
        }
    connections:
        CBR1.out1 --> FIFO.in;
        FIFO.out --> DatarateChannel {  datarate = link_datarate; delay = link_delay; } --> RECEIVER.in1;
}

//
// TODO documentation
//
// @author hugo
//
network FileRouterTest
{
    @display("bgb=463,383");
    submodules:
        fileRouter: FileRouter {
            @display("p=192,157");
        }
        CBR1: CBRNode {
            @display("p=44,74");
        }
        CBR2: CBRNode {
            @display("p=44,157");
        }
        CBR3: CBRNode {
            @display("p=44,243");
        }
        R1: Receiver {
            @display("p=314,74");
        }
        R2: Receiver {
            @display("p=314,157");
        }
        R3: Receiver {
            @display("p=314,243");
        }
    connections:
        CBR1.out1 --> fileRouter.in++;
        CBR2.out1 --> fileRouter.in++;
        CBR3.out1 --> fileRouter.in++;
        fileRouter.out++ --> R1.in1;
        fileRouter.out++ --> R2.in1;
        fileRouter.out++ --> R3.in1;
}

//
// TODO documentation
//
// @author hugo
//
network FileBufferedRouterTest
{
    parameters:
        double link_delay @unit(s);
        double link_datarate @unit(bps);

        @display("bgb=470,265");
    submodules:
        FBROUTER2: FileBufferedRouter {
            @display("p=160,173");
        }
        CBR2: CBRNode {
            @display("p=45,113");
        }
        CBR3: CBRNode {
            @display("p=45,181");
        }
        FBROUTER1: FileBufferedRouter {
            @display("p=160,74");
        }
        CBR1: CBRNode {
            @display("p=45,42");
        }
        R1: Receiver {
            @display("p=406,74");
        }
        R2: Receiver {
            @display("p=406,173");
        }
        FBROUTER3: FileBufferedRouter {
            @display("p=278,113");
        }
    connections:
        CBR1.out1 --> FBROUTER1.in++;
        CBR2.out1 --> FBROUTER1.in++;
        CBR3.out1 --> FBROUTER2.in++;
        FBROUTER1.out++ --> DatarateChannel {  datarate = link_datarate; delay = link_delay; } --> FBROUTER3.in++;
        FBROUTER2.out++ --> DatarateChannel {  datarate = link_datarate; delay = link_delay; } --> FBROUTER3.in++;
        FBROUTER3.out++ --> DatarateChannel {  datarate = link_datarate; delay = link_delay; } --> R1.in1;
        FBROUTER3.out++ --> DatarateChannel {  datarate = link_datarate; delay = link_delay; } --> R2.in1;
}

//
// TODO documentation
//
// @author hugo
//
network ControlledRouterTest
{
    parameters:
        @display("bgb=470,191");
        double link_delay @unit(s);
        double link_datarate @unit(bps);

    submodules:
        receiver: Receiver {
            @display("p=398,110");
        }
        fileInputNode: FileInputNode {
            @display("p=76,110");
        }
        controlledRouter: ControlledRouter {
            @display("p=236,110");
        }
    connections:
        fileInputNode.out1 --> controlledRouter.in++;
        controlledRouter.out++ --> DatarateChannel {  datarate = link_datarate; delay = link_delay; } --> receiver.in1;
}

