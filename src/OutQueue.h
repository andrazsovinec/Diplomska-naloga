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

#ifndef __BACKBONENETWORK_QUEUE_H_
#define __BACKBONENETWORK_QUEUE_H_

#include <omnetpp.h>

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class OutQueue : public cSimpleModule
{
  private:
    simsignal_t droppedSignal;
    simsignal_t waitTimeSignal;
    simsignal_t queueLengthSignal;
    simsignal_t utilizationSignal;

    cQueue inputQueue;
    cPacket *currentMessage;
    bool measureRouters;
    int capacity;

  public:
    OutQueue();
    virtual ~OutQueue();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void sendNextMessage();
    virtual void finish() override;
};

#endif
