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

#ifndef __BACKBONENETWORK_SERVER_H_
#define __BACKBONENETWORK_SERVER_H_

#include <omnetpp.h>

using namespace omnetpp;

class Server : public omnetpp::cSimpleModule
{
  private:
    simsignal_t droppedSignal;
    simsignal_t deliveredSignal;
    simsignal_t waitTimeSignal;
    simsignal_t travelTimeSignal;
    
    cQueue queue;
    int capacity, length;
    int resources, processing;
    bool measureRouters;
    simtime_t serviceTime;

    typedef std::list<cMessage *> jobsProcessingList;
    jobsProcessingList jobsProcessing;
  public:
      Server();
      virtual ~Server();
  protected:
    virtual void initialize() override;
    virtual void handleMessage(omnetpp::cMessage *msg) override;
    virtual void finish() override;
};

#endif

