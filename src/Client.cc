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

#include "Client.h"
#include "SharedCounter.h"

Define_Module(Client);

void Client::initialize()
{
    messageInterval = par("messageInterval").doubleValue();
    expDistribution = par("expDistribution");
    simulationEndTime = par("simulationEndTime");

    double index = getIndex();
    double startDelay = 0.0002;
    if (expDistribution) {
        omnetpp::cPacket *control = new omnetpp::cPacket("Control");
        control->setName("Control");
        scheduleAt(omnetpp::simTime(), control);
    }else{
        scheduleAt(omnetpp::simTime() + (startDelay * index), generateMessage());
    }
}

void Client::handleMessage(omnetpp::cMessage *msg)
{
    //if(simTime() < simulationEndTime){
        if (expDistribution) {
            if(msg->isName("PrimaryMessage")){
                cPacket *packet = check_and_cast<cPacket *>(msg);
                packet->setTimestamp();
                send(packet, "out");
                if(simTime() < 60.0){
                    SharedCounter::totalPacketsGenerated++;
                }
            }else{
                cancelAndDelete(msg);
                simtime_t delay = exponential(0.1);
                if(delay > 1.0){
                    delay = 0.98;
                    SharedCounter::fixedDelays++;
                }
                scheduleAt(omnetpp::simTime() + delay, generateMessage());
                omnetpp::cPacket *control = new omnetpp::cPacket("Control");
                control->setName("Control");
                scheduleAt(omnetpp::simTime() + messageInterval, control);
            }
        } else {
            cPacket *packet = check_and_cast<cPacket *>(msg);
            packet->setTimestamp();
            send(packet, "out");
            if(simTime() < 60.0){
                SharedCounter::totalPacketsGenerated++;
            }
            scheduleAt(omnetpp::simTime() + messageInterval, generateMessage());
        }
    //}
}

omnetpp::cPacket *Client::generateMessage()
{
    omnetpp::cPacket *msg = new omnetpp::cPacket("ClientMessage");
    msg->setName("PrimaryMessage");
    msg->setSrcProcId(0);
    msg->setBitLength(128);
    return msg;
}

