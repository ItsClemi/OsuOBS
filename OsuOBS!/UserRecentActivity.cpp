#include "stdafx.h"
#include "UserRecentActivity.h"


CUserRecentActivity::CUserRecentActivity( XElement* pData ) 
	: TextOutputSource( pData )
{

}

CUserRecentActivity::~CUserRecentActivity( )
{
}

void CUserRecentActivity::Tick( float fSeconds )
{
	TextOutputSource::Tick( fSeconds );

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
