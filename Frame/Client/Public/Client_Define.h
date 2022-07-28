#pragma once

namespace Client
{
	const unsigned int g_iWinCX = 1280;
	const unsigned int g_iWinCY = 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_LOBY, LEVEL_TUTORIAL, LEVEL_GAME1, LEVEL_GAME2, LEVEL_GAME3, LEVEL_TEST, LEVEL_END };
}

#include <process.h>

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

extern bool g_bdebug;

using namespace Client;