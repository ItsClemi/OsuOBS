#pragma once
#include "TextSource.h"

class CUserRecentActivity : public TextOutputSource
{
public:
	CUserRecentActivity( XElement* pData );
	virtual ~CUserRecentActivity( );

public:
	virtual void Tick( float fSeconds );
	virtual void UpdateSettings( );

	virtual void SetInt( CTSTR szName, int nValue );

private:
	bool		m_bNewData = false;

	
	XElement*	m_pData;

	std::mutex	m_cs;
	size_t		m_nRowCount = 5;
	std::vector< std::wstring >		m_vecActivity;

};


struct ConfigActivitySourceInfo
{
	CTSTR		m_szName;
	XElement*	m_pElement;

	bool		m_bCreating;

};



ImageSource* STDCALL _SourceUserRecentInfo( XElement* pData );
bool STDCALL _SourceConfigUserRecentInfo( XElement* pElement, bool bCreating );



