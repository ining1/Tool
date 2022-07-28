#pragma once
#include "Base.h"

BEGIN(Engine)

class CInput_Device final : public CBase
{
	DECLARE_SINGLETON(CInput_Device)
public:
	enum MOUSEBUTTONSTATE { DIMB_LBUTTON, DIMB_RBUTTON, DIMB_WHEEL, DIMB_END };
	enum MOUSEMOVESTATE { DIMM_X, DIMM_Y, DIMM_WHEEL, DIMM_END };
private:
	CInput_Device();
	virtual ~CInput_Device() = default;
public:
	HRESULT InitDevice(HINSTANCE hInst, HWND hWnd);
	void SetUp_InputDeviceState();
public:
	_char Get_DIKeyState(_uchar byKeyID);
	_char Get_DIMButtonState(MOUSEBUTTONSTATE eDIMBState);
	_long Get_DIMMoveState(MOUSEMOVESTATE eDIMMState);
public:
	_bool Key_Pressing(_uchar byKeyID);
	_bool Key_Down(_uchar byKeyID);
	_bool Key_Up(_uchar byKeyID);
	_bool Button_Pressing(MOUSEBUTTONSTATE eDIMBState);
	_bool Button_Down(MOUSEBUTTONSTATE eDIMBState);
	_bool Button_Up(MOUSEBUTTONSTATE eDIMBState);
private:
	LPDIRECTINPUT8 m_pInputSDK = nullptr;
	LPDIRECTINPUTDEVICE8 m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8 m_pMouse = nullptr;
private:
	DIMOUSESTATE m_MouseState;
	_char m_KeyBoardState[256];	
	_bool m_bKeyState[256];
	_bool m_bButtonState[4];
public:
	virtual void Free() override;
};

END