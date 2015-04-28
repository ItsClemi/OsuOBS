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
	GetUserInfo( )->StartUserThread( );
	GetBmInfoWeb( )->StartBeatmapWebThread( );
}

void CCore::StopOsuThread( )
{
	GetUserInfo( )->StopUserThread( );
	GetProcessInfo( )->StopProcessInfo( );
	GetBmInfoWeb( )->StopBeatmapWebThread( );
}


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