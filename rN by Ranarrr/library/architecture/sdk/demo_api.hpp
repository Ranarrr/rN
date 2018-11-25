#ifndef DEMO_API_HPP
#define DEMO_API_HPP

#pragma once

struct demo_api_s {
	int		( *IsRecording )	();
	int		( *IsPlayingback )	();
	int		( *IsTimeDemo )		();
	void	( *WriteBuffer )	( int size, unsigned char *buffer );
};

extern demo_api_s demoapi;

#endif // !DEMO_API_HPP