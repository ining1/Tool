#pragma once
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CLayer final : public CBase
{
public:
	CLayer();
	virtual ~CLayer() = default;
public:
	HRESULT Add_GameObject(CGameObject* pGameObject);
	void Tick(_double TimeDelta);
	void LateTick(_double TimeDelta);
public:
	CComponent* Get_Component(const _tchar* pComponentTag, _uint iIndex = 0);
	list<class CGameObject*>* Get_List();
private:
	list<class CGameObject*> m_ObjectList;
	typedef list<class CGameObject*> OBJECTLIST;
public:
	static CLayer* Create();
	virtual void Free() override;
};

END