#pragma once

enum enum_ESCRETURN{
	enum_ESCRETURN_exit = 0,
	enum_ESCRETURN_continue,
	enum_ESCRETURN_restart,
};

enum_ESCRETURN Call_Escape( HWND hWnd );
