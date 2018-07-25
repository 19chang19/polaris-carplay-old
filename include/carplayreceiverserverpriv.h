/*
 * carplayreceiverserverpriv.h
 *
 *  Created on: Jul 24, 2018
 *      
 */

#ifndef CARPLAYCLIENT_CARPLAYRECEIVERSERVERPRIV_H_
#define CARPLAYCLIENT_CARPLAYRECEIVERSERVERPRIV_H_
#include "CarPlayReceiverServer.h"
struct CarPlayReceiverServerPrivate
{
	CFRuntimeBase						base;			// CF type info. Must be first.
	void *								platformPtr;	// Pointer to the platform-specific data.
	dispatch_queue_t					queue;			// Internal queue used by the server.
	AirPlayReceiverServerDelegate		delegate;		// Hooks for delegating functionality to external code.

	// Bonjour
	Boolean						bonjourRestartPending;	// True if we're waiting until playback stops to restart Bonjour.
	uint64_t					bonjourStartTicks;		// Time when Bonjour was successfully started.
	dispatch_source_t			bonjourRetryTimer;		// Timer to retry Bonjour registrations if they fail.
	DNSServiceRef				bonjourAirPlay;			// _airplay._tcp Bonjour service.

	// Servers

	HTTPServerRef				httpServer;				// HTTP server
	dispatch_queue_t			httpQueue;				// Internal queue used by the http servers and connections.
#if( AIRPLAY_HTTP_SERVER_LEGACY )
	HTTPServerRef				httpServerLegacy;		// HTTP server to support legacy BTLE clients that use hardcoded port 5000.
#endif
	uint8_t						httpTimedNonceKey[ 16 ];

	Boolean						playing;				// True if we're currently playing.
	Boolean						serversStarted;			// True if the network servers have been started.
	Boolean						started;				// True if we've been started. Prefs may still disable network servers.
	uint8_t						deviceID[ 6 ];			// Globally unique device ID (i.e. primary MAC address).
	char						ifname[ IF_NAMESIZE + 1 ]; // Name of the interface to advertise on (all if empty).
	int							timeoutDataSecs;		// Timeout for data (defaults to kAirPlayDataTimeoutSecs).
	char *						configFilePath;			// Path to airplay.conf/airplay.ini
	CFDictionaryRef				config;
};
struct AirPlayReceiverSessionPrivate
{
	CFRuntimeBase					base;						// CF type info. Must be first.
	dispatch_queue_t				queue;						// Internal queue used by the session.
	AirPlayReceiverServerRef		server;						// Pointer to the server that owns this session.
	AirPlayReceiverConnectionRef	connection;					// Control connection for the session
	void *							platformPtr;				// Pointer to the platform-specific data.
	AirPlayReceiverSessionDelegate	delegate;					// Hooks for delegating functionality to external code.
	char							clientOSBuildVersion[ 32 ];
	pthread_mutex_t					mutex;
	pthread_mutex_t *				mutexPtr;
	dispatch_source_t				periodicTimer;				// Timer for periodic tasks.

	NetTransportType				transportType;				// Network transport type for the session.
	sockaddr_ip						peerAddr;					// Address of the sender.
	uint8_t							sessionUUID[ 16 ];			// Random UUID for this AirPlay session.
	OSStatus						startStatus;				// Status of starting the session (i.e. if session failed).
	uint64_t						clientDeviceID;				// Unique device ID of the client sending to us.
	uint8_t							clientIfMACAddr[ 6 ];		// Client's MAC address of the interface this session is connected on.
	uint64_t						clientSessionID;			// Unique session ID from the client.
	uint32_t						clientVersion;				// Source version of the client or 0 if unknown.
	uint64_t						sessionTicks;				// Ticks when this session was started.
	uint64_t						playTicks;					// Ticks when playback started.
	AirPlayCompressionType			compressionType;
	AES_CBCFrame_Context			decryptorStorage;			// Used for decrypting audio content.
	AES_CBCFrame_Context *			decryptor;					// Ptr to decryptor or NULL if content is not encrypted.
	uint8_t							aesSessionKey[ 16 ];
	uint8_t							aesSessionIV[ 16 ];
	AirTunesSource					source;
	Boolean							screen;						// True if AirPlay Screen. False if normal AirPlay Audio.
	uint32_t						framesPerPacket;
	int								audioQoS;					// QoS to use for audio control and data.

	// Control/Events

	Boolean							controlSetup;				// True if control is set up.
	Boolean							useEvents;					// True if the client supports events.
	Boolean							sessionStarted;				// True if the session has been started (e.g. RECORD received).
	dispatch_queue_t				eventQueue;					// Internal queue used by eventClient.
	HTTPClientRef					eventClient;				// Client for sending RTSP events back to the sender.
	int								eventPendingMessageCount;	// Number of outgoing event messages which haven't got corresponding replies.
	dispatch_source_t				eventReplyTimer;			// Timer for waiting event replies.
	SocketRef						eventSock;					// Socket for accepting an RTSP event connection from the sender.
	int								eventPort;					// Port we're listening on for an RTSP event connection.
	Boolean							sessionIdle;				// True if no stream is setup, ie. no audio and video.
	Boolean							sessionIdleValid;			// True if sessionIdle is accurate

	PairingSessionRef				pairVerifySession;			// PairingSession to derive encryption/decription keys for individual streams

	// Main/AltAudio

	AirPlayAudioStreamContext		mainAudioCtx;				// Context for main audio input and output.
	AirPlayAudioStreamContext		altAudioCtx;				// Context for alt audio output.

	// GeneralAudio

	int								rtpAudioPort;				// Port we're listening on for RTP audio packets.
	int								redundantAudio;				// If > 0, redundant audio packets are being sent.
	Boolean							rtpAudioDupsInitialized;	// True if the dup checker has been initialized.
	uint16_t						rtpAudioDupsLastSeq;		// Last valid sequence number we've checked.
	uint16_t						rtpAudioDupsArray[ kAirTunesDupWindowSize ]; // Sequence numbers for duplicate checking.

	SocketRef						rtcpSock;					// Socket for sending and receiving RTCP packets.
	int								rtcpPortLocal;				// Port we're listening on for RTCP packets.
	int								rtcpPortRemote;				// Port of the peer sending us RTCP packets.
	sockaddr_ip						rtcpRemoteAddr;				// Address of the peer to send RTCP packets to.
	socklen_t						rtcpRemoteLen;				// Length of the sockaddr for the RTCP peer.
	Boolean							rtcpConnected;				// True if the RTCP socket is connected.

	uint32_t						minLatency;					// Minimum samples of latency.
	uint32_t						maxLatency;					// Maximum samples of latency.

	// NTP Time Sync
	AirTunesClockRef				airTunesClock;				// NTP Synchronizer clock
	SocketRef						timingSock;
	int								timingPortLocal;			// Local port we listen for time sync response packets on.
	int								timingPortRemote;			// Remote port we send time sync requests to.
	sockaddr_ip						timingRemoteAddr;			// Address of the peer to send timing packets to.
	socklen_t						timingRemoteLen;			// Length of the sockaddr for the timing peer.
	Boolean							timingConnected;			// True if the timing socket is connected.
	SocketRef						timingCmdSock;
	pthread_t						timingThread;
	pthread_t *						timingThreadPtr;

	// Keep Alive
	SocketRef						keepAliveSock;
	int								keepAlivePortLocal;			// Local port we listen for keep alive packets on.
	SocketRef						keepAliveCmdSock;
	pthread_t						keepAliveThread;
	pthread_t *						keepAliveThreadPtr;

	// Buffering

	uint32_t						nodeCount;					// Number of buffer nodes.
	size_t							nodeBufferSize;				// Number of bytes of data each node can hold.
	uint8_t *						nodeBufferStorage;			// Backing store for all the node buffers.
	AirTunesBufferNode *			nodeHeaderStorage;			// Array backing store for all the node headers.
	AirTunesBufferNode				busyListSentinelStorage;	// Dummy head node to maintain a circular list with a sentinel.
	AirTunesBufferNode *			busyListSentinel;			// Ptr to dummy node...saves address-of calculations.
	AirTunesBufferNode *			freeList;					// List of free nodes. Standard head pointer, null tail list.
	uint32_t						busyNodeCount;				// Number of nodes currently in use.
	uint8_t *						decodeBuffer;
	size_t							decodeBufferSize;
	uint8_t *						readBuffer;
	size_t							readBufferSize;
	uint8_t *						skewAdjustBuffer;			// Temporary buffer for doing skew compensation.
	size_t							skewAdjustBufferSize;

	// Audio

	uint32_t						audioLatencyOffset;			// Timestamp offset to compensate for latency.
	Boolean							flushing;					// Flushing is in progress.
	uint32_t						flushTimeoutTS;				// Stay in flush mode until this timestamp is received.
	uint32_t						flushUntilTS;				// Flush packets with timestamps earlier than this.
	uint16_t						lastRTPSeq;					// Last RTP sequence number received.
	uint32_t						lastRTPTS;					// Last RTP timestamp received.
	Boolean							lastPlayedValid;			// true if lastPlayed info is valid.
	uint32_t						lastPlayedTS;				// Last played RTP timestamp (sender/packet timeline).
	uint16_t						lastPlayedSeq;				// Last played RTP sequence number.

	AudioConverterRef				audioConverter;				// Audio converter for decoding packets.
	const uint8_t *					encodedDataPtr;				// Ptr to the data to decode from converter callback.
	const uint8_t *					encodedDataEnd;				// End of the data to decode.
	AudioStreamPacketDescription	encodedPacketDesc;			// Used for passing packet info back to the converter.
	uint32_t						compressionPercentAvg;		// EWMA of the compression percent we're getting.
	Boolean							stutterCreditPending;		// True if the next normal glitch will be ignored.
	int								glitchTotal;				// Total number of glitches in the session.
	int								glitchLast;					// Number of glitches when we last checked.
	int								glitchyPeriods;				// Number of periods with glitches.
	int								glitchTotalPeriods;			// Number of periods (with or without glitches).
	uint64_t						glitchNextTicks;			// Next ticks to check the glitch counter.
	uint64_t						glitchIntervalTicks;		// Number of ticks between glitch counter checks.
	MirroredRingBuffer				inputRing;					// Ring buffer for processing audio input.
	MirroredRingBuffer *			inputRingRef;				// Ptr to the ring buffer

	// Screen

	Boolean							screenInitialized;			// true=Screen fully initialized;
	AirPlayReceiverSessionScreenRef	screenSession;
	SocketRef						screenSock;					// Socket for receiving screen data.
	pthread_t						screenThread;				// Thread for receiving and processing packets.
	pthread_t *						screenThreadPtr;			// Ptr to the packet thread. NULL if thread isn't running.

};

#endif /* CARPLAYCLIENT_CARPLAYRECEIVERSERVERPRIV_H_ */
