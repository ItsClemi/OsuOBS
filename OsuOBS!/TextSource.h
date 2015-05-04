#pragma once

#include "resource.h"



#define ClampVal(val, minVal, maxVal) \
    if(val < minVal) val = minVal; \
	    else if(val > maxVal) val = maxVal;

#define MAX_TEX_SIZE_W 8192
#define MAX_TEX_SIZE_H 8192
#define MIN_TEX_SIZE_W 32
#define MIN_TEX_SIZE_H 32

inline DWORD GetAlphaVal( UINT opacityLevel )
{
	return ( ( opacityLevel * 255 / 100 ) & 0xFF ) << 24;
}


class TextOutputSource : public ImageSource
{
public:
	TextOutputSource( XElement *data );
	virtual ~TextOutputSource( );

public:
	void Preprocess( );

	virtual void Tick( float fSeconds );

	void Render( const Vect2 &pos, const Vect2 &size );

	Vect2 GetSize( ) const
	{
		return baseSize;
	}

	virtual void UpdateSettings( );

	void SetString( CTSTR lpName, CTSTR lpVal );

	virtual void SetInt( CTSTR lpName, int iValue );

	void SetFloat( CTSTR lpName, float fValue );

	inline void ResetExtentRect( ) { showExtentTime = 0.0f; }

private:
	void DrawOutlineText( Gdiplus::Graphics *graphics, Gdiplus::Font &font, const Gdiplus::GraphicsPath &path, const Gdiplus::StringFormat &format, const Gdiplus::Brush *brush );

	HFONT GetFont( );

	void UpdateCurrentText( );

	void SetStringFormat( Gdiplus::StringFormat &format );

	float ProcessScrollMode( Gdiplus::Graphics *graphics, Gdiplus::Font *font, Gdiplus::RectF &layoutBox, Gdiplus::StringFormat *format );

	void UpdateTexture( );

private:
	bool        bUpdateTexture;

	String      strCurrentText;
	Texture     *texture;
	float       scrollValue;
	float       showExtentTime;

	int         mode;
	String      strText;
	String      strFile;

	String      strFont;
	int         size;
	DWORD       color;
	UINT        opacity;
	UINT        globalOpacity;
	int         scrollSpeed;
	bool        bBold, bItalic, bUnderline, bVertical;

	UINT        backgroundOpacity;
	DWORD       backgroundColor;

	bool        bUseOutline;
	float       outlineSize;
	DWORD       outlineColor;
	UINT        outlineOpacity;

	bool        bUseExtents;
	UINT        extentWidth, extentHeight;

	bool        bWrap;
	bool        bScrollMode;
	int         align;

	Vect2       baseSize;
	SIZE        textureSize;
	bool        bUsePointFiltering;

	bool        bMonitoringFileChanges;
	OSFileChangeData *fileChangeMonitor;

	std::unique_ptr<SamplerState> sampler;

	bool        bDoUpdate;

	SamplerState *ss;

	XElement    *m_pData;


};

struct ConfigTextSourceInfo
{
	CTSTR lpName;
	XElement *data;
	float cx, cy;

	StringList fontNames;
	StringList fontFaces;
};

inline int CALLBACK FontEnumProcThingy( ENUMLOGFONTEX *logicalData, NEWTEXTMETRICEX *physicalData, DWORD fontType, ConfigTextSourceInfo *configInfo )
{
	if( fontType == TRUETYPE_FONTTYPE ) //HomeWorld - GDI+ doesn't like anything other than truetype
	{
		configInfo->fontNames << logicalData->elfFullName;
		configInfo->fontFaces << logicalData->elfLogFont.lfFaceName;
	}

	return 1;
}

inline void DoCancelStuff( HWND hwnd )
{
	ConfigTextSourceInfo *configInfo = ( ConfigTextSourceInfo* )GetWindowLongPtr( hwnd, DWLP_USER );
	ImageSource *source = API->GetSceneImageSource( configInfo->lpName );
	//XElement *data = configInfo->data;

	if( source )
		source->UpdateSettings( );
}

inline UINT FindFontFace( ConfigTextSourceInfo *configInfo, HWND hwndFontList, CTSTR lpFontFace )
{
	UINT id = configInfo->fontFaces.FindValueIndexI( lpFontFace );
	if( id == INVALID )
		return INVALID;
	else
	{
		for( UINT i = 0; i<configInfo->fontFaces.Num( ); i++ )
		{
			UINT targetID = ( UINT )SendMessage( hwndFontList, CB_GETITEMDATA, i, 0 );
			if( targetID == id )
				return i;
		}
	}

	return INVALID;
}

inline UINT FindFontName( ConfigTextSourceInfo *configInfo, HWND hwndFontList, CTSTR lpFontFace )
{
	return configInfo->fontNames.FindValueIndexI( lpFontFace );
}

inline CTSTR GetFontFace( ConfigTextSourceInfo *configInfo, HWND hwndFontList )
{
	UINT id = ( UINT )SendMessage( hwndFontList, CB_GETCURSEL, 0, 0 );
	if( id == CB_ERR )
		return NULL;

	UINT actualID = ( UINT )SendMessage( hwndFontList, CB_GETITEMDATA, id, 0 );
	return configInfo->fontFaces[ actualID ];
}


bool STDCALL ConfigureTextSource( XElement *element, bool bCreating, bool bUseParent = false );
