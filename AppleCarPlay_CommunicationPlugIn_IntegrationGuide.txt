/*
	File:    	AppleCarPlay_CommunicationPlugIn_IntegrationGuide.txt
	Package: 	Apple CarPlay Communication Plug-in.
	Abstract: 	n/a 
	Version: 	n/a
	
	Disclaimer: IMPORTANT: This Apple software is supplied to you, by Apple Inc. ("Apple"), in your
	capacity as a current, and in good standing, Licensee in the MFi Licensing Program. Use of this
	Apple software is governed by and subject to the terms and conditions of your MFi License,
	including, but not limited to, the restrictions specified in the provision entitled ”Public 
	Software”, and is further subject to your agreement to the following additional terms, and your 
	agreement that the use, installation, modification or redistribution of this Apple software
	constitutes acceptance of these additional terms. If you do not agree with these additional terms,
	please do not use, install, modify or redistribute this Apple software.
	
	Subject to all of these terms and in consideration of your agreement to abide by them, Apple grants
	you, for as long as you are a current and in good-standing MFi Licensee, a personal, non-exclusive 
	license, under Apple's copyrights in this original Apple software (the "Apple Software"), to use, 
	reproduce, and modify the Apple Software in source form, and to use, reproduce, modify, and 
	redistribute the Apple Software, with or without modifications, in binary form. While you may not 
	redistribute the Apple Software in source form, should you redistribute the Apple Software in binary
	form, you must retain this notice and the following text and disclaimers in all such redistributions
	of the Apple Software. Neither the name, trademarks, service marks, or logos of Apple Inc. may be
	used to endorse or promote products derived from the Apple Software without specific prior written
	permission from Apple. Except as expressly stated in this notice, no other rights or licenses, 
	express or implied, are granted by Apple herein, including but not limited to any patent rights that
	may be infringed by your derivative works or by other works in which the Apple Software may be 
	incorporated.  
	
	Unless you explicitly state otherwise, if you provide any ideas, suggestions, recommendations, bug 
	fixes or enhancements to Apple in connection with this software (“Feedback”), you hereby grant to
	Apple a non-exclusive, fully paid-up, perpetual, irrevocable, worldwide license to make, use, 
	reproduce, incorporate, modify, display, perform, sell, make or have made derivative works of,
	distribute (directly or indirectly) and sublicense, such Feedback in connection with Apple products 
	and services. Providing this Feedback is voluntary, but if you do provide Feedback to Apple, you 
	acknowledge and agree that Apple may exercise the license granted above without the payment of 
	royalties or further consideration to Participant.
	
	The Apple Software is provided by Apple on an "AS IS" basis. APPLE MAKES NO WARRANTIES, EXPRESS OR 
	IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY 
	AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR
	IN COMBINATION WITH YOUR PRODUCTS.
	
	IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
	PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION 
	AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
	(INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE 
	POSSIBILITY OF SUCH DAMAGE.
	
	Copyright (C) 2007-2017 Apple Inc. All Rights Reserved. Not to be used or disclosed without permission from Apple.
*/

Apple CarPlay 
Communication Plug-in, R14G17 (06/19/2017)

=======
SOURCES
=======

The reference implementation sources are organized into five folders:
  - AccessorySDK: CoreUtils library with utilities and abstractions for OS and low-level services, such as 
                  MFiServer, Bonjour advertisement and discovery, network utilities, encryption, CFLite, DispatchLite.
                  The external directory contains public domain implementations of curve22519-donna, ed25519,
                  Gladman AES, LibTomMath, SRP, and Small25519.
  - PlatformPOSIX: High level sample Makefile
  - Platform: Stub DLL interfaces for Audio, Screen, AudioConverter, sample HID descriptors, example AudioUtils for ALSA
  - Sources: High-level objects that implement the Apple CarPlay protocol. AirPlayReceiverServer.h,
             AirPlayReceiverSession.h, and CarPlayControl.h provide interfaces for integrating with
             other head unit functionality.
  - Support: APIs for CarPlay session, screen, audio, and control.

Additionally, there is a basic sample application in the Examples folder.
Sources in the Platform folder should be modified as necessary.

====================
COMPILE-TIME OPTIONS
====================

By default, all code is compiled into a shared library with dynamically loaded libraries for AudioUtils, ScreenUtils, and
AudioConverter.

AccessorySDK is recursively invoked from the top-level Makefile of the plug-in, so no additional build steps are required.

Please refer to PlatformPOSIX/Makefile for more details and descriptions of all compile-time options.

Additional compile time options are supported.
  -D AIRTUNES_DYNAMIC_PORTS - Use dynamic or static ports defined in AirPlayCommon.h
  -D AUDIO_STREAM_DLL_PATH - Use a custom AudioStream DLL path & filename
  -D SCREEN_STREAM_DLL_PATH - Use a custom ScreenStream DLL path & filename
  -D AUDIO_CONVERTER_DLL_PATH - Use a custom AudioConverter DLL path & filename

=======
BONJOUR
=======

AirPlay requires a Bonjour implementation. By default, it links against libdns_sd.so and requires mdnsd to be running 
concurrently with the Apple CarPlay implementation.
See http://developer.apple.com/bonjour for more details.
See AppleCarPlay_CommunicationPlugIn_Bonjour.txt for more details on Bonjour requirements specific to Apple CarPlay.


==================
PORTING QUICKSTART
==================

The sources in the Support folder provide an abstraction layer for platform-specific services. The reference implementation 
has already been ported to multiple platforms, so in general, very few, if any, changes will be required. The stub implementations
are located in the Platform folder. These stub implementations need to be modified:
  - AudioUtils: AudioStream interacts with a platform's audio subsystem, and implements APIs for setting up audio 
                input/output, registering callbacks for audio data, and providing audio feedback information. Buffer sizes for
                the audio callback I/O must be less than 10ms. If this requirement cannot be met, then additional buffering needs 
                to be implemented between the audio callback and the platform's audio layer.
  - ScreenUtils: ScreenStream interacts with a platform's video decoder and display subsystem. It implements APIs for 
                 setting properties, starting, stopping, and providing screen data in the form of an H.264 bit stream.
  - HID: Each HID file contains a sample HID descriptor for a particular virtual HID type.  There are samples for Knob, Buttons,
         Telephony, TouchScreen, and TouchPad.  The accessory should include what is applicable to the system and should make 
         necessary HID descriptor changes to match their input methods.
  - AudioConverter: AudioConverter interacts with a platform's audio decoders/encoders.  It implements APIs for initializing
                    a codec, setting up properties for a particular audio stream, and providing data to be pushed into the 
                    codec.  Codec implementation is only required for wireless CarPlay.

Some hardware platforms may require minor modifications to:
  - MFiServerPlatform: AirPlay accessories are required to have an MFi authentication IC so the controller can authenticate 
                       and set up encrypted communication with the accessory. The authentication and encryption protocol is 
                       provided as portable code that should work on any platform. Access to the MFi authentication IC is 
                       different for each platform. Communicating with the MFi auth IC via I2C can be tricky. Unlike some I2C
                       devices, when the MFi auth IC cannot respond quickly, such as when it performing cryptographic 
                       operations, instead of clock stretching, it will not ack requests. It also goes into a sleep mode
                       soon after using it (e.g. within 250 ms) and may not respond to initial requests. Because of these 
                       issues, it's necessary to implement a robust error detection and retry scheme.

A few platforms may require alternate implementations of the following:
  - AtomicUtils: Provides lock-free atomic operations, such as incrementing an integer and compare-and-swap.
  - RandomNumberUtils: Random numbers are used for cryptographic key exchange and various other purposes. All random numbers 
                       go through a single API, RandomBytes(). This API generates N bytes of cryptographic random data. 
                       This implementation must generate cryptographic random numbers (e.g., can't use ANSI rand() or 
                       POSIX random()). If there is not enough entropy to satisfy the request, the function must wait until 
                       sufficient entropy is available (it should not return an error in that case).
  - TickUtils: Provides high resolution timing. This is based on a simple tick counter, such as the processor cycle counter 
               in many CPUs.

You must register the following delegates to provide information to the controller and handle events based on the protocol.
  - AirPlayReceiverServerDelegate:
      control: interface to handle server control requests
      copyProperty: interface to retrieve static information about the platform and its features: audio formats, audio latencies,
                     supported features, icons, displays, HID devices.
      sessionCreated: interface to notify when a successful session is established - set up the AirPlayReceiverSessionDelegate
      sessionFailed: interface to notify of a session failure
  - AirPlayReceiverSessionDelegate:
      initialize: interface to notify when a session's platform context has been initialized successfully
      finalize: interface to notify when a session has ended
      started: interface to notify when a session has started
      control: interface to handle session control requests
      copyProperty: interface to retrieve information about the platform: initial modes
      modesChanged: interface to modes changes from the controller
      requestUI: interface to handle accessory UI requests from the controller
      duckAudio: interface to handle audio ducking requests
      unduckAudio: interface to handle audio unducking requests

Other interaction with the controller occurs using a core set of APIs:
  - AirPlayReceiverSessionChangeModes + helper functions: sends a mode change request
  - AirPlayReceiverSessionRequestUI: sends request UI command
  - AirPlayReceiverSessionSetNightMode: sends night mode command
  - AirPlayReceiverSessionSetLimitedUI: sends limited UI command
  - AirPlayReceiverSessionForceKeyFrame: sends request for key frame
  - AirPlayReceiverSessionRequestSiriAction: sends Siri command
  - AirPlayReceiverSessionSendiAPMessage: sends iAP message
  - AirPlayReceiverSessionUpdateVehicleInformation: sends vehicle information command
  - AirPlayReceiverSessionSendHIDReport: sends HID report

======================
INTEGRATION QUICKSTART
======================

The reference implementation relies heavily on Core Foundation, which implements a limited object model in C. A cross-
platform implementation is provided in the Support folder as part of the reference implementation. Working knowledge of Core
Foundation concepts is strongly encouraged:
  http://developer.apple.com/library/mac/documentation/CoreFoundation/Conceptual/CFDesignConcepts/CFDesignConcepts.html

The sample client application AppleCarPlay_AppStub.c illustrates how key features of Apple CarPlay can be integrated into 
the accessory software. Specifically, it illustrates the steps typically performed at application initialization:
  - Configure and provide screen information through the AirPlayReceiverServer delegate
  - Configure and provide all HID devices through the AirPlayReceiverServer delegate
  - Create an AirPlayReceiverServer (AirPlayReceiverServer.h) and register a delegate to be notified when a session is
    established with the accessory from the controller.
  - Create a single CarPlayControlClient (CarPlayControlClient.h) instance and invoke the CarPlayControl APIs to initiate a session.

When a session is initialized:
  - Register a delegate (AirPlayReceiverSession.h) to be notified of controller-initiated state changes (e.g., mode changes,
    requestUI).
  - Accessory-initiated state changes can be requested via AirPlayReceiverSession APIs.
  - The completion callbacks are done on the AirPlayReceiverSessionEventQueue for the following APIs:
    AirPlayReceiverSessionChangeModes
    AirPlayReceiverSessionForceKeyFrame
    AirPlayReceiverSessionRequestSiriAction
    AirPlayReceiverSessionRequestUI
    AirPlayReceiverSessionSetNightMode
    AirPlayReceiverSessionSetLimitedUI
    AirPlayReceiverSessionSendiAPMessage
    AirPlayReceiverSessionUpdateVehicleInformation
    AirPlayReceiverSessionSendHIDReport

Once a session is established, ScreenStream (ScreenUtils.h), AudioStream (AudioUtils.h), AudioConverter (AudioConverter.h) objects
will be dynamically created (and destroyed) by AirPlay:
  - ScreenStream: A ScreenStream instance will be created when the controller successfully takes or borrows the screen 
                  resource. The AirPlayReceiverSession delegate will be informed of the mode change. The ScreenStream 
                  instance will be released when control of the screen returns to the accessory.
                  See ScreenUtilsStub.c for additional details.
  - AudioStream: An AudioStream instance will be created when the controller successfully takes or borrows the main audio
                 resource. The AirPlayReceiverSession delegate will be informed of the mode change. The AudioStream instance 
                 will be released when control of main audio returns to the accessory.
                 An AudioStream instance will also be created when the controller wishes to play to the alternate audio 
                 stream. In this case, there is no accompanying mode change. The AudioStream instance will be released
                 some time after the controller has no more audio to play.
                 An AudioStream() will never be reconfigured (e.g., format, latency) after it has been started 
                 (AudioStreamStart()).
                 See AudioUtilsStub.c for additional details.
  - AudioConverter: The AudioConverter APIs will be invoked during Wireless CarPlay sessions when a new audio stream is
                    initialized. The accessory will need initialize the codec with the provided stream information such as
                    input/output format, number of channels, sample rate, and packet size.  Once a stream and codec has been
                    initialized, the AudioConverterFillComplexBuffer() will be invoked repeatedly while the stream is active.
                    See AudioConverterStub.c for additional details.

Audio/video synchronization is managed by the Apple device, using information provided by the accessory:
  - Wall clock synchronization via NTP: This is handled internally by the reference implementation.
  - Audio timestamps: The accessory must supply accurate audio timestamps in the form of { sampleTime, hostTime } pairs
  					  each time it calls the plugin's audio callback to read audio data. SampleTime should be a (reasonably) 
  					  current running total of the number of samples that have hit the speaker since AudioStreamStart() was called. 
  					  HostTime is the system time, in units of ticks, corresponding to SampleTime (see TickUtils.h). These values
					  must be sampled in a consistent manner.
					  Effectively the plugin monitors ((currentSampleTime - previousSampleTime) * TicksPerSecond / SampleRate) and 
					  (currentHostTimeTicks - previousHostTimeTicks). These two differences should be very close to each other.
					  This information will be returned to the controller and is a key piece in allowing the controller to derive 
					  the relationship between the controller's system clock and the accessory's audio (DAC) clock for A/V sync.
					  To verify a successful implementation the accessory must provide a set of continuous logs containing the following values:
		    		    - audio input: ((currentInputSampleTime - previousInputSampleTime) * TicksPerSecond / SampleRate) ; (currentHostTimeTicks - previousHostTimeTicks)
		    			- audio output: ((currentOutputSampleTime - previousOutputSampleTime) * TicksPerSecond / SampleRate) ; (currentHostTimeTicks - previousHostTimeTicks)
		    		  The logs must be taken in the following scenarios:
						- at least 1 min of continuous Siri interaction
						- at least 3 min of an active phone call
						- at least 3 min of an active FaceTime Audio call
						- at least 5 min of music playback.
  - Video timestamps: The accessory must the honor timestamps accompanying each compressed H.264 frame.
                      Please see ScreenStreamProcessData() in ScreenUtilsStub.c for more details.

Accessories have to report its platform latency through the AirPlayReceiverServer copyProperty_f delegate. Alternatively, this can also be specified in the
airplay.conf configuration file.
  - All fixed latencies must be reported, including any signal processing.
  - Latencies are reported per stream, so any additional processing being added to a stream based off the stream type 
    or audio type should only add to the latency of that stream.
  - Output buffer size should be kept to a minimum. If double buffering is applied, it is often possible to keep the latency to the size of a single buffer.

Accessories have to report its platform audio format support through the AirPlayReceiverServer copyProperty_f delegate. Alternatively, this can also be
specified in the in the airplay.conf configuration file.
  - Supported audio formats must be provided for all required audio types for each stream type.

Accessories have to report its supported displays through the AirPlayReceiverServer copyProperty_f delegate.

Accessories have to report the supported HID devices through the AirPlayReceiverServer copyProperty_f delegate.

When the device gets disconnected:
	The network stack on a head unit should post a reliable notification as soon as possible after physical disconnect. 
	The Apple CarPlay Communication Plug-in uses routing socket to detect changes in the network stack by listening to 
	the following messages (if defined on a platform):
		- RTM_NEWLINK
		- RTM_DELLINK
		- RTM_NEWADDR
		- RTM_DELADDR
		- RTM_IFINFO
		- RTM_CHANGE.

	The Apple CarPlay Communication Plug-in will tear down the session if any of the following occurs:
		- The NCM network interface is torn down (can't get SIOCGIFEFLAGS)
		- The NCM network interface is not running (IFF_RUNNING flag is not present)
		- The NCM network interface status becomes inactive.

    The Apple CarPlay Communication Plug-in will tear down the session due to inactivity:
        - No data received for 9 seconds - if the platform supports TCP_KEEPCNT when an audio or video stream was active
        - No data received for 30 seconds - if the platform does not support TCP_KEEPCNT when an audio or video stream was active
        - No data received for 30 seconds - no audio or video stream was active

The network stack on the head unit must support the SIOCGIFMEDIA ioctl / IFM_IEEE80211 flag for CarPlay over wireless.
  - If the network stack does not support this, you can manually set the type based on the unique name of the CarPlay WiFi interface:

    AccessorySDK/Support/NetUtils.c:SocketGetInterfaceInfo() (line 3939)

    if( strcmp( inIfName, anInterface ) == 0 ) {
      *outTransportType = kNetTransportType_WiFi;
    }

Please refer to the MFi Accessory Interface Specification with CarPlay for more details on the reference architecture.

==========
Port Usage
==========

Server or incoming ports can be configured to be dynamic or fixed.  Fixed ports can be specified using the compile define -DAIRTUNES_DYNAMIC_PORTS=0
and each port is defined in AirPlayCommon.h.  CarPlayControl will use a dynamic outgoing port in which a response will be on the same port number.
There are additional loopback ports that are dynamic only which are used internally to signal exit conditions.

====================
CarPlayControlClient
====================

	The CarPlayControlClient interface must be used by receivers to connect and disconnect a CarPlay session to a particular device
	(wireless and/or wired).

	To use this interface, clients instantiate a "CarPlayControlClientCreate” object (it is a CoreFoundation object that follows normal CF 
	retain/release semantics) and provide it a callback. The callback will be invoked to inform the client when phones that support CarPlay 
	are discovered to be available, cease to be available, or some aspect about an already present phone changes (such as it’s name).

	Once instantiated, clients can control when it actively searches for phones by using the Start and Stop methods (typically, clients would 
	leave it running continuously by “Start”ing it immediately after creation.

	Finally, when the client decides which phone it would like to initiate a CarPlay session with, it calls the “Connect” method.  This method 
	is asynchronous and may return without error before a session is actually established. Because of this, and the fact that an unforeseen 
	error may occur during the connection process, it is strongly recommended that the client watch for an incoming session to occur and if 
	that does not happen within some threshold of time, it should try again or select another phone.

	If a client wants to explicitly close a CarPlay session, CarPlayControlClientDisconnect() can be used.

	You must follow this order when creating the CarPlayControlClient & AirPlayReceiverServerCreate
	AirPlayReceiverServerCreate()
	CarPlayControlClientCreateWithServer()
	AirPlayReceiverServerStart()
	CarPlayControlClientStart()

	For wireless CarPlay, the CarPlayControlClient interface will prevent false removals due to inactivity by default.  Therefore, removal events
	will not always be provided.  If a device is still available through the CarPlayControl interface where it's Bonjour record is still present,
	issuing CarPlayControlClientConnect() will attempt to connect via wake on wireless using cached device information when it was first discovered.
	If the device is not available, an error is returned.  If a Bonjour record is still active, it is recommended to try to establish a connection
	via CarPlayControlClientConnect().  This will reconfirm the record causing it to be removed asynchronously if the device is no longer present.

=================================
iAP2 SUPPORT FOR WIRELESS CARPLAY 
=================================

	To support continued iAP during Wireless CarPlay operation, additional AirPlayReceiverSession APIs have been added to tunnel iAP traffic over 
	the CarPlay protocol.  You must perform the full iAP handshaking over this protocol which includes the detect sequence and link synchronization.
	The Zero-Ack implementation is recommended for the link parameters.  See the AISpec for additional details.

	AirPlayReceiverSessionSendiAPMessage
		Use this function to send an iAP message to the iPhone.

	kAirPlayCommand_iAPSendMessage
		This new command (delivered via the session’s AirPlayReceiverSessionControl_f delegate method) is how an iAP message is delivered by 
		the phone to the receiver.

	AirPlayReceiverSessionStarted_f session delegate method
		This new delegate method is invoked once the session has successfully started. This state information is important because it is not 
		valid to send any commands or messages to the iPhone until after this method has been called.  Previously, there was no way to know 
		when it was valid to send commands to the phone and sending commands too early would cause undefined behavior.

		You must determine if the current CarPlay session is wireless by checking the kAirPlayProperty_TransportType property using
		AirPlayReceiverSessionCopyProperty.  Only if the current CarPlay session is wireless, you must start a new iAP2 session over the
		CarPlay control channel.  iAP2 over Bluetooth must not be disconnected until the disableBluetooth command is received in the
		AirPlayReceiverSessionControl_f delegate.

=======================================
AudioConverter API for Wireless CarPlay
=======================================

Wireless CarPlay requires accessory codec support to decode and encode the various streams of the following formats:
  - raw AAC-LC decode
  - Opus encode/decode or raw AAC-ELD encode/decode

  - The flow will be
      - Initialize the codec when a stream is set up -> AudioConverterNew().  The input and output formats are provided, along with additional
        information about the stream such as sample rate and number of channels.
      - Additional properties may be provided -> AudioConverterSetProperty()
      - Compressed data will be repeatedly provided to the AudioConverter APIs to be inputted into a codec and decoded data will be 
        returned -> AudioConverterFillComplexBuffer()
      - Decoded/uncompressed data will be provided to the AudioStream APIs
      - AudioConverterReset() should reset the codec and any remaining data that is held by the codec should be discarded
      - Once the stream is no longer required, the codec instance can be destroyed -> AudioConverterDispose()

Recommended OPUS encoder parameters
   - OPUS_APPLICATION_VOIP
   - OPUS_SET_BITRATE(OPUS_AUTO)
   - OPUS_SET_BANDWIDTH(OPUS_AUTO)
   - OPUS_SET_VBR(1)
   - OPUS_SET_VBR_CONSTRAINT(1)
   - OPUS_SET_COMPLEXITY(10)
   - OPUS_SET_INBAND_FEC(0)
   - OPUS_SET_FORCE_CHANNELS(OPUS_AUTO)
   - OPUS_SET_DTX(0)
   - OPUS_SET_PACKET_LOSS_PERC(0)
   - OPUS_SET_LSB_DEPTH(16)

=======================================
Keychain storage
=======================================

Pairing information is stored in a keychain file which is created once in ~/Library/KeyChains/default.keychain" (pair-setup).  Once the keychain is 
created, this data is used to establish a secure session (pair-verify).  To override the default location of the keychain file, use the compile 
time options: KEYCHAINS_PATH and KEYCHAIN_DEFAULT_FILENAME.

==================
CONFIGURATION FILE
==================

Configuration properties should be set through the AirPlayReceiverServer and AirPlayReceiverSession delegates.  The recommended method is through the delegates.
A limited set of properties can be set through a configuration file. The default path to this config file is "/etc/airplay.conf", which can be overridden at
build time by setting the compile flag AIRPLAY_CONFIG_FILE_PATH.

The following file formats are supported:
  - Binary plist: This is the preferred format, as it is the most efficient and full-featured. These files can be created 
                  and edited on a Mac using XCode. A number of third-party editors also exist. It can also be created and 
                  edited on the accessory itself using airplayutil: 
                    airplayutil prefs write -f /etc/airplay.conf <key> <value>
  - INI: This is a more easily editable text format. The INI file reader works with any line ending format, supports # and ; 
         comment lines, blank lines, and spaces before and after delimiters (e.g., "name=value" or "name = value"). 
         Properties specified before any section are treated as global properties (e.g. "model" for the model name of the 
         accessory).

The following properties are supported in the config file:
  - audioInputFormats:          Number, or'ed list of input audio formats that the hardware natively supports.  See AirPlayCommon.h
                                for a list of valid values (kAirPlayAudioFormat_*).
  - audioOutputFormats:         Number, or'ed list of output audio formats that the hardware natively supports.  See AirPlayCommon.h
                                for a list of valid values (kAirPlayAudioFormat_*).
  - clientOSBuildVersionMin:    Minimum iOS version which is supported by the accessory.
  - extendedFeatures:           Array of extended features keys. This key is only supported in the plist file formats.
  - firmwareRevision:           Firmware revision of the accessory.
  - hardwareRevision:
  - limitedUIElements           Dictionary (plist) or named section (INI) with property lines for each key/value pair
                                affecting by limited UI mode (softKeyboard, nonMusicLists, musicLists).
  - manufacturer:               Manufacturer of the accessory.
  - maxFPS:                     Max frames per second the display supports.
  - model:                      Model name of the accessory.
  - oemIconLabel:               Label text to accompany the OEM icon.
  - oemIconPath:                Path of a PNG file for the accessory’s icon to show in the user interface. If present,
                                the file at this path will be read and reported via the /info response (deprecated, must be used to support older iOS devices).
  - oemIconVisible:             Whether or not the OEM icon is visible on the home screen ("0" or "1").
  - OSInfo:                     Name, version and architecture (e.g. "Darwin 13.0.0 x86_64").
  - rightHandDrive              Whether or not the vehicle is right hand drive ("0" or "1").

Please refer to the MFi Accessory Interface Specification With CarPlay section on the "Info Message" for more 
details.


===========
AIRPLAYUTIL
===========

The "airplayutil" command line utility is provided as part of the reference implementation. It provides options to run basic
tests and to enable additional logging. Run "airplayutil help" for more detailed information.

===============
TOOLS & LOGGING 
===============

In parallel to CarPlay, for testing and debugging the platform must provide:
  - A way to run iPerf.
  - A way to set logging level using airplayutil.



