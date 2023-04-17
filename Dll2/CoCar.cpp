#include "CoCar.h"

extern DWORD g_objCount;

CoCar::CoCar() {
    m_refCount = 0;
    m_currSpeed = 0;
    m_maxSpeed = 0;
    m_petName = SysAllocString(L"Default Pet Name");
    ++g_objCount;
}

CoCar::~CoCar() {
    --g_objCount;
    if (m_petName)
        SysFreeString(m_petName);
    MessageBox(NULL, L"CoCar is dead", L"Destructor", MB_OK |
        MB_SETFOREGROUND);

}

STDMETHODIMP_(HRESULT __stdcall) CoCar::QueryInterface(REFIID riid, void** pIFace) {
    // Which aspect of me do they want?
    if (riid == IID_IUnknown)
    {
        *pIFace = (IUnknown*)(IEngine*)this;
        MessageBox(NULL, L"Handed out IUnknown", L"QI", MB_OK |
            MB_SETFOREGROUND);
    }

    else if (riid == IID_IEngine)
    {
        *pIFace = (IEngine*)this;
        MessageBox(NULL, L"Handed out IEngine", L"QI", MB_OK |
            MB_SETFOREGROUND);
    }

    else if (riid == IID_IStats)
    {
        *pIFace = (IStats*)this;
        MessageBox(NULL, L"Handed out IStats", L"QI", MB_OK |
            MB_SETFOREGROUND);
    }

    else if (riid == IID_ICreateCar)
    {
        *pIFace = (ICreateCar*)this;
        MessageBox(NULL, L"Handed out ICreateCar", L"QI", MB_OK |
            MB_SETFOREGROUND);
    }
    else
    {
        *pIFace = NULL;
        return E_NOINTERFACE;
    }

    ((IUnknown*)(*pIFace))->AddRef();
    return S_OK;

}

STDMETHODIMP_(DWORD __stdcall) CoCar::AddRef() {
    ++m_refCount;
    return m_refCount;
}

STDMETHODIMP_(DWORD __stdcall) CoCar::Release() {
    if (--m_refCount == 0) {
        delete this;
        return 0;
    }
    else
        return m_refCount;
}

// IEngine
STDMETHODIMP CoCar::SpeedUp()
{
    m_currSpeed += 10;
    return S_OK;
}

STDMETHODIMP CoCar::GetMaxSpeed(int* maxSpeed)
{
    *maxSpeed = m_maxSpeed;
    return S_OK;
}

STDMETHODIMP CoCar::GetCurSpeed(int* curSpeed)
{
    *curSpeed = m_currSpeed;
    return S_OK;
}

// ICreateCar
STDMETHODIMP CoCar::SetPetName(BSTR petName)
{
    SysReAllocString(&m_petName, petName);
    return S_OK;
}

STDMETHODIMP CoCar::SetMaxSpeed(int maxSp)
{
    if (maxSp < MAX_SPEED)
        m_maxSpeed = maxSp;
    return S_OK;
}

STDMETHODIMP CoCar::GetPetName(BSTR* petName)
{
    *petName = SysAllocString(m_petName);
    return S_OK;
}

STDMETHODIMP CoCar::DisplayStats()
{
    // Need to transfer a BSTR to a char array.
    char buff[20];

    WideCharToMultiByte(CP_ACP, NULL, m_petName, -1, buff,
        MAX_NAME_LENGTH, NULL, NULL);

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide = converter.from_bytes(buff);

    LPCWSTR wideChar = wide.c_str();

    MessageBox(NULL, wideChar, L"Pet Name", MB_OK | MB_SETFOREGROUND);
    memset(buff, 0, sizeof(buff));
    sprintf_s(buff, "%d", m_maxSpeed);
    MessageBox(NULL, wideChar, L"Max Speed", MB_OK |
        MB_SETFOREGROUND);
    return S_OK;
}

STDMETHODIMP CoCar::GetTypeInfoCount(UINT* pctinfo)
{
    *pctinfo = 0;
    return S_OK;
}

STDMETHODIMP CoCar::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
{
    return E_NOTIMPL;
}

STDMETHODIMP CoCar::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames,
    LCID lcid, DISPID* rgDispId)
{
    return E_NOTIMPL;
}

STDMETHODIMP CoCar::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
    DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo,
    UINT* puArgErr)
{
    if (dispIdMember == 0x01) // IEngine::SpeedUp
    {
        return SpeedUp();
    }
    else if (dispIdMember == 0x02) // IEngine::GetMaxSpeed
    {
        if (pVarResult != NULL && pDispParams != NULL && pDispParams->cArgs == 1 &&
            pDispParams->rgvarg != NULL && pDispParams->rgvarg->vt == VT_BYREF)
        {
            int maxSpeed;
            HRESULT hr = GetMaxSpeed(&maxSpeed);
            if (SUCCEEDED(hr))
            {
                pVarResult->vt = VT_I4;
                pVarResult->lVal = maxSpeed;
                return S_OK;
            }
            else
            {
                return hr;
            }
        }
        else
        {
            return DISP_E_BADPARAMCOUNT;
        }
    }
    else if (dispIdMember == 0x03) // IEngine::GetCurSpeed
    {
        if (pVarResult != NULL && pDispParams != NULL && pDispParams->cArgs == 1 &&
            pDispParams->rgvarg != NULL && pDispParams->rgvarg->vt == VT_BYREF)
        {
            int curSpeed;
            HRESULT hr = GetCurSpeed(&curSpeed);
            if (SUCCEEDED(hr))
            {
                pVarResult->vt = VT_I4;
                pVarResult->lVal = curSpeed;
                return S_OK;
            }
            else
            {
                return hr;
            }
        }
        else
        {
            return DISP_E_BADPARAMCOUNT;
        }
    }
    else if (dispIdMember == 0x04) // IStats::DisplayStats
    {
        return DisplayStats();
    }
    else if (dispIdMember == 0x05) // IStats::GetPetName
    {
        if (pVarResult != NULL && pDispParams == NULL)
        {
            BSTR petName;
            HRESULT hr = GetPetName(&petName);
            if (SUCCEEDED(hr))
            {
                pVarResult->vt = VT_BSTR;
                pVarResult->bstrVal = petName;
                return S_OK;
            }
            else
            {
                return hr;
            }
        }
        else
        {
            return DISP_E_BADPARAMCOUNT;
        }
    }
    else if (dispIdMember == 0x06) // ICreateCar::SetPetName
    {
        if (pDispParams != NULL && pDispParams->cArgs == 1 && pDispParams->rgvarg != NULL &&
            pDispParams->rgvarg->vt == VT_BSTR)
        {
            return SetPetName(pDispParams->rgvarg->bstrVal);
        }
        else
        {
            return DISP_E_BADPARAMCOUNT;
        }
    }
    else if (dispIdMember == 0x07) // ICreateCar::SetMaxSpeed
    {
        if (pDispParams != NULL && pDispParams->cArgs == 1 && pDispParams->rgvarg != NULL &&
            pDispParams->rgvarg->vt == VT_I4)
        {
            return SetMaxSpeed(pDispParams->rgvarg->lVal);
        }
        else
        {
            return DISP_E_BADPARAMCOUNT;
        }
    }
    else
    {
        return DISP_E_MEMBERNOTFOUND;
    }
}
