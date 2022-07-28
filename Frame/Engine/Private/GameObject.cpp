#include "..\Public\GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:m_pDevice(pDevice)
	,m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

CGameObject::CGameObject(const CGameObject& rhs)
	:m_pDevice(rhs.m_pDevice)
	,m_pDeviceContext(rhs.m_pDeviceContext)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CGameObject::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::NativeConstruct(void* pArg)
{
	if (nullptr != Find_Component(L"Com_Transform"))
	{
		MSG_BOX(L"Failed To CGameObject : NativeConstruct : nullptr != Find_Component");
		return E_FAIL;
	}
	m_pTransform = CTransform::Create(m_pDevice, m_pDeviceContext);
	m_Components.insert(COMPONENTS::value_type(L"Com_Transform", m_pTransform));
	Safe_AddRef(m_pTransform);

	return S_OK;
}

void CGameObject::Tick(_double TimeDelta)
{
}

void CGameObject::LateTick(_double TimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

CComponent* CGameObject::Get_Component(const _tchar* pComponentTag)
{
	return Find_Component(pComponentTag);
}

_bool CGameObject::Get_Death()
{
	return m_bDeath;
}

_float CGameObject::Get_DistanceToCamera()
{
	return m_fDistanceToCamera;
}

void CGameObject::Set_Dead(_bool bDead)
{
	m_bDeath = bDead;
}

HRESULT CGameObject::SetUp_Component(const _tchar* pComponentTag, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (nullptr != Find_Component(pComponentTag))
	{
		MSG_BOX(L"Failed To CGameObject : SetUp_Component : nullptr != Find_Component");
		return E_FAIL;
	}

	CComponent* pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
	{
		MSG_BOX(L"Failed To CGameObject : SetUp_Component : nullptr == pComponent");
		return E_FAIL;
	}
	m_Components.insert(COMPONENTS::value_type(pComponentTag, pComponent));
	*ppOut = pComponent;
	Safe_AddRef(pComponent);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CComponent* CGameObject::Find_Component(const _tchar* pComponentTag)
{
	auto iter = find_if(m_Components.begin(), m_Components.end(), CTagFinder(pComponentTag));
	if (iter == m_Components.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CGameObject::Free()
{
	Safe_Release(m_pTransform);

	for (auto& Pair : m_Components)
	{
		Safe_Release(Pair.second);
	}
	m_Components.clear();

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}