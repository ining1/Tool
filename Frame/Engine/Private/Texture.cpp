#include "..\Public\Texture.h"

CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
	, m_Textures(rhs.m_Textures)
{
	for (auto& pSRV : m_Textures)
	{
		Safe_AddRef(pSRV);
	}
}

HRESULT CTexture::NativeConstruct_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures)
{
	_tchar szTextureFilePath[MAX_PATH] = L"";
	for (_uint i = 0; i < iNumTextures; ++i)
	{
		wsprintf(szTextureFilePath, pTextureFilePath, i);
		_tchar szExt[MAX_PATH] = L"";
		_wsplitpath_s(szTextureFilePath,  nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);
		ID3D11ShaderResourceView* pSRV = nullptr;

		HRESULT hr = 0;

		if (!lstrcmp(szExt, L".dds"))
		{
			hr = DirectX::CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}
		else
		{
			hr = DirectX::CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}
		if (FAILED(hr))
		{
			MSG_BOX(L"Failed To CTexture : NativeConstruct_Prototype : 0 == hr");
			return E_FAIL;
		}
		m_Textures.push_back(pSRV);		
	}
	return S_OK;
}

HRESULT CTexture::NativeConstruct(void* pArg)
{
	return S_OK;
}

HRESULT CTexture::SetUp_ShaderResourceView(CShader* pShader, const char* pConstantName, _uint iTextureIndex)
{
	if (iTextureIndex >= m_Textures.size())
	{
		MSG_BOX(L"Failed To CTexture : SetUp_ShaderResourceView : iTextureIndex >= m_Textures.size");
		return E_FAIL;
	}
	return pShader->Set_ShaderResourceView(pConstantName, m_Textures[iTextureIndex]);	
}

CTexture* CTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pTextureFilePath, _uint iNumTextures)
{
	CTexture* pInstance = new CTexture(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype(pTextureFilePath, iNumTextures)))
	{
		MSG_BOX(L"Failed To CTexture : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
	CTexture* pInstance = new CTexture(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CTexture : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTexture::Free()
{
	__super::Free();
	for (auto& pSRV : m_Textures)
	{
		Safe_Release(pSRV);
	}
	m_Textures.clear();
}