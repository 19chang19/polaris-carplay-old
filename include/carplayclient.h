/*
 * carplayclient.h
 *
 *  Created on: Jul 24, 2018
 *      
 */

#ifndef CARPLAYCLIENT_CARPLAYCLIENT_H_
#define CARPLAYCLIENT_CARPLAYCLIENT_H_
#include <pthread.h>
#include <stdio.h>
#include "carplayreceiverserver.h"
static bool							gKnob			= true;
static bool							gHiFiTouch		= true;
static bool							gLoFiTouch		= true;
static bool							gTouchpad		= true;
static CarPlayReceiverServerRef		gCarPlayServer	= NULL;
typedef int32_t			OSStatus;

class CPClient
{
  public:
	OSStatus		err;
	void carplythread();

};


#endif /* CARPLAYCLIENT_CARPLAYCLIENT_H_ */
