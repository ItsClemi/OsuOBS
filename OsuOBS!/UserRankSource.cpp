#include "stdafx.h"
#include "UserRankSource.h"

#include "UserInfo.h"
#include "Core.h"

CUserRankSource::CUserRankSource( XElement* pData ) 
	: TextOutputSource( pData )
{
	this->SetString( L"text", L"Performance:" );
}

CUserRankSource::~CUserRankSource( )
{
}

void CUserRankSource::Tick( float fSeconds )
{
	TextOutputSource::Tick( fSeconds );

	CUserInfo* pInfo = CCore::GetInstance( )->GetUserInfo( );
	pInfo->m_cs.lock( );
	{
		if( pInfo->m_bNewData )
		{
			this->SetString( L"text", pInfo->m_szPerformance.c_str( ) );
		}
	}
	pInfo->m_cs.unlock( );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


ImageSource* STDCALL _SourceUserRank( XElement* data )
{
	return new CUserRankSource( data );
}

bool STDCALL _SourceConfigUserRank( XElement *element, bool bCreating )
{
	return ConfigureTextSource( element, bCreating );
}
