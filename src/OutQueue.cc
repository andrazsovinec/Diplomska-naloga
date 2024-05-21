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

#include "OutQueue.h"

Define_Module(OutQueue);

OutQueue::OutQueue()
{
    currentMessage = nullptr;
}

OutQueue::~OutQueue()
{
    cancelAndDelete(currentMessage);
    //delete currentMessage;
    while (!inputQueue.isEmpty()) {
        cPacket *packet = check_and_cast<cPacket *>(inputQueue.pop());
        cancelAndDelete( packet );
        //delete(packet);
    }
    inputQueue.clear();
}

void OutQueue::initialize()
{
    droppedSignal = registerSignal("dropped");
    waitTimeSignal = registerSignal("waitTime");

    capacity = par("capacity");
    measureRouters = par("measureRouters");
    EV << "Queue capacity: " << capacity << endl;
}

void OutQueue::handleMessage(cMessage *msg)
{
    cPacket *packet = check_and_cast<cPacket *>(msg);

    if (packet->isName("Control")) {
        cancelAndDelete(packet);
        currentMessage = nullptr;
        sendNextMessage();
    } else {
        if (currentMessage == nullptr) {
            if (inputQueue.isEmpty()) {
                // TIMESTAMP
                if(measureRouters){
                    packet->setTimestamp();
                }
                inputQueue.insert(packet);
                sendNextMessage();
            }else{
                if(inputQueue.getLength() < capacity){
                    // TIMESTAMP
                    if(measureRouters){
                        packet->setTimestamp();
                    }
                    inputQueue.insert(packet);
                }else{
                    cancelAndDelete( packet );
                    //delete(packet);
                    emit(droppedSignal, 1);
                }
            }
        } else {
            if(inputQueue.getLength() < capacity){
                // TIMESTAMP
                if(measureRouters){
                    packet->setTimestamp();
                }
                inputQueue.insert(packet);
            }else{
                cancelAndDelete( packet );
                //delete(packet);
                emit(droppedSignal, 1);
            }
        }
    }


}

void OutQueue::sendNextMessage()
{
    if (!inputQueue.isEmpty()) {
        cPacket *packet = check_and_cast<cPacket *>(inputQueue.pop());
        currentMessage = packet;
        emit( waitTimeSignal, simTime() - packet->getTimestamp() );
        send(currentMessage, "out");
        cGate *outputGate = gate("out");
        cChannel *channel = outputGate->getTransmissionChannel();
        simtime_t transmissionFinishTime = channel->getTransmissionFinishTime();
        cPacket *msg = new cPacket("Control");
        scheduleAt(transmissionFinishTime, msg);
    }
}

void OutQueue::finish(){
}

