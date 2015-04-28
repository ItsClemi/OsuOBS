#pragma once


inline HANDLE GetProcessByName( _In_ TCHAR const *name, _Out_ DWORD &pid )
{
	pid = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( snapshot == INVALID_HANDLE_VALUE )
	{
		Log( L"ERROR | GetProcessByName | Couldn't create process snapshot!" );
		return nullptr;
	}

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof( PROCESSENTRY32 );
	if( !Process32First( snapshot, &entry ) )
	{
		Log( L"ERROR | GetProcessByName | Couldn't enumerate first process!" );
	}
	else
	{
		while( Process32Next( snapshot, &entry ) )
		{
			if( wcscmp( entry.szExeFile, name ) == 0 )
			{
				pid = entry.th32ProcessID;
				break;
			}
		}
	}
	CloseHandle( snapshot );

	if( pid != 0 )
	{
		Log( L"INFO  | GetProcessByName | Found process", name );
		HANDLE process = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid );
		if( process == nullptr )
		{
			Log( L"ERROR | GetProcessByName | Couldn't open process", name );
			return nullptr;
		}
		else
		{
			return process;
		}
	}

	return nullptr;
}

inline HWND GetWindowById( DWORD pId )
{
	HWND hwndCurrent = GetWindow( GetDesktopWindow( ), GW_CHILD );
	if( hwndCurrent == nullptr )
	{
		Log( L"ERROR | GetWindowById | Couldn't get desktop window handle!" );
		return nullptr;
	}

	DWORD pIdCurrent = 0;
	do
	{
		GetWindowThreadProcessId( hwndCurrent, &pIdCurrent );
		// checking for GetWindowTextLength to not equal 0 is to ensure that we found 
		// the actual game window, not the small icon that pops up just before the game starts
		if( pIdCurrent == pId && IsWindowVisible( hwndCurrent ) && GetWindowTextLength( hwndCurrent ) != 0 )
		{
			Log( L"INFO  | GetWindowById | Found window with pId", pId );
			return hwndCurrent;
		}
	}
	while( hwndCurrent = GetNextWindow( hwndCurrent, GW_HWNDNEXT ) );
	return nullptr;
}


