/*
 * carplayclient.cpp
 *
 *  Created on: Jul 24, 2018
 
 */

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


void CPClient::carplythread()
{
	err=pthread_create( &gCarPlayThread, NULL, CarPlayThread, NULL );


}
static void * CarPlayThread(void *inarg)
{
	    OSStatus							err;
		CarPlayReceiverServerDelegate		delegate;

		(void) inArg;

		// Create the AirPlay server. This advertise via Bonjour and starts listening for connections.

		err = rserv.CarPlayReceiverServerCreate(&gCarPlayServer);
		require_noerr( err, exit );

		// Register ourself as a delegate to receive server-level events, such as when a session is created.

		AirPlayReceiverServerDelegateInit( &delegate );
		delegate.copyProperty_f		= _AirPlayHandleServerCopyProperty;
		delegate.sessionCreated_f = _AirPlayHandleSessionCreated;
		AirPlayReceiverServerSetDelegate( gAirPlayServer, &delegate );
}


int main(int argc,char **argv)
{

	    int					i;
		const char *		arg;
		OSStatus			err;
		for( i = 1; i < argc; )
		{
			arg = argv[ i++ ];
			if( 0 ) {}
			else if( strcmp( arg, "--no-knob" ) == 0 )
			{
				gKnob = false;
			}
			else if( strcmp( arg, "--no-hi-fi-touch" ) == 0 )
			{
				gHiFiTouch = false;
			}
			else if( strcmp( arg, "--no-lo-fi-touch" ) == 0 )
			{
				gLoFiTouch = false;
			}
			else if( strcmp( arg, "--no-touchpad" ) == 0 )
			{
				gTouchpad = false;
			}
	        else if( strcmp( arg, "--modes" ) == 0 )
			{
				if( i >= argc )
				{
					fprintf( stderr, "error: %s requires a value. For example:\n", arg );
				    fprintf( stderr, "    --modes mainAudio=anytime,anytime\n" );
					fprintf( stderr, "    --modes phoneCall\n" );
					fprintf( stderr, "    --modes speech=speaking\n" );
					fprintf( stderr, "    --modes turnByTurn\n" );
					exit( 1 );
				}
				arg = argv[ i++ ];
				err = _ParseModes( arg );
				if( err ) exit( 1 );
				gHasInitialModes = true;
			}
			CPClient clientobj;
			Carplayreceiverserver rserv;
			clientobj.carplythread();

}



