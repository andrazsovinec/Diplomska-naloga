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

#include "ClientOut.h"

Define_Module(ClientOut);

ClientOut::ClientOut()
{
    currentlyTransmitting = nullptr;
}

ClientOut::~ClientOut()
{
    cancelAndDelete(currentlyTransmitting);
    delete currentlyTransmitting;
    while (!inputQueue.isEmpty()) {
        cPacket *packet = check_and_cast<cPacket *>(inputQueue.pop());
        //cancelAndDelete( packet );
        delete(packet);
    }
    inputQueue.clear();
}

void ClientOut::initialize()
{
    capacity = par("capacity");
}

void ClientOut::handleMessage(cMessage *msg)
{
    cPacket *packet = check_and_cast<cPacket *>(msg);

    if (packet->isName("Control")) {
        cancelAndDelete(packet);
        currentlyTransmitting = nullptr;
        transmitNextMessage();
    } else {
        if (currentlyTransmitting == nullptr) {
            if (inputQueue.isEmpty()) {
                inputQueue.insert(packet);
                transmitNextMessage();
            }else{
                if(inputQueue.getLength() < capacity){
                    inputQueue.insert(packet);
                }else{
                    cancelAndDelete( packet );
                    //delete(packet);
                }
            }
        } else {
            if(inputQueue.getLength() < capacity){
                inputQueue.insert(packet);
            }else{
                cancelAndDelete( packet );
                //delete(packet);
            }
        }
    }
}

void ClientOut::transmitNextMessage()
{
    if (!inputQueue.isEmpty()) {
        cPacket *packet = check_and_cast<cPacket *>(inputQueue.pop());
        currentlyTransmitting = packet;
        send(currentlyTransmitting, "out");
        cGate *outGate = gate("out");
        cChannel *channel = outGate->getTransmissionChannel();
        simtime_t transmissionFinishTime = channel->getTransmissionFinishTime();
        cPacket *msg = new cPacket("Control");
        scheduleAt(transmissionFinishTime, msg);
    }
}

void ClientOut::finish(){
    while (!inputQueue.isEmpty()) {
        cPacket *packet = check_and_cast<cPacket *>(inputQueue.pop());
        //cancelAndDelete( packet );
        delete(packet);
    }
}
