#pragma once

#include "BeatmapInfo.h"
#include "UserInfo.h"
#include "ProcessInfo.h"


class CCore
{
	CCore( );
	~CCore( );

public:
	static CCore* GetInstance( );

public:
	void StartOsuThread( );
	void StopOsuThread( );


public:	//=> Better coding style ! >> forward
	
	void RegBmChangedEvent( CProcessInfo::BmCallback f )	{ GetProcessInfo( )->RegBmChangedEvent( f ); }


public:
	CUserInfo*		GetUserInfo( )							{ return &m_userInfo; }
	CBeatmapInfo*	GetBmInfo( )							{ return &m_bmInfo; }
	CProcessInfo*	GetProcessInfo( )						{ return &m_processInfo; }



public:
	Shader*	m_pSolidVertexShader = nullptr;
	Shader*	m_pSolidPixelShader = nullptr;

private:

	CProcessInfo	m_processInfo;
	CUserInfo		m_userInfo;
	CBeatmapInfo	m_bmInfo;

};