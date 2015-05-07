#pragma once
#include "TextSource.h"

class CUserRankSource : public TextOutputSource
{
public:
	CUserRankSource( XElement* pData );
	virtual ~CUserRankSource( );

public:
	virtual void Tick( float fSeconds );

private:
	std::mutex		m_cs;
	std::wstring	m_szPerformance;

	bool			m_bIsDirty = false;


	size_t			m_nCallbackId;
};

ImageSource* STDCALL _SourceUserRank( XElement *data );
bool STDCALL _SourceConfigUserRank( XElement *element, bool bCreating );

