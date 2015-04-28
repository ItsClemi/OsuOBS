#include "stdafx.h"
#include "BeatmapName.h"

#include "ProcessInfo.h"
#include "Core.h"




CBeatmapName::CBeatmapName( XElement* pData ) : TextOutputSource( pData )
{
	CCore::GetInstance( )->RegBmChangedEvent( [ this ]( sBeatmapInfo* pInfo ){
		std::lock_guard< std::mutex > l( m_cs );

		if( pInfo == nullptr )
		{
			m_szName = L"Selecting beatmap!";
		}
		else
		{
			m_szName = pInfo->m_szFullName;
		}

		m_bIsDirty = true;
	} );
}

CBeatmapName::~CBeatmapName( )
{
}

void CBeatmapName::Tick( float fSeconds )
{
	TextOutputSource::Tick( fSeconds );

	//=> If the mutex is not free / try to set it with the next frame
	if( m_cs.try_lock( ) )
	{
		if( m_bIsDirty )
		{
			this->SetString( L"text", m_szName.c_str( ) );
			m_bIsDirty = false;
		}

		m_cs.unlock( );
	}

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ImageSource* STDCALL _SourceBeatmapName( XElement* pData )
{
	return new CBeatmapName( pData );
}

bool STDCALL _SourceConfigBeatmapName( XElement *element, bool bCreating )
{
	return ConfigureTextSource( element, bCreating );
}

