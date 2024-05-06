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

#include "NetworkModule.h"

Define_Module(NetworkModule);

void NetworkModule::initialize()
{
    for (int i = 0; i < 3; ++i)
    {
        cModuleType *clientType = cModuleType::find("src.Empty");
        cModule *client = clientType->create("empty", this);
    }
}

void NetworkModule::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}
