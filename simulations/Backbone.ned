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

package simulations;

import inet.node.ethernet.*;
import ned.DatarateChannel;
import src.MyRouter;
import src.NetworkModule;
import src.Server;
import src.Client;
import src.ClientOut;
import src.OutQueue;
//import src.NetworkModule;
import src.SecondaryTrafficSource;
import src.OutsideSecondary;

network Backbone
{
    parameters:
        int numClients = 5000;
        @display("bgb=1767.8325,884.7525");
    submodules:
        router: MyRouter {
            @display("p=294.36002,409.7625");
        }
        router1: MyRouter {
            @display("p=508.44,408.09003");
        }
        router2: MyRouter {
            @display("p=781.0575,408.09003;b=12.099999,5.5");
        }
        router3: MyRouter {
            @display("p=1053.745,407.33");
        }
        server: Server {
            @display("p=1350.3875,409.54373");
        }
        clients[numClients]: Client {
            @display("p=61.984997,409.54373");
        }
        outClients[numClients]: ClientOut {
            @display("p=148.85251,409.7625");
        }
        queue01: OutQueue {
            @display("p=359.58752,409.7625");
        }
        queue02: OutQueue {
            @display("p=573.66754,408.09003");
        }
        queue03: OutQueue {
            @display("p=846.28503,409.7625");
        }
        queue04: OutQueue {
            @display("p=1120.5751,409.7625");
        }
        queue05: OutQueue {
            @display("p=294.36002,304.39502");
        }
        queue06: OutQueue {
            @display("p=508.44,304.39502");
        }
        queue07: OutQueue {
            @display("p=779.385,304.39502");
        }
        queue08: OutQueue {
            @display("p=1055.3475,304.39502");
        }
        S1: SecondaryTrafficSource {
            @display("p=292.215,781.45374");
        }
        S2: SecondaryTrafficSource {
            @display("p=506.94873,781.45374");
        }
        S3: SecondaryTrafficSource {
            @display("p=779.24,781.45374");
        }
        S4: SecondaryTrafficSource {
            @display("p=1053.745,781.45374");
        }
        queue09: OutQueue {
            @display("p=292.215,537.9412");
        }
        queue10: OutQueue {
            @display("p=506.94873,537.9412");
        }
        queue11: OutQueue {
            @display("p=779.24,537.9412");
        }
        queue12: OutQueue {
            @display("p=1053.745,537.9412");
        }
        O1: OutsideSecondary {
            @display("p=292.215,101.8325");
        }
        O2: OutsideSecondary {
            @display("p=506.94873,101.8325");
        }
        O3: OutsideSecondary {
            @display("p=779.24,101.8325");
        }
        O4: OutsideSecondary {
            @display("p=1053.745,101.8325");
        }
        queue13: OutQueue {
            @display("p=292.215,695.1175");
        }
        queue14: OutQueue {
            @display("p=506.94873,695.1175");
        }
        queue15: OutQueue {
            @display("p=779.24,695.1175");
        }
        queue16: OutQueue {
            @display("p=1053.745,695.1175");
        }
        queue17: OutQueue {
            @display("p=292.215,188.16875");
        }
        queue18: OutQueue {
            @display("p=506.94873,188.16875");
        }
        queue19: OutQueue {
            @display("p=779.24,188.16875");
        }
        queue20: OutQueue {
            @display("p=1053.745,188.16875");
        }
    connections:

        router.out --> queue01.in;
        router1.out --> queue02.in;
        router2.out --> queue03.in;
        router.outSec --> queue05.in;
        router1.outSec --> queue06.in;
        router2.outSec --> queue07.in;
        for i=0..numClients-1 {
            clients[i].out --> outClients[i].in;
            outClients[i].out --> {  datarate = 50Mbps; } --> router.in++;
        }
        queue01.out --> Eth10G {  length = 1000m; } --> router1.in++;
        queue02.out --> Eth10G {  length = 3000m; } --> router2.in++;
        queue03.out --> Eth10G {  length = 1000m; } --> router3.in++;
        S1.out --> queue13.in;
        S2.out --> queue14.in;
        S3.out --> queue15.in;
        S4.out --> queue16.in;
        O1.out --> queue17.in;
        O2.out --> queue18.in;
        O3.out --> queue19.in;
        O4.out --> queue20.in;
        queue13.out --> Eth10G --> router.in++;
        queue14.out --> Eth10G --> router1.in++;
        queue15.out --> Eth10G --> router2.in++;
        queue16.out --> Eth10G --> router3.in++;
        queue17.out --> Eth10G --> router.in++;
        queue18.out --> Eth10G --> router1.in++;
        queue19.out --> Eth10G --> router2.in++;
        queue20.out --> Eth10G --> router3.in++;
        queue05.out --> Eth10G --> O1.in;
        queue06.out --> Eth10G --> O2.in;
        queue07.out --> Eth10G --> O3.in;
        queue08.out --> Eth10G --> O4.in;
        queue04.out --> Eth1G --> server.in++;
        router3.out --> queue04.in;
        router3.outSec --> queue08.in;
        router.outTer --> queue09.in;
        router1.outTer --> queue10.in;
        router2.outTer --> queue11.in;
        router3.outTer --> queue12.in;
        queue09.out --> Eth10G --> S1.in;
        queue10.out --> Eth10G --> S2.in;
        queue11.out --> Eth10G --> S3.in;
        queue12.out --> Eth10G --> S4.in;
}
