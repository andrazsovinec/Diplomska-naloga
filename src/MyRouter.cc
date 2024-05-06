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

#include "MyRouter.h"

Define_Module(MyRouter);

MyRouter::MyRouter() {
    //endServiceMsg = NULL;
}

MyRouter::~MyRouter() {
    jobsProcessingList::iterator msg;
    for (msg=jobsProcessing.begin(); msg!=jobsProcessing.end(); msg++)
    {
        //cancelAndDelete(*msg);
        delete(*msg);
    }
    jobsProcessing.clear();
}

void MyRouter::initialize()
{
    droppedSignal = registerSignal("dropped");
    droppedSecSignal = registerSignal("droppedSec");
    waitTimeSignal = registerSignal("waitTime");

    capacity = par("capacity");
    serviceTime = par("serviceTime");
    queue.setName("queue");
    length = 0;
    resources = par("resources");
    processing = 0;
    measureRouters = par("measureRouters");
    jobsProcessing.clear();

    chanceToContinue = par("chanceToContinue");

    //emit(droppedSignal, 0);
    //emit(droppedSecSignal, 0);
    emit(waitTimeSignal, (simtime_t)0);
}

void MyRouter::handleMessage(cMessage *msg)
{
    // The message kind member is not used by OMNeT++, it can be used freely by the user.
    //EV << "Service time: " << serviceTime << "\n";
    // ali je prislo sporocilo o koncu procesiranja
    if (msg->getKind() == 10)
    {

        //EV << "Nardil je enga\n";
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

        job->setKind(0);
        if(msg->isName("PrimaryMessage")){
            send(job, "out");
        }else{
            if(msg->isName("SecondaryMessage")){
                send(job, "outSec");
            }else{
                send(job, "outTer");
            }

        }

        if ( ! queue.isEmpty() )
        {
            job = check_and_cast<cMessage *>( queue.pop() );
            job->setKind(10);
            jobsProcessing.push_back(job);

            scheduleAt( simTime()+serviceTime, job );   // v izvajanje damo novo opravilo, ki se bo izvedlo cez serviceTime casa

            //EV << "Wait time: " << serviceTime << "s" << endl;
            emit( waitTimeSignal, simTime() - job->getTimestamp() );

            processing++;
            length--;
        }
    }
    // ali je prislo novo opravilo
    else
    {
        cMessage *job = check_and_cast<cMessage *>(msg);
        job->setKind(1);

        /*
        if(job->isName("PrimaryMessage")){
            emit( waitTimeSignal, simTime() - job->getTimestamp() );
        }
        */

        if (processing < resources)
        {
            processing ++;
            job->setKind(10);
            jobsProcessing.push_back(job);
            scheduleAt( simTime()+serviceTime, job );
            //EV << "Wait time: " << serviceTime << "s" << endl;
            emit(waitTimeSignal,(simtime_t)0);
        }
        else
        {
            if (capacity >=0 && length >= capacity)
            {
                // cakalna vrsta je presegla svojo kapaciteto
                if(msg->isName("PrimaryMessage")){
                    emit(droppedSignal, 1);
                }else{
                    emit(droppedSecSignal, 1);
                }
                cancelAndDelete( job );
                //delete(job);
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
    updateDisplay(length, processing, resources);
}

void MyRouter::updateDisplay(int i, int p, int r)
{
    char buf[300];
    sprintf(buf, "Q_length :%ld, Resources: %ld/%ld", (long) i, (long) p, (long) r);
    getDisplayString().setTagArg("t",0,buf);
}

void MyRouter::finish(){
    while (!queue.isEmpty()) {
        cMessage *job = check_and_cast<cMessage *>(queue.pop());
        //cancelAndDelete( job );
        delete(job);
    }
}


