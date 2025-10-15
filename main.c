#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int print_usage( const char *prog )
{
	printf( "%s <file> [output | -/+]\n", prog );
	return 0;
}

int main( int argc, char **argv )
{
	if ( argc < 2 )
		return print_usage( argv[ 0 ] );

	FILE *ifh = fopen( argv[ 1 ], "r" );
	if ( !ifh )
	{
		printf( "could not open file: %s\n", strerror( errno ) );
		return errno;
	}

	char  line[ 1024 ];
	char *pEnd = line + sizeof( line );

	int32_t line_pos = 1;

	bool bIsCli = false;
	if (argv[2] && (*argv[2] == '-' || *argv[2] == '+') )
		bIsCli = true;

	FILE *ofh = !bIsCli ? fopen( !argv[2] ? "out.json" : argv[2], "w" ) : stdout;
	// out.json
	// {
	//     ...
	// }
	if ( !ofh )
	{
		printf( "could not open file: %s\n", strerror( errno ) );
		return errno;
	}
	if (!bIsCli)
		fprintf( ofh, "{\n" );
	while ( !feof( ifh ) && fgets( line, sizeof( line ), ifh ) )
	{
		++line_pos;
		// strip whitespace
		char *pContent = line;
		while ( *pContent && isspace( *pContent ) )
		{
			if ( pContent == pEnd )
			{
				printf( "invalid line %d: %s\n", line_pos, line );
				return 1;
			}
			++pContent;
		}
		// ignore // # \n
		if ( *pContent == '\n' || ( pContent[ 0 ] == '/' && pContent[ 1 ] == '/' ) || pContent[ 0 ] == '#' || !*pContent )
			continue;

		// parsing KEY:TYPE=VALUE
		char *pSeparate_KeyType	  = strchr( pContent, ':' );
		char *pSeparate_TypeValue = strchr( pContent, '=' );
		char *pEnd_TypeValue	  = strchr( pContent, '\n' );
		if ( pSeparate_KeyType == NULL || pSeparate_TypeValue == NULL )
		{
			printf( "invalid line %d: %s\n", line_pos, line );
			return 1;
		}

		*pSeparate_KeyType	 = '\0';
		*pSeparate_TypeValue = '\0';
		*pEnd_TypeValue		 = '\0';

		char *pKey	 = pContent;
		char *pType	 = pSeparate_KeyType + 1;
		char *pValue = ( pSeparate_TypeValue + 1 == pEnd_TypeValue ) ? NULL : pSeparate_TypeValue + 1;
		// if string doesn't include value KEY:TYPE=

		/*
		"FIRST_CACHE_VARIABLE": {
		  "type": "BOOL",
		  "value": "OFF"
		},
		 */

		const char* fmt = "\t\"%s\": { \"type\": \"%s\", \"value\": \"%s\"  },\n";
		if (bIsCli )
		{
			if ( *argv[ 2 ] == '+' )
				fmt = "-D%s:%s=\"%s\" ";
			else // -
				fmt = "-D%s:%s=\"%s\"\n";
		}

		fprintf( ofh, fmt, pKey, pType, !pValue ? "" : pValue );
	}
	if (!bIsCli)
	{
		fseek( ofh, -2, SEEK_CUR );
		fprintf( ofh, "\n}\n\n" );
	}

	return 0;
}
