#include "stdafx.h"
#include "UserRankSource.h"

#include "UserInfo.h"
#include "Core.h"


CUserRankSource::CUserRankSource( XElement* pData ) 
	: TextOutputSource( pData )
{
	this->SetString( L"text", L"[Performance]" );

	//=> TODO: IsDirty check
	m_nCallbackId = CCore::GetInstance( )->GetUserInfo( )->RegisterCallbackPerformance( [ this ]( std::shared_ptr< std::wstring > szPerformance ){ 
		std::lock_guard< std::mutex > l( m_cs );

		m_szPerformance = *szPerformance;
	} );
}

CUserRankSource::~CUserRankSource( )
{
	CCore::GetInstance( )->GetUserInfo( )->UnregisterCallbackPerformance( m_nCallbackId );
}

void CUserRankSource::Tick( float fSeconds )
{
	TextOutputSource::Tick( fSeconds );

	if( m_cs.try_lock( ) )
	{
		this->SetString( L"text", m_szPerformance.c_str( ) );
			
		m_cs.unlock( );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


ImageSource* STDCALL _SourceUserRank( XElement* data )
{
	return new CUserRankSource( data );
}

bool STDCALL _SourceConfigUserRank( XElement* pElement, bool bCreating )
{	
	return ConfigureTextSource( pElement, bCreating );
}