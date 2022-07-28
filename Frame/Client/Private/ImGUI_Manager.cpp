#include "stdafx.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "..\Public\ImGUI_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CImGUI_Manager)

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

CImGUI_Manager::CImGUI_Manager()
{
}

HRESULT CImGUI_Manager::Init_ImGui(ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut)
{
	ImGui_ImplDX11_Init(*ppDeviceOut, *ppDeviceContextOut);
	m_pDevice = *ppDeviceOut;
	m_pDeviceContext = *ppDeviceContextOut;
	
	

	

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	return S_OK;
}

HRESULT CImGUI_Manager::StartFrame(void)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	return S_OK;
}

HRESULT CImGUI_Manager::Set_Contents(void)
{
	if (ImGui::Begin("Tool"))
	{
		Seting_Effect();



	}
	ImGui::End();

	return S_OK;
}

HRESULT CImGUI_Manager::Seting_Effect(void)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (!Instance)
	{
		if (ImGui::Button("Ready"))
		{
			if (Buffer_Rect_Instance == nullptr)
			{
				Buffer_Rect_Instance = (CVIBuffer_Rect_Instance*)pGameInstance->Get_Component(LEVEL_TEST, L"Layer_Test", L"Com_VIBuffer");
				if (Buffer_Rect_Instance != nullptr)
				{
					m_InstanceDesc = Buffer_Rect_Instance->Get_InstanceDesc();
					m_ChangeDesc = Buffer_Rect_Instance->Get_ChangeDesc();
					m_CreateDesc = Buffer_Rect_Instance->Get_CreateDesc();
					Instance = true;
				}
			}
		}
	}
	
	
	if (Instance)
	{
		_int m_i[1] = { m_CreateDesc->iKeyFrame };
		_float m_f[1] = { m_CreateDesc->fMaxTime };


		if (ImGui::Button("CreateDesc"))
		{
			InstanceMod = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("ChangeDesc"))
		{
			InstanceMod = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset"))
		{
			Buffer_Rect_Instance->Reset();
			m_ChangeDesc = Buffer_Rect_Instance->Get_ChangeDesc();
		}
		ImGui::SameLine();
		if (ImGui::Button("Restart"))
		{
			Buffer_Rect_Instance->Restart();
		}


		if (InstanceMod == 0)
		{
			if (ImGui::SliderFloat("MaxTime", m_f, 0.f, 30.f))
			{
				m_CreateDesc->fMaxTime = m_f[0];
			}
			
			if (ImGui::SliderInt("KeyFrame", m_i, 0.f, 10.f))
			{
				m_CreateDesc->iKeyFrame = m_i[0];
			}

			m_i[0] = m_CreateDesc->iTexture;
			if (ImGui::SliderInt("Texture", m_i, 0, 28))
			{
				m_CreateDesc->iTexture = m_i[0];
			}
			
			m_i[0] = m_CreateDesc->iShader;
			if (ImGui::SliderInt("Shader", m_i, 0, 3))
			{
				m_CreateDesc->iShader = m_i[0];
			}

			m_f[0] = m_CreateDesc->fLastTime;
			if (ImGui::SliderFloat("LastTime", m_f, -10.f, 10.f))
			{
				m_CreateDesc->fLastTime = m_f[0];
			}

			m_f[0] = m_CreateDesc->fScale;
			if (ImGui::SliderFloat("Scale", m_f, 0.f, 2.f))
			{
				m_CreateDesc->fScale = m_f[0];
			}
			
			m_f[0] = m_CreateDesc->fSpeed;
			if (ImGui::SliderFloat("Speed", m_f, -10.f, 10.f))
			{
				m_CreateDesc->fSpeed = m_f[0];
			}
			m_f[0] = m_CreateDesc->fSpeedRand;
			if (ImGui::SliderFloat("SpeedRand", m_f, -10.f, 10.f))
			{
				m_CreateDesc->fSpeedRand = m_f[0];
			}
			m_f[0] = m_CreateDesc->fScale_Speed;
			if (ImGui::SliderFloat("Scale_Speed", m_f, 0.f, 30.f))
			{
				m_CreateDesc->fScale_Speed = m_f[0];
			}

			_float m_v[3] = { m_CreateDesc->vMovement.x,m_CreateDesc->vMovement.y,m_CreateDesc->vMovement.z };
			if (ImGui::SliderFloat3("Movement", m_v, -1.f, 1.f))
			{
				m_CreateDesc->vMovement.x = m_v[0];
				m_CreateDesc->vMovement.y = m_v[1];
				m_CreateDesc->vMovement.z = m_v[2];
			}

			m_v[0] = m_CreateDesc->vMovementRand.x;
			m_v[1] = m_CreateDesc->vMovementRand.y;
			m_v[2] = m_CreateDesc->vMovementRand.z;
			if (ImGui::SliderFloat3("MovementRand", m_v, -1.f, 1.f))
			{
				m_CreateDesc->vMovementRand.x = m_v[0];
				m_CreateDesc->vMovementRand.y = m_v[1];
				m_CreateDesc->vMovementRand.z = m_v[2];
			}

			m_v[0] = m_CreateDesc->vPostion.x;
			m_v[1] = m_CreateDesc->vPostion.y;
			m_v[2] = m_CreateDesc->vPostion.z;
			if (ImGui::SliderFloat3("Postion", m_v, -10.f, 10.f))
			{
				m_CreateDesc->vPostion.x = m_v[0];
				m_CreateDesc->vPostion.y = m_v[1];
				m_CreateDesc->vPostion.z = m_v[2];
			}

			m_v[0] = m_CreateDesc->vPostionRand.x;
			m_v[1] = m_CreateDesc->vPostionRand.y;
			m_v[2] = m_CreateDesc->vPostionRand.z;
			if (ImGui::SliderFloat3("PostionRand", m_v, -10.f, 10.f))
			{
				m_CreateDesc->vPostionRand.x = m_v[0];
				m_CreateDesc->vPostionRand.y = m_v[1];
				m_CreateDesc->vPostionRand.z = m_v[2];
			}
			_float m_v1[4];
			m_v1[0] = m_CreateDesc->vColor1.x;
			m_v1[1] = m_CreateDesc->vColor1.y;
			m_v1[2] = m_CreateDesc->vColor1.z;
			m_v1[3] = m_CreateDesc->vColor1.w;
			if (ImGui::SliderFloat4("Color1", m_v1, 0.f, 1.f))
			{
				m_CreateDesc->vColor1.x = m_v1[0];
				m_CreateDesc->vColor1.y = m_v1[1];
				m_CreateDesc->vColor1.z = m_v1[2];
				m_CreateDesc->vColor1.w = m_v1[3];
			}

			_float m_v2[4];
			m_v2[0] = m_CreateDesc->vColor2.x;
			m_v2[1] = m_CreateDesc->vColor2.y;
			m_v2[2] = m_CreateDesc->vColor2.z;
			m_v2[3] = m_CreateDesc->vColor2.w;
			if (ImGui::SliderFloat4("Color2", m_v2, 0.f, 1.f))
			{
				m_CreateDesc->vColor2.x = m_v2[0];
				m_CreateDesc->vColor2.y = m_v2[1];
				m_CreateDesc->vColor2.z = m_v2[2];
				m_CreateDesc->vColor2.w = m_v2[3];
			}


		}
		else if (InstanceMod == 1)
		{
			m_i[0] = KeyFrame_Num;
			if (ImGui::SliderInt("KeyFrame_Num", m_i, 0, Buffer_Rect_Instance->Get_KeyFrameNum() - 1))
			{
				KeyFrame_Num = m_i[0];
			}
			if (KeyFrame_Num == Buffer_Rect_Instance->Get_KeyFrameNum() - 1)
			{
				m_f[0] = m_ChangeDesc[KeyFrame_Num].fTime;
				if (ImGui::SliderFloat("Time", m_f, m_ChangeDesc[KeyFrame_Num - 1].fTime, Buffer_Rect_Instance->Get_MaxTime()))
				{
					m_ChangeDesc[KeyFrame_Num].fTime = m_f[0];
				}
			}
			else if(KeyFrame_Num ==0)
			{
				m_f[0] = m_ChangeDesc[KeyFrame_Num].fTime;
				if (ImGui::SliderFloat("Time", m_f, 0.f, Buffer_Rect_Instance->Get_MaxTime()))
				{
					m_ChangeDesc[KeyFrame_Num].fTime = m_f[0];
				}
			}
			else
			{
				m_f[0] = m_ChangeDesc[KeyFrame_Num].fTime;
				if (ImGui::SliderFloat("Time", m_f, m_ChangeDesc[KeyFrame_Num - 1].fTime, m_ChangeDesc[KeyFrame_Num + 1].fTime))
				{
					m_ChangeDesc[KeyFrame_Num].fTime = m_f[0];
				}
			}

			m_f[0] = m_ChangeDesc[KeyFrame_Num].fRelaxSpeed;
			if (ImGui::SliderFloat("RelaxSpeed", m_f, -1.f, 1.f))
			{
				m_ChangeDesc[KeyFrame_Num].fRelaxSpeed = m_f[0];
			}
			m_f[0] = m_ChangeDesc[KeyFrame_Num].fSpeed;
			if (ImGui::SliderFloat("Speed", m_f, -10.f, 10.f))
			{
				m_ChangeDesc[KeyFrame_Num].fSpeed = m_f[0];
			}
		}
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CImGUI_Manager::Render(void)
{
	if (m_bImguiEnable) 
	{
		StartFrame();
		Set_Contents();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	return S_OK;
}

void CImGUI_Manager::InitWin32(HWND hWnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
}

LRESULT CImGUI_Manager::WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
}

void CImGUI_Manager::Shutdown(void)
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}

void CImGUI_Manager::Free()
{
	Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}