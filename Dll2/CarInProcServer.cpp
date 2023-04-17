#include <windows.h>
#include "CoCarClassFactory.h"

ULONG g_lockCount = 0;
ULONG g_objCount = 0;

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
	HRESULT hr;
	CoCarClassFactory* pCFact = NULL;

	if (rclsid != CLSID_CoCar)
		return CLASS_E_CLASSNOTAVAILABLE;

	pCFact = new CoCarClassFactory;

	hr = pCFact->QueryInterface(riid, ppv);

	if (FAILED(hr))
		delete pCFact;

	return hr;
}

STDAPI DllCanUnloadNow()
{
	if (g_lockCount == 0 && g_objCount == 0)
	{
		return S_OK;
	}
	else
		return S_FALSE;
}
