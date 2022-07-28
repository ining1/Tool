#include "..\Public\Layer.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
	{
		MSG_BOX(L"Failed To CLayer : Add_GameObject : nullptr == pGameObject");
		return E_FAIL;
	}
	m_ObjectList.push_back(pGameObject);
	return S_OK;
}

void CLayer::Tick(_double TimeDelta)
{
	for (auto& iter = m_ObjectList.begin(); iter != m_ObjectList.end();)
	{
		(*iter)->Tick(TimeDelta);
		++iter;
	}
}

void CLayer::LateTick(_double TimeDelta)
{
	for (auto& iter = m_ObjectList.begin(); iter != m_ObjectList.end();)
	{
		(*iter)->LateTick(TimeDelta);
		if (true == (*iter)->Get_Death())
		{
			Safe_Release(*iter);
			iter = m_ObjectList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

CComponent* CLayer::Get_Component(const _tchar* pComponentTag, _uint iIndex)
{
	if (iIndex >= m_ObjectList.size())
	{
		return nullptr;
	}
	auto iter = m_ObjectList.begin();
	for (_uint i = 0; i < iIndex; ++i)
	{
		++iter;
	}
	return (*iter)->Get_Component(pComponentTag);
}

list<class CGameObject*>* CLayer::Get_List()
{
	return &m_ObjectList;
}

CLayer* CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	for (auto& pGameObject : m_ObjectList)
	{
		Safe_Release(pGameObject);
	}
	m_ObjectList.clear();
}