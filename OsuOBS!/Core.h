#pragma once

#include "BeatmapInfo.h"
#include "UserInfo.h"
#include "ProcessInfo.h"
#include "BeatmapWeb.h"


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
	
	size_t	RegBmChangedEvent( CProcessInfo::BmCallback f )		{ return GetProcessInfo( )->RegBmChangedEvent( f ); }
	void	UnregBmChangedEvent( size_t nId )					{ GetProcessInfo( )->UnregBmChangedEvent( nId ); }


public:
	CUserInfo*			GetUserInfo( )							{ return &m_userInfo; }
	CBeatmapInfo*		GetBmInfo( )							{ return &m_bmInfo; }
	CProcessInfo*		GetProcessInfo( )						{ return &m_processInfo; }
	CBeatmapWeb*		GetBmInfoWeb( )							{ return &m_bmWeb; }


public:
	Shader*	m_pSolidVertexShader = nullptr;
	Shader*	m_pSolidPixelShader = nullptr;

private:

	CProcessInfo	m_processInfo;
	CUserInfo		m_userInfo;
	CBeatmapInfo	m_bmInfo;
	CBeatmapWeb		m_bmWeb;


};