#pragma once
#include "TextSource.h"

class CBeatmapName : public TextOutputSource
{
public:
	CBeatmapName( XElement* pData );
	virtual ~CBeatmapName( );

public:
	virtual void Tick( float fSeconds );

private:
	std::mutex		m_cs;
	std::wstring	m_szName;
	bool			m_bIsDirty = false;
};

//////////////////////////////////////////////////////////////////////////

ImageSource* STDCALL _SourceBeatmapName( XElement *data );
bool STDCALL _SourceConfigBeatmapName( XElement *element, bool bCreating );

