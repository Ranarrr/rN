#ifndef NETADR_HPP
#define NETADR_HPP

#pragma once

enum netadrtype_t {
	NA_UNUSED,
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
	NA_IPX,
	NA_BROADCAST_IPX,
};

struct netadr_s {
	netadrtype_t	type;
	unsigned char	ip[ 4 ];
	unsigned char	ipx[ 10 ];
	unsigned short	port;
};

#endif // !NETADR_HPP