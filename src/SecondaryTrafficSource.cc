#include "SecondaryTrafficSource.h"

Define_Module(SecondaryTrafficSource);

void SecondaryTrafficSource::initialize()
{
    messageInterval = par("secondaryMessageInterval").doubleValue();
    expDistribution = par("secondaryUseExponentialDistribution");
    generatedSignal = registerSignal("generated");

    scheduleAt(omnetpp::simTime(), generateMessage());
}

void SecondaryTrafficSource::handleMessage(cMessage *msg)
{
    if(msg->isName("OutsideResponse")){
        cancelAndDelete(msg);
    }else{
        cPacket *packet = check_and_cast<cPacket *>(msg);
        packet->setTimestamp();
        emit(generatedSignal, 1);
        send(packet, "out");
        scheduleAt(omnetpp::simTime() + exponential(messageInterval), generateMessage());
    }
}

omnetpp::cPacket *SecondaryTrafficSource::generateMessage()
{
    omnetpp::cPacket *msg = new omnetpp::cPacket("SecondaryMessage");
    msg->setName("SecondaryMessage");
    msg->setSrcProcId(0);
    msg->setBitLength(12000);
    msg->setTimestamp();
    return msg;
}

