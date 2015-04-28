#include "stdafx.h"
#include "BeatmapInfoSource.h"
#include "Core.h"

using namespace std;

CBeatmapInfoSource::CBeatmapInfoSource( XElement* pData )
	: TextOutputSource( pData )
{
	this->SetString( L"text", L"[Beatmap info]" );

	CCore::GetInstance( )->RegBmChangedEvent( [ this ]( sBeatmapInfo* pInfo )
	{ 
		lock_guard< mutex >	l( m_cs );

		if( pInfo == nullptr )
		{
			m_szText = L"";
		}
		else
		{

		}

	} );

}


CBeatmapInfoSource::~CBeatmapInfoSource( )
{
}

void CBeatmapInfoSource::Tick( float fSeconds )
{
	TextOutputSource::Tick( fSeconds );
// 
// 	CCore* pCore = CCore::GetInstance( );
// 	pCore->m_beatMap.m_cs.lock( );
// 	{
// 		if( pCore->m_beatMap.m_vecIsDirty[ m_nId ] )
// 		{
// 			sBeatmapInfo* pInfo = pCore->m_beatMap.pInfo;
// 			if( pInfo == nullptr )
// 			{
// 				this->SetString( L"text", L"" );
// 			}
// 			else
// 			{
// 				wchar_t szBuff[ 256 ];
// 
// 
// 				std::wstring szInfo;
// 				{
// 					swprintf_s( szBuff, L"HP: %.1f ", pInfo->m_fDrain );
// 					szInfo += szBuff;
// 				}
// 				{
// 					swprintf_s( szBuff, L"CS: %.1f ", pInfo->m_fCircleSize );
// 					szInfo += szBuff;
// 				}
// 				{
// 					swprintf_s( szBuff, L"OD: %.1f ", pInfo->m_fOverallDifficulty );
// 					szInfo += szBuff;
// 				}
// 				{
// 					swprintf_s( szBuff, L"AR: %.1f ", pInfo->m_fApproachRate );
// 					szInfo += szBuff;
// 				}
// 				{
// 					swprintf_s( szBuff, L"SMul: %.1f ", pInfo->m_fSliderMul );
// 					szInfo += szBuff;
// 				}
// 			
// 
// 				this->SetString( L"text", szInfo.c_str( ) );
// 			}
// 
// 			pCore->m_beatMap.m_vecIsDirty[ m_nId ] = false;
// 		}
// 	}
// 	pCore->m_beatMap.m_cs.unlock( );
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

