#pragma once

template< class T >
inline void RemoveCallback( T& t, size_t nId )
{
	for( size_t i = 0; i < t.size( ); i++ )
	{
		const auto& it = t[ i ];

		if( it.first == nId )
		{
			t.erase( t.begin( ) + i );
		}
	}
}