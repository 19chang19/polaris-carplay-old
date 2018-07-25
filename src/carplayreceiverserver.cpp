/*
 * carplayreceiverserver.cpp
 *
 *  Created on: Jul 24, 2018
 *     
 */
#include "carplayreceiverserver.h"
#include "carplayclient.h"
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include "StringUtils.h"
#include "AirPlayReceiverServer.h"
#include "AirPlayReceiverSession.h"
#include "AirPlayVersion.h"
#include "AirPlayUtils.h"
#include "DebugServices.h"
#include "HIDKnob.h"
#include "HIDTouchScreen.h"
#include "HIDProximity.h"
#include "MathUtils.h"
#include "AudioUtils.h"
#include "CarPlayControlClient.h"
Carplayreceiverserver::CarPlayReceiverServer()
{
public:

}
Carplayreceiverserver::CarPlayReceiverServerCreate(CarPlayReceiverServerRef *outServer)
{
   return rserv.CarPlayReceiverServerCreateWithConfigFilePath( NULL, outServer );
}
Carplayreceiverserver::CarPlayReceiverServerCreateWithConfigFilePath(CFStringRef inConfigFilePath, CarPlayReceiverServerRef *outServer)
{
	    OSStatus						err;
		CarrPlayReceiverServerRef		me;
		size_t							extraLen;

		dispatch_once_f( &gAirPlayReceiverInitOnce, NULL, _GlobalInitialize );

		extraLen = sizeof( *me ) - sizeof( me->base );
		me = (CarPlayReceiverServerRef) _CFRuntimeCreateInstance( NULL, CarPlayReceiverServerGetTypeID(), (CFIndex) extraLen, NULL );
		require_action( me, exit, err = kNoMemoryErr );
		memset( ( (uint8_t *) me ) + sizeof( me->base ), 0, extraLen );

		me->timeoutDataSecs  = kAirPlayDataTimeoutSecs;

		me->queue = dispatch_queue_create( "CarPlayReceiverServerQueue", 0 );
		me->httpQueue = dispatch_queue_create( "CarPlayReceiverServerHTTPQueue", 0 );

		if( !inConfigFilePath )
		{
			me->configFilePath = strdup( AIRPLAY_CONFIG_FILE_PATH );
		}
		else
		{
			err = CFStringCopyUTF8CString( inConfigFilePath, &me->configFilePath );
			require_noerr( err, exit );
		}
		_InitializeConfig( me );

		RandomBytes( me->httpTimedNonceKey, sizeof( me->httpTimedNonceKey ) );

		err = CarPlayReceiverServerPlatformInitialize( me );
		require_noerr( err, exit );

		*outServer = me;
		me = NULL;
		err = kNoErr;

	exit:
		if( me ) CFRelease( me );
		return( err );
}



