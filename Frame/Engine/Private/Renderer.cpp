#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"

CRenderer::CRenderer(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	:CComponent(pDeviceOut, pDeviceContextOut)
	,m_pTarget_Manager(CTarget_Manager::GetInstance())
	,m_pLight_Manager(CLight_Manager::GetInstance())
{
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pTarget_Manager);
}

HRESULT CRenderer::NativeConstruct_Prototype()
{
	if (nullptr == m_pTarget_Manager)
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : nullptr == m_pTarget_Manager");
		return E_FAIL;
	}
	_uint iNumViewports = 1;

	D3D11_VIEWPORT Viewport;
	m_pDeviceContext->RSGetViewports(&iNumViewports, &Viewport);

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(L"Target_Diffuse", m_pDevice, m_pDeviceContext, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f))))
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : m_pTarget_Manager->Add_RenderTarget(Target_Diffuse)");
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Normal"), m_pDevice, m_pDeviceContext, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : m_pTarget_Manager->Add_RenderTarget(Target_Normal)");
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Depth"), m_pDevice, m_pDeviceContext, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : m_pTarget_Manager->Add_RenderTarget(Target_Depth)");
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Shade"), m_pDevice, m_pDeviceContext, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : m_pTarget_Manager->Add_RenderTarget(Target_Shade)");
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Specular"), m_pDevice, m_pDeviceContext, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : m_pTarget_Manager->Add_RenderTarget(Target_Specular)");
		return E_FAIL;
	}

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : m_pTarget_Manager->Add_MRT(Target_Diffuse)");
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : m_pTarget_Manager->Add_MRT(Target_Normal)");
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth"))))
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : m_pTarget_Manager->Add_MRT(Target_Depth)");
		return E_FAIL;
	}

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : m_pTarget_Manager->Add_MRT(Target_Shade)");
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : m_pTarget_Manager->Add_MRT(Target_Specular)");
		return E_FAIL;
	}

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext);
	if (nullptr == m_pVIBuffer)
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : nullptr == m_pVIBuffer");
		return E_FAIL;
	}

	m_pShader = CShader::Create(m_pDevice, m_pDeviceContext, L"../ShaderFiles/Shader_Deferred.hlsl", VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : nullptr == m_pShader");
		return E_FAIL;
	}
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixIdentity());	

	m_WorldMatrix._11 = Viewport.Width;
	m_WorldMatrix._22 = Viewport.Height;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f)));

#ifdef _DEBUG
	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(m_pDevice, m_pDeviceContext)))
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : m_pTarget_Manager->Ready_DebugBuffer");
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Ready_DebugDesc(TEXT("Target_Diffuse"), 0.f, 0.f, 200.f, 200.f)))
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : m_pTarget_Manager->Ready_DebugDesc(Target_Diffuse)");
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Ready_DebugDesc(TEXT("Target_Normal"), 0.f, 200.f, 200.f, 200.f)))
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : m_pTarget_Manager->Ready_DebugDesc(Target_Normal)");
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Ready_DebugDesc(TEXT("Target_Depth"), 0.f, 400.f, 200.f, 200.f)))
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : m_pTarget_Manager->Ready_DebugDesc(Target_Depth)");
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Ready_DebugDesc(TEXT("Target_Shade"), 200.f, 0.f, 200.f, 200.f)))
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : m_pTarget_Manager->Ready_DebugDesc(Target_Shade)");
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Ready_DebugDesc(TEXT("Target_Specular"), 200.f, 200.f, 200.f, 200.f)))
	{
		MSG_BOX(L"Failed To CRenderer : NativeConstruct_Prototype : m_pTarget_Manager->Ready_DebugDesc(Target_Specular)");
		return E_FAIL;
	}
#endif // _DEBUG

	return S_OK;
}

HRESULT CRenderer::NativeConstruct(void* pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eGroup, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
	{
		MSG_BOX(L"Failed To CRenderer : Add_RenderGroup : nullptr == pGameObject");
		return E_FAIL;
	}
	m_RenderList[eGroup].push_back(pGameObject);
	Safe_AddRef(pGameObject);
	return S_OK;
}

HRESULT CRenderer::Render_GameObjects()
{
	if (FAILED(Render_Priority()))
	{
		MSG_BOX(L"Failed To CRenderer : Render_GameObjects : Render_Priority");
		return E_FAIL;
	}
	if (FAILED(Render_NonBlend()))
	{
		MSG_BOX(L"Failed To CRenderer : Render_GameObjects : Render_NonBlend");
		return E_FAIL;
	}
	if (FAILED(Render_LightAcc()))
	{
		MSG_BOX(L"Failed To CRenderer : Render_GameObjects : Render_LightAcc");
		return E_FAIL;
	}
	if (FAILED(Render_Deferred()))
	{
		MSG_BOX(L"Failed To CRenderer : Render_GameObjects : Render_Deferred");
		return E_FAIL;
	}
	if (FAILED(Render_NonLight()))
	{
		MSG_BOX(L"Failed To CRenderer : Render_GameObjects : Render_NonLight");
		return E_FAIL;
	}
	if (FAILED(Render_Blend()))
	{
		MSG_BOX(L"Failed To CRenderer : Render_GameObjects : Render_Blend");
		return E_FAIL;
	}
	if (FAILED(Render_UI()))
	{
		MSG_BOX(L"Failed To CRenderer : Render_GameObjects : Render_UI");
		return E_FAIL;
	}
#ifdef _DEBUG
	if (FAILED(Render_DebugComponents()))
	{
		MSG_BOX(L"Failed To CRenderer : Render_GameObjects : Render_DebugComponents");
		return E_FAIL;
	}
	m_pTarget_Manager->Render_Debug(TEXT("MRT_Deferred"));
	m_pTarget_Manager->Render_Debug(TEXT("MRT_LightAcc"));
#endif // _DEBUG

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Add_DebugComponent(CComponent* pComponent)
{
	m_DebugComponents.push_back(pComponent);
	Safe_AddRef(pComponent);
	return S_OK;
}
#endif // _DEBUG

HRESULT CRenderer::Render_Priority()
{	
	HRESULT hr = 0;
	for (auto& pGameObject : m_RenderList[GROUP_PRIORITY])
	{
		if (nullptr != pGameObject)
		{
			hr = pGameObject->Render();
			Safe_Release(pGameObject);
		}
		if (FAILED(hr))
		{
			MSG_BOX(L"Failed To CRenderer : Render_Priority : 0 == hr");
			return E_FAIL;
		}
	}
	m_RenderList[GROUP_PRIORITY].clear();
	return S_OK;
}

HRESULT CRenderer::Render_Terrain()
{
	HRESULT hr = 0;
	for (auto& pGameObject : m_RenderList[GROUP_TERRAIN])
	{
		if (nullptr != pGameObject)
		{
			hr = pGameObject->Render();
			Safe_Release(pGameObject);
		}
		if (FAILED(hr))
		{
			MSG_BOX(L"Failed To CRenderer : Render_Terrain : 0 == hr");
			return E_FAIL;
		}
	}
	m_RenderList[GROUP_TERRAIN].clear();
	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	if (nullptr == m_pTarget_Manager)
	{
		MSG_BOX(L"Failed To CRenderer : Render_NonBlend : nullptr == m_pTarget_Manager");
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pDeviceContext, L"MRT_Deferred")))
	{
		MSG_BOX(L"Failed To CRenderer : Render_NonBlend : m_pTarget_Manager->Begin_MRT");
		return E_FAIL;
	}
	HRESULT hr = 0;
	for (auto& pGameObject : m_RenderList[GROUP_NONBLEND])
	{
		if (nullptr != pGameObject)
		{
			hr = pGameObject->Render();
			Safe_Release(pGameObject);
		}
		if (FAILED(hr))
		{
			MSG_BOX(L"Failed To CRenderer : Render_NonBlend : 0 == hr");
			return E_FAIL;
		}
	}
	m_RenderList[GROUP_NONBLEND].clear();
	if (FAILED(m_pTarget_Manager->End_MRT(m_pDeviceContext, L"MRT_Deferred")))
	{
		MSG_BOX(L"Failed To CRenderer : Render_NonBlend : m_pTarget_Manager->End_MRT");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
	if (nullptr == m_pTarget_Manager || nullptr == m_pLight_Manager)
	{
		MSG_BOX(L"Failed To CRenderer : Render_LightAcc : nullptr == m_pTarget_Manager || nullptr == m_pLight_Manager");
		return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pDeviceContext, L"MRT_LightAcc")))
	{
		MSG_BOX(L"Failed To CRenderer : Render_LightAcc : m_pTarget_Manager->Begin_MRT");
		return E_FAIL;
	}
	m_pLight_Manager->Render_Lights();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pDeviceContext, L"MRT_LightAcc")))
	{
		MSG_BOX(L"Failed To CRenderer : Render_LightAcc : m_pTarget_Manager->End_MRT");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CRenderer::Render_Deferred()
{
	if (FAILED(m_pShader->Set_ShaderResourceView("g_DiffuseTexture", m_pTarget_Manager->Get_SRV(L"Target_Diffuse"))))
	{
		MSG_BOX(L"Failed To CRenderer : Render_Deferred : m_pShader->Set_ShaderResourceView(g_DiffuseTexture)");
		return E_FAIL;
	}
	if (FAILED(m_pShader->Set_ShaderResourceView("g_ShadeTexture", m_pTarget_Manager->Get_SRV(L"Target_Shade"))))
	{
		MSG_BOX(L"Failed To CRenderer : Render_Deferred : m_pShader->Set_ShaderResourceView(g_ShadeTexture)");
		return E_FAIL;
	}
	if (FAILED(m_pShader->Set_ShaderResourceView("g_SpecularTexture", m_pTarget_Manager->Get_SRV(L"Target_Specular"))))
	{
		MSG_BOX(L"Failed To CRenderer : Render_Deferred : m_pShader->Set_ShaderResourceView(g_SpecularTexture)");
		return E_FAIL;
	}

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
	{
		MSG_BOX(L"Failed To CRenderer : Render_Deferred : m_pShader->Set_RawValue(g_WorldMatrix)");
		return E_FAIL;
	}
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
	{
		MSG_BOX(L"Failed To CRenderer : Render_Deferred : m_pShader->Set_RawValue(g_ViewMatrix)");
		return E_FAIL;
	}
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
	{
		MSG_BOX(L"Failed To CRenderer : Render_Deferred : m_pShader->Set_RawValue(g_ProjMatrix)");
		return E_FAIL;
	}
	m_pShader->Begin(3);
	m_pVIBuffer->Render();
	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	for (auto& pGameObject : m_RenderList[GROUP_NONLIGHT])
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->Render()))
			{
				MSG_BOX(L"Failed To CRenderer : Render_NonLight : pGameObject->Render");
				return E_FAIL;
			}
			Safe_Release(pGameObject);
		}		
	}
	m_RenderList[GROUP_NONLIGHT].clear();
	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	HRESULT hr = 0;
	m_RenderList[GROUP_ALPHABLEND].sort([](CGameObject* pSour, CGameObject* pDest) 
	{
		return ((CGameObject*)pSour)->Get_DistanceToCamera() > ((CGameObject*)pDest)->Get_DistanceToCamera();
	});

	for (auto& pGameObject : m_RenderList[GROUP_ALPHABLEND])
	{
		if (nullptr != pGameObject)
		{
			hr = pGameObject->Render();
			Safe_Release(pGameObject);
		}
		if (FAILED(hr))
		{
			MSG_BOX(L"Failed To CRenderer : Render_Blend : 0 == hr");
			return E_FAIL;
		}
	}
	m_RenderList[GROUP_ALPHABLEND].clear();	
	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	HRESULT hr = 0;
	for (auto& pGameObject : m_RenderList[GROUP_UI])
	{
		if (nullptr != pGameObject)
		{
			hr = pGameObject->Render();
			Safe_Release(pGameObject);
		}
		if (FAILED(hr))
		{
			MSG_BOX(L"Failed To CRenderer : Render_UI : 0 == hr");
			return E_FAIL;
		}
	}
	m_RenderList[GROUP_UI].clear();
	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Render_DebugComponents()
{
	for (auto& pComponent : m_DebugComponents)
	{
		if (nullptr != pComponent)
		{
			pComponent->Render();
			Safe_Release(pComponent);
		}
	}
	m_DebugComponents.clear();
	return S_OK;
}
#endif // _DEBUG

CRenderer* CRenderer::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CRenderer* pInstance = new CRenderer(pDeviceOut, pDeviceContextOut);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CRenderer : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CRenderer::Clone(void* pArg)
{
	AddRef();
	return this;
}

void CRenderer::Free()
{
	__super::Free();
	for (_uint i = 0; i < GROUP_END; ++i)
	{
		for (auto& pGameObject : m_RenderList[i])
		{
			Safe_Release(pGameObject);
		}m_RenderList[i].clear();
	}
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShader);

	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);
}