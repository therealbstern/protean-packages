/*
 *  TCPEvent.cpp
 *
 *  Created by Ian Taylor on 11/01/2007.
 *
 */
 
#include "TCPEvent.h"
 
TCPEvent::TCPEvent(Event theType, void* theData, int theDataSize) {
	type=theType;
	data=theData;
	dataSize=theDataSize;
	}  