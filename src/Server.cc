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

#include "Server.h"
#include "SharedCounter.h"

Define_Module(Server);

Server::Server() {
    //endServiceMsg = NULL;
}

Server::~Server() {

    while (!queue.isEmpty()) {
        cMessage *job = check_and_cast<cMessage *>(queue.pop());
        cancelAndDelete( job );
        //delete(job);
    }

    jobsProcessingList::iterator msg;
    for (msg=jobsProcessing.begin(); msg!=jobsProcessing.end(); msg++)
    {
        cancelAndDelete(*msg);
    }
    jobsProcessing.clear();
}

void Server::initialize()
{
    droppedSignal = registerSignal("dropped");
    deliveredSignal = registerSignal("delivered");
    waitTimeSignal = registerSignal("waitTime");
    travelTimeSignal = registerSignal("travelTime");

    capacity = par("capacity");
    serviceTime = par("serviceTime");
    queue.setName("queue");
    length = 0;
    resources = par("resources");
    processing = 0;
    measureRouters = par("measureRouters");
    jobsProcessing.clear();
    SharedCounter::successfulDeliveries = 0;
}

void Server::handleMessage(omnetpp::cMessage *msg)
{
    EV << "Service time: " << serviceTime << "\n";
        if (msg->getKind() == 10)
        {

            EV << "Nardil je enga\n";
            cMessage *job = check_and_cast<cMessage *>(msg);

            jobsProcessingList::iterator msgIterator;
            for (msgIterator=jobsProcessing.begin(); msgIterator!=jobsProcessing.end(); msgIterator++)
            {
                if ((*msgIterator)->getId()==job->getId())
                {
                    jobsProcessing.erase(msgIterator);
                    processing--;
                    break;
                }

            }

            emit( travelTimeSignal, simTime() - msg->getTimestamp() );
            cancelAndDelete(msg);
            //delete(msg);
            SharedCounter::successfulDeliveries++;
            emit(deliveredSignal, 1);

            if ( ! queue.isEmpty() )
            {
                job = check_and_cast<cMessage *>( queue.pop() );
                job->setKind(10);
                jobsProcessing.push_back(job);

                scheduleAt( simTime()+serviceTime, job );

                EV << "Wait time: " << serviceTime << "s" << endl;
                emit( waitTimeSignal, simTime() - job->getTimestamp() );

                processing++;
                length--;
            }
        }
        else
        {
            cMessage *job = msg;
            job->setKind(1);

            if (processing < resources)
            {
                processing ++;
                job->setKind(10);
                jobsProcessing.push_back(job);
                scheduleAt( simTime()+serviceTime, job );
                EV << "Wait time: " << serviceTime << "s" << endl;
                emit(waitTimeSignal,(simtime_t)0);
            }
            else
            {
                if (capacity >=0 && length >= capacity)
                {
                    if(msg->isName("PrimaryMessage")){
                        emit(droppedSignal, 1);
                    }
                    cancelAndDelete( job );
                }
                else
                {
                    // TIMESTAMP
                    if(measureRouters){
                        job->setTimestamp();
                    }
                    queue.insert( job );
                    length++;
                }
            }
        }
}


void Server::finish() {
    EV << "Successful Deliveries at Server: " << SharedCounter::successfulDeliveries << endl;
    EV << "Total amount of generated packets: " << SharedCounter::totalPacketsGenerated << endl;
    EV << "Total amount of fixed delays: " << SharedCounter::fixedDelays << endl;
}
