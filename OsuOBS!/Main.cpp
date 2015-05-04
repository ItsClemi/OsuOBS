#include "StdAfx.h"

#include "BeatmapInfoSource.h"
#include "BeatmapName.h"
#include "BeatmapInfoWeb.h"

#include "UserRankSource.h"
#include "UserRecentActivity.h"

#include "Core.h"
#include "UserInfo.h"
#include "Config.h"


OBSEXPORT CTSTR GetPluginName( )
{
	return L"OsuOBS!";
}

OBSEXPORT CTSTR GetPluginDescription( )
{
	return L"Add Osu! infos to OBS";
}

OBSEXPORT bool LoadPlugin( )
{
	Log( L"Info | OSUOBS by It'sClemi (C)2015 v0.1" );

	CConfig::GetInstance( )->LoadConfig( );
	srand( GetTickCount( ) );

	API->RegisterImageSourceClass( L"OSUOBS-BMPN", L"[OSUOBS]Beatmap name", ( OBSCREATEPROC )_SourceBeatmapName, ( OBSCONFIGPROC )_SourceConfigBeatmapName );
	API->RegisterImageSourceClass( L"OSUOBS-BMPI", L"[OSUOBS]Beatmap info", ( OBSCREATEPROC )_SourceBeatmapInfo, ( OBSCONFIGPROC )_SourceConfigBeatmapInfo );
	API->RegisterImageSourceClass( L"OSUOBS-BMSD", L"[OSUOBS]Beatmap star difficulty", ( OBSCREATEPROC )_SourceBeatmapWebInfo, ( OBSCONFIGPROC )_SourceConfigBeatmapWebInfo );

	API->RegisterImageSourceClass( L"OSUOBS-USRK", L"[OSUOBS]User rank", ( OBSCREATEPROC )_SourceUserRank, ( OBSCONFIGPROC )_SourceConfigUserRank );
	API->RegisterImageSourceClass( L"OSUOBS-USRA", L"[OSUOBS]User recent activity", ( OBSCREATEPROC )_SourceUserRecentInfo, ( OBSCONFIGPROC )_SourceConfigUserRecentInfo );



	return true;
}

OBSEXPORT void UnloadPlugin( )
{
	CConfig::GetInstance( )->SaveConfig( );
}

OBSEXPORT void OnStartStream( )
{
	CCore::GetInstance( )->StartOsuThread( );
}

OBSEXPORT void OnStopStream( )
{
	CCore::GetInstance( )->StopOsuThread( );
}

OBSEXPORT void ConfigPlugin( HWND /*hWndParent*/ )
{
	OBSDialogBox( g_hInstance, MAKEINTRESOURCE( IDD_CONFIG ), API->GetMainWindow( ), ConfigDlgProc );
}
