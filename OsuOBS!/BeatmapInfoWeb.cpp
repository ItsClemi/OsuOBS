#include "stdafx.h"
#include "BeatmapInfoWeb.h"

#include "BeatmapWeb.h"
#include "Core.h"



CBeatmapInfoWeb::CBeatmapInfoWeb( XElement* pData ) 
	: TextOutputSource( pData )
{
	CCore::GetInstance( )->GetBmInfoWeb( )->RegWebInfoCallback( [ this ]( sBeatmapQuery* pQuery ){
		std::lock_guard< std::mutex > l( m_cs );
		{
			if( pQuery == nullptr )
			{
				m_fDifficulty = -1.0f;
			}
			else
			{
				m_fDifficulty = pQuery->m_fResult;
			}

			m_bIsDirty = true;
		}
	} );
}


CBeatmapInfoWeb::~CBeatmapInfoWeb( )
{
}

void CBeatmapInfoWeb::Tick( float fSeconds )
{
	TextOutputSource::Tick( fSeconds );

	if( m_cs.try_lock( ) )
	{
		if( m_bIsDirty )
		{
			if( m_fDifficulty == -1 )
			{
				this->SetString( L"text", L"" );
			}
			else
			{
				wchar_t szBuff[ 32 ];
				swprintf_s( szBuff, L"%.1f", m_fDifficulty );

				this->SetString( L"text", szBuff );
			}

			m_bIsDirty = false;
		}

		m_cs.unlock( );
	}
}

ImageSource* STDCALL _SourceBeatmapWebInfo( XElement* pData )
{
	return new CBeatmapInfoWeb( pData );
}

bool STDCALL _SourceConfigBeatmapWebInfo( XElement* pElement, bool bCreating )
{
	return ConfigureTextSource( pElement, bCreating );
}
