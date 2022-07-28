#include "..\Public\Shader.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CShader::CShader(const CShader& rhs)
	:CComponent(rhs)
	,m_pEffect(rhs.m_pEffect)
	,m_Passes(rhs.m_Passes)
{
	for (auto& pPassDesc : m_Passes)
	{
		Safe_AddRef(pPassDesc->pInputLayout);
	}
	Safe_AddRef(m_pEffect);
}

HRESULT CShader::NativeConstruct_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	_uint iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	ID3DBlob* pCode = nullptr;
	ID3DBlob* pErrorMsg = nullptr;	

	if (FAILED(D3DCompileFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, nullptr, "fx_5_0", iFlag, 0, &pCode, &pErrorMsg)))
	{
		MSG_BOX(L"Failed To CShader : NativeConstruct_Prototype : D3DCompileFromFile");
		return E_FAIL;
	}

	if (FAILED(D3DX11CreateEffectFromMemory(pCode->GetBufferPointer(), pCode->GetBufferSize(), 0, m_pDevice, &m_pEffect)))
	{
		MSG_BOX(L"Failed To CShader : NativeConstruct_Prototype : D3DX11CreateEffectFromMemory");
		return E_FAIL;
	}

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
	{
		MSG_BOX(L"Failed To CShader : NativeConstruct_Prototype : nullptr == pTechnique");
		return E_FAIL;
	}

	D3DX11_TECHNIQUE_DESC TechniqueDesc;
	pTechnique->GetDesc(&TechniqueDesc);

	for (_uint i = 0; i < TechniqueDesc.Passes; ++i)
	{
		PASSDESC* pPassDesc = new PASSDESC;
		ZeroMemory(pPassDesc, sizeof(PASSDESC));

		pPassDesc->pEffectPass = pTechnique->GetPassByIndex(i);

		D3DX11_PASS_DESC PassDesc;
		pPassDesc->pEffectPass->GetDesc(&PassDesc);

		if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pPassDesc->pInputLayout)))
		{
			MSG_BOX(L"Failed To CShader : NativeConstruct_Prototype : m_pDevice->CreateInputLayout");
			return E_FAIL;
		}
		m_Passes.push_back(pPassDesc);
	}
	return S_OK;
}

HRESULT CShader::NativeConstruct(void* pArg)
{
	return S_OK;
}

HRESULT CShader::Set_RawValue(const char* pConstantName, const void* pData, _uint iDataByteLength)
{
	if (nullptr == m_pEffect)
	{
		MSG_BOX(L"Failed To CShader : Set_RawValue : nullptr == m_pEffect");
		return E_FAIL;
	}
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
	{
		MSG_BOX(L"Failed To CShader : Set_RawValue : nullptr == pVariable");
		return E_FAIL;
	}
	return pVariable->SetRawValue(pData, 0, iDataByteLength);	
}

HRESULT CShader::Set_ShaderResourceView(const char* pConstantName, ID3D11ShaderResourceView* pSRV)
{
	if (nullptr == m_pEffect)
	{
		MSG_BOX(L"Failed To CShader : Set_ShaderResourceView : nullptr == m_pEffect");
		return E_FAIL;
	}

	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
	{
		MSG_BOX(L"Failed To CShader : Set_ShaderResourceView : nullptr == pVariable");
		return E_FAIL;
	}

	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVariable)
	{
		MSG_BOX(L"Failed To CShader : Set_ShaderResourceView : nullptr == pSRVariable");
		return E_FAIL;
	}

	if(FAILED(pSRVariable->SetResource(pSRV)))
	{
		MSG_BOX(L"Failed To CShader : Set_ShaderResourceView : pSRVariable->SetResource");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CShader::Begin(_uint iPassIndex)
{
	if (iPassIndex >= m_Passes.size())
	{
		MSG_BOX(L"Failed To CShader : Begin : iPassIndex >= m_Passes.size");
		return E_FAIL;
	}
	m_pDeviceContext->IASetInputLayout(m_Passes[iPassIndex]->pInputLayout);
	m_Passes[iPassIndex]->pEffectPass->Apply(0, m_pDeviceContext);
	return S_OK;
}

CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	CShader* pInstance = new CShader(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype(pShaderFilePath, pElements, iNumElements)))
	{
		MSG_BOX(L"Failed To CShader : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
	CShader* pInstance = new CShader(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CShader : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShader::Free()
{
	__super::Free();
	for (auto& pPassDesc : m_Passes)
	{
		Safe_Release(pPassDesc->pInputLayout);
		if (false == m_isCloned)
		{
			Safe_Delete(pPassDesc);
		}	
	}
	m_Passes.clear();
	Safe_Release(m_pEffect);
}