#include "stdafx.h"
#include "BeatmapInfoSource.h"
#include "Core.h"

using namespace std;

CBeatmapInfoSource::CBeatmapInfoSource( XElement* pData )
	: TextOutputSource( pData )
{
	this->SetString( L"text", L"[Beatmap info]" );

	m_nCallbackId = CCore::GetInstance( )->RegBmChangedEvent( [ this ]( sBeatmapInfo* pInfo )
	{
		lock_guard< mutex >	l( m_cs );

		m_szText = L"";

		if( pInfo != nullptr )
		{
			wchar_t szBuff[ 256 ];

			if( m_bShowDrain )
			{
				swprintf_s( szBuff, L"HP: %.1f ", pInfo->m_fDrain );
				m_szText += szBuff;
			}

			if( m_bShowCircleSize )
			{
				swprintf_s( szBuff, L"CS: %.1f ", pInfo->m_fCircleSize );
				m_szText += szBuff;
			}

			if( m_bShowOverallDifficulty )
			{
				swprintf_s( szBuff, L"OD: %.1f ", pInfo->m_fOverallDifficulty );
				m_szText += szBuff;
			}

			if( m_bShowApprochRate )
			{
				swprintf_s( szBuff, L"AR: %.1f ", pInfo->m_fApproachRate );
				m_szText += szBuff;
			}

			if( m_bShowSliderMul )
			{
				swprintf_s( szBuff, L"SMul: %.1f ", pInfo->m_fSliderMul );
				m_szText += szBuff;
			}
		}

		m_bIsDirty = true;
	} );
}


CBeatmapInfoSource::~CBeatmapInfoSource( )
{
	CCore::GetInstance( )->UnregBmChangedEvent( m_nCallbackId );
}

void CBeatmapInfoSource::Tick( float fSeconds )
{
	TextOutputSource::Tick( fSeconds );

	if( m_cs.try_lock( ) )
	{
		if( m_bIsDirty )
		{
			this->SetString( L"text", m_szText.c_str( ) );
		}

		m_cs.unlock( );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ImageSource* STDCALL _SourceBeatmapInfo( XElement* pData )
{
	return new CBeatmapInfoSource( pData );
}

bool STDCALL _SourceConfigBeatmapInfo( XElement *element, bool bCreating )
{
	return ConfigureTextSource( element, bCreating );
}

