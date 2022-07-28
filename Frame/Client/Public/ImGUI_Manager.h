#pragma once
#include "Client_Define.h"
#include "Base.h"
#include "VIBuffer_Rect_Instance.h"

BEGIN(Client)

class CImGUI_Manager final : public CBase
{
	DECLARE_SINGLETON(CImGUI_Manager)
private:
	CImGUI_Manager();
	virtual ~CImGUI_Manager() = default;
public:
	HRESULT Init_ImGui(ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	HRESULT StartFrame(void);
	HRESULT Set_Contents(void);
	HRESULT Seting_Effect(void);
	HRESULT Render(void);
public:
	void InitWin32(HWND hWnd);
	LRESULT WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Shutdown(void);
private:
	bool m_bImguiEnable = true;
private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

	_bool Instance = false;
	_int  InstanceMod = 0;
	_int  KeyFrame_Num = 0;
	CVIBuffer_Rect_Instance*						Buffer_Rect_Instance = nullptr;
	CVIBuffer_Rect_Instance::INSTANCEDESC*			m_InstanceDesc = nullptr;
	CVIBuffer_Rect_Instance::INSTANCECHANGEDESC*	m_ChangeDesc = nullptr;
	CVIBuffer_Rect_Instance::INSTANCECREATEDESC*	m_CreateDesc = nullptr;

public:
	void OnOffImgui(void) { m_bImguiEnable = !m_bImguiEnable; }
	bool isImguiEnable() { return m_bImguiEnable; }
public:
	virtual void Free() override;
};

END