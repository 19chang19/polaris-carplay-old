/*
 * carplayreceiverserver.h
 *
 *  Created on: Jul 24, 2018
 *     
 */

#ifndef CARPLAYCLIENT_CARPLAYRECEIVERSERVER_H_
#define CARPLAYCLIENT_CARPLAYRECEIVERSERVER_H_
#include "carplayreceiverserverpriv.h"

typedef struct CarPlayReceiverServerPrivate *		CarPlayReceiverServerRef;
typedef struct CarPlayReceiverSessionPrivate *		CarPlayReceiverSessionRef;

//---------------------------------------------------------------------------------------------------------------------------
/*!	@group		AirPlayReceiverServerDelegate
	@abstract	Allows functionality to be delegated to external code.
*/
typedef OSStatus
	( *CarPlayReceiverServerControl_f )(
		CarPlayReceiverServerRef	inServer,
		CFStringRef					inCommand,
		CFTypeRef					inQualifier,
		CFDictionaryRef				inParams,
		CFDictionaryRef *			outParams,
		void *						inContext );
typedef CFTypeRef
	( *CarPlayReceiverServerCopyProperty_f )(
		CarPlayReceiverServerRef	inServer,
		CFStringRef					inProperty,
		CFTypeRef					inQualifier,
		OSStatus *					outErr,
		void *						inContext );
typedef OSStatus
	( *CarPlayReceiverServerSetProperty_f )(
		CarPlayReceiverServerRef	inServer,
		CFStringRef					inProperty,
		CFTypeRef					inQualifier,
		CFTypeRef					inValue,
		void *						inContext );
typedef void
	( *CarPlayReceiverSessionCreated_f )(
		CarPlayReceiverServerRef	inServer,
		CarPlayReceiverSessionRef	inSession,
		void *						inContext );
typedef void
	( *CarPlayReceiverSessionFailed_f )(
		CarPlayReceiverServerRef	inServer,
		OSStatus					inReason,
		void *						inContext );

typedef struct
{
	void *									context;			// Context pointer for the delegate to use.
	void *									context2;			// Extra context pointer for the delegate to use.
	CarPlayReceiverServerControl_f			control_f;			// Function to call for control requests.
	CarPlayReceiverServerCopyProperty_f		copyProperty_f;		// Function to call for copyProperty requests.
	CarPlayReceiverServerSetProperty_f		setProperty_f;		// Function to call for setProperty requests.
	CarPlayReceiverSessionCreated_f			sessionCreated_f;	// Function to call when a session is created.
	CarPlayReceiverSessionFailed_f			sessionFailed_f;	// Function to call when a session creation fails.

}	CarPlayReceiverServerDelegate;
class Carplayreceiverserver
{
public:
	OSStatus CarPlayReceiverServerCreate( CarPlayReceiverServerRef *outServer );
	OSStatus CarPlayReceiverServerCreateWithConfigFilePath( CFStringRef inConfigFilePath, CarPlayReceiverServerRef *outServer );
};


#endif /* CARPLAYCLIENT_CARPLAYRECEIVERSERVER_H_ */
