#ifndef PARSEMSG_HPP
#define PARSEMSG_HPP

#pragma once

void	BEGIN_READ( void *buf, int size );
int		READ_CHAR();
int		READ_BYTE();
int		READ_SHORT();
int		READ_WORD();
int		READ_LONG();
float	READ_FLOAT();
char	*READ_STRING();
float	READ_COORD();
float	READ_ANGLE();
float	READ_HIRESANGLE();

#endif // !PARSEMSG_HPP