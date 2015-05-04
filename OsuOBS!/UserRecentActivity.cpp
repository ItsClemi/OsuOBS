#include "stdafx.h"
#include "UserRecentActivity.h"

#include "UserInfo.h"
#include "Core.h"

using namespace std;

CUserRecentActivity::CUserRecentActivity( XElement* pData ) 
	: TextOutputSource( pData )
{
	this->SetString( L"text", L"[RecentActivty]\n[RecentActivty]\n[RecentActivty]\n[RecentActivty]\n" );

	CCore::GetInstance( )->GetUserInfo( )->RegisterCallbackActivity( [ this ]( shared_ptr< vector< wstring > > vec ){
		
		lock_guard< mutex > l( m_cs );

		m_vecActivity.clear( );

		for( size_t i = 0; i < vec->size( ); i++ )
		{
			m_vecActivity.push_back( vec->at( i ) );
		}

		m_bNewData = true;
	} );
}

CUserRecentActivity::~CUserRecentActivity( )
{
}

void CUserRecentActivity::Tick( float fSeconds )
{
	TextOutputSource::Tick( fSeconds );

	if( m_cs.try_lock( ) )
	{
		if( m_bNewData )
		{
			wstring szOut;
			for( auto i : m_vecActivity )
				szOut += i += L"\n";
		
			this->SetString( L"text", szOut.c_str( ) );

			m_bNewData = false;
		}

		m_cs.unlock( );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ImageSource* STDCALL _SourceUserRecentInfo( XElement* pData )
{
	return new CUserRecentActivity( pData );
}

bool STDCALL _SourceConfigUserRecentInfo( XElement* pElement, bool bCreating )
{
	return ConfigureTextSource( pElement, bCreating );
}
