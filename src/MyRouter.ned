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

package src;

simple MyRouter
{   
    parameters:
    	@display("i=block/routing");
    	@signal[forwardingSignal](type="simsignal_t");
    	
    	@signal[dropped](type=long);
    	@signal[droppedSec](type=long);
        @signal[waitTime](type=simtime_t);
        
        @statistic[dropped](title="drop event";record=vector,count;interpolationmode=none);
        @statistic[droppedSec](title="secondary drop event";record=vector,count;interpolationmode=none);
        @statistic[waitTime](title="queueing time at dequeue";record=vector,mean,min,max;unit=s;interpolationmode=none);
    	
    	//int capacity = default(2000000);
    	int capacity = default(100000);
    	int resources = default(8);
    	bool measureRouters = default(false);
    	double chanceToContinue = default(0.5);      
        // volatile double serviceTime @unit(s) = default( truncnormal(0.0000055s,0.0000015s) );
        volatile double serviceTime @unit(s) = default( truncnormal(5.5s,1.5s) );
    
    gates:
        input in[];
        output out;
        output outSec;
        output outTer;
}
