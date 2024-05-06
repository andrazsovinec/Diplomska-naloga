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

//
// TODO auto-generated module
//
simple Server
{
    parameters:
    	@display("i=device/server");
    	@signal[forwardingSignal](type="simsignal_t");
    	
    	@signal[dropped](type=long);
    	@signal[delivered](type=long);
        @signal[waitTime](type=simtime_t);
        @signal[travelTime](type=simtime_t);
        
        @statistic[dropped](title="drop event";record=vector,count;interpolationmode=none);
        @statistic[delivered](title="delivered packet";record=vector,count;interpolationmode=none);
        @statistic[waitTime](title="queueing time at dequeue";record=vector,mean,min,max;unit=s;interpolationmode=none);
        @statistic[travelTime](title="travel time";record=vector,mean,min,max;unit=s;interpolationmode=none);
    	
    	int capacity = default(40000);
    	int resources = default(1);
    	bool measureRouters = default(false);
    	//double chanceToContinue = default(0.5);      
        //volatile double serviceTime @unit(s) = default( truncnormal(0.0000055s,0.0000015s) );
        volatile double serviceTime @unit(s) = default( 0.0001s );
    
    gates:
        input in[];
}
