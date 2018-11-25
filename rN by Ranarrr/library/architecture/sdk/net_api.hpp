#ifndef NET_API_HPP
#define NET_API_HPP

#pragma once

#include "netadr.hpp"

struct net_adrlist_s {
	net_adrlist_s	*next;
	netadr_s		remote_address;
};

struct net_response_s {
	int			error;			// NET_SUCCESS or an error code
	int			context;		// Context ID
	int			type;
	netadr_s	remote_address; // Server that is responding to the request
	double		ping;			// Response RTT ping time

	// Key/Value pair string (separated by backlash \ characters)
	// WARNING:  You must copy this buffer in the callback function, because it is freed
	//  by the engine right after the call!!!!
	// ALSO:  For NETAPI_REQUEST_SERVERLIST requests, this will be a pointer to a linked list of net_adrlist_t's
	void		*response;
};

typedef void( *net_api_response_func_t ) ( net_response_s *response );

struct net_status_s {
	int			connected;			// Connected to remote server?  1 == yes, 0 otherwise
	netadr_s	local_address;		// Client's IP address
	netadr_s	remote_address;		// Address of remote server
	int			packet_loss;		// Packet Loss (as a percentage)
	double		latency;			// Latency, in seconds (multiply by 1000.0 to get milliseconds)
	double		connection_time;	// Connection time, in seconds
	double		rate;				// Rate setting (for incoming data)
};

struct net_api_s {
	void		( *InitNetworking )		();
	void		( *Status )				( net_status_s *status );
	void		( *SendRequest )		( int context, int request, int flags, double timeout, netadr_s *remote_address, net_api_response_func_t response );
	void		( *CancelRequest )		( int context );
	void		( *CancelAllRequests )	();
	char		*( *AdrToString )		( netadr_s *a );
	int			( *CompareAdr )			( netadr_s *a, netadr_s *b );
	int			( *StringToAdr )		( char *s, netadr_s *a );
	const char *( *ValueForKey )		( const char *s, const char *key );
	void		( *RemoveKey )			( char *s, const char *key );
	void		( *SetValueForKey )		( char *s, const char *key, const char *value, int maxsize );
};

extern net_api_s netapi;

#endif // !NET_API_HPP