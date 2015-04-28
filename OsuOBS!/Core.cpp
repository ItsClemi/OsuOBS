#include "stdafx.h"
#include "Core.h"

CCore::CCore( )
{
}

CCore::~CCore( )
{
}

CCore* CCore::GetInstance( )
{
	static CCore s_core;
	return &s_core;
}



void CCore::StartOsuThread( )
{
	if( m_pSolidVertexShader == nullptr )
	{
		m_pSolidVertexShader = CreateVertexShaderFromFile( L"shaders/DrawSolid.vShader" );
	}

	if( m_pSolidPixelShader == nullptr )
	{
		m_pSolidPixelShader = CreatePixelShaderFromFile( L"shaders/DrawSolid.pShader" );
	}

	GetProcessInfo( )->StartProcessInfo( );
	GetBmInfo( )->StartBeatmapWebThread();
	GetUserInfo( )->StartUserThread( );
}

void CCore::StopOsuThread( )
{
	GetUserInfo( )->StopUserThread( );
	GetBmInfo( )->StopBeatmapWebThread( );
	GetProcessInfo( )->StopProcessInfo( );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//=> Threadfunc


/*
	JSON Cache

	{
	"Xi - Happy End of the World" : {
	"Author" : "xyz",
	"diff" :{
	"Happy End":{
	"AR: 4
	},
	}
	},
	}


	*/