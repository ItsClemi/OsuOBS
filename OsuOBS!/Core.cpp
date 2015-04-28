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

size_t CCore::RegisterBeatmapChangedINT( )
{
	m_beatMap.m_vecIsDirty.push_back( false );
	return m_beatMap.m_vecIsDirty.size( ) - 1;
}

void CCore::StopOsuThread( )
{
	GetUserInfo( )->StopUserThread( );
	GetProcessInfo( )->StopProcessInfo( );
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