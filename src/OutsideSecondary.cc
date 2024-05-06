#include "OutsideSecondary.h"

Define_Module(OutsideSecondary);

void OutsideSecondary::initialize()
{
    messageInterval = (par("secondaryMessageInterval").doubleValue()) / 4;
    expDistribution = par("secondaryUseExponentialDistribution");
    generatedSignal = registerSignal("generated");

    scheduleAt(omnetpp::simTime(), generateMessage());
}

void OutsideSecondary::handleMessage(cMessage *msg)
{
    if(msg->isName("SecondaryMessage")){
        cancelAndDelete(msg);
    }else{
        cPacket *packet = check_and_cast<cPacket *>(msg);
        packet->setTimestamp();
        emit(generatedSignal, 1);
        send(packet, "out");
        scheduleAt(omnetpp::simTime() + exponential(messageInterval), generateMessage());
    }
}

omnetpp::cPacket *OutsideSecondary::generateMessage()
{
    omnetpp::cPacket *msg = new omnetpp::cPacket("OutsideMessage");
    msg->setName("OutsideResponse");
    msg->setSrcProcId(0);
    msg->setBitLength(12000);
    msg->setTimestamp();
    return msg;
}
