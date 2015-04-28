#pragma once
#include "TextSource.h"


class CBeatmapInfoSource : public TextOutputSource
{
public:
	CBeatmapInfoSource( XElement* pData );
	virtual ~CBeatmapInfoSource( );

public:
	virtual void Tick( float fSeconds );

private:
	std::mutex		m_cs;
	bool			m_bIsDirty;

	std::wstring	m_szText;

};


//////////////////////////////////////////////////////////////////////////

ImageSource* STDCALL _SourceBeatmapInfo( XElement *data );
bool STDCALL _SourceConfigBeatmapInfo( XElement *element, bool bCreating );



