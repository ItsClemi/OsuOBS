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
	size_t	m_nId;
	

};


//////////////////////////////////////////////////////////////////////////

ImageSource* STDCALL _SourceBeatmapInfo( XElement *data );
bool STDCALL _SourceConfigBeatmapInfo( XElement *element, bool bCreating );



