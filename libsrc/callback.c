/*+@@file@@----------------------------------------------------------------*//*!
 \file		Interfaces.c
 \par Description 
 \par Description 
            Toast notification library based on former work of 
            Valentin-Gabriel Radu
 \par Project: 
            ToastLib
 \date		Created  on Sun Nov 19 23:55:14 2023
 \date		Modified on Sun Nov 19 23:55:14 2023
 \author	frankie
\*//*-@@file@@----------------------------------------------------------------*/
#ifdef __POCC__
#define  UNICODE
#define _UNICODE
#define __STDC_WANT_LIB_EXT1__ 1
#endif

#include <initguid.h>
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <wchar.h>
#ifndef USE_WINDOWS_RUNTIME
#include "ToasterDecl.h"
#else
#include <winstring.h>
#include <roapi.h>
#include <EventToken.h>
#include <Windows.ui.notifications.h>
#include <notificationactivationcallback.h>
#endif
#include "include/ToastLib.h"

/*
 * The following is the memory layout used for all local objects
 * (class factory and, INotificationActivationCallback.).
 * All objects inherit from IUnknown.
 */
typedef struct InterfaceGeneric
{
	IUnknownVtbl   *lpVtbl;
	LONG64          dwRefCount;
	FN_TOAST_NOTIFY fnToastNotify;
	INT_PTR         arg;
} InterfaceGeneric;

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		Impl_IGeneric_AddRef
 \date		Created  on Thu Nov  9 15:00:07 2023
 \date		Modified on Thu Nov  9 15:00:07 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static FORCEINLINE ULONG STDMETHODCALLTYPE Impl_IGeneric_AddRef(InterfaceGeneric *this)
{
	return (ULONG)InterlockedIncrement64(&this->dwRefCount);
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		Impl_IGeneric_Release
 \date		Created  on Thu Nov  9 15:00:14 2023
 \date		Modified on Thu Nov  9 15:00:14 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static FORCEINLINE ULONG STDMETHODCALLTYPE Impl_IGeneric_Release(InterfaceGeneric *this)
{
	LONG64 dwNewRefCount = InterlockedDecrement64(&this->dwRefCount);
	if (!dwNewRefCount) free(this);
	return (ULONG)dwNewRefCount;
}

/*
 * INotificationActivationCallback implementation.
 * This implementation inherit from the INotificationActivationCallback, adding a
 * property to set the callback user function.
 */
#pragma region INotificationActivationCallback
/*
 * This is where the magic happens when someone interacts with our notification:
 * this method will be called (on another thread !!!).
 */

typedef struct
{
	INotificationActivationCallbackVtbl;
	HRESULT (STDMETHODCALLTYPE *SetUserFn)(INotificationActivationCallback *this, FN_TOAST_NOTIFY fnToastNotify);
} InterfaceNotifyVtbl;


/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		Impl_INotificationActivationCallback_QueryInterface
 \date		Created  on Thu Nov  9 14:59:55 2023
 \date		Modified on Thu Nov  9 14:59:55 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static HRESULT STDMETHODCALLTYPE Impl_INotificationActivationCallback_QueryInterface(INotificationActivationCallback* this, REFIID riid, void **ppvObject)
{
	if (!IsEqualIID(riid, &IID_INotificationActivationCallback) && !IsEqualIID(riid, &IID_IUnknown))
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	*ppvObject = this;
	this->lpVtbl->AddRef(this);
	return S_OK;
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		Impl_INotificationActivationCallback_Activate
 \date		Created  on Thu Nov  9 15:00:01 2023
 \date		Modified on Thu Nov  9 15:00:01 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static HRESULT STDMETHODCALLTYPE Impl_INotificationActivationCallback_Activate(INotificationActivationCallback* this, LPCWSTR appUserModelId, LPCWSTR invokedArgs, const NOTIFICATION_USER_INPUT_DATA* data, ULONG count)
{
	if (((InterfaceGeneric *)this)->fnToastNotify)
	{
		((InterfaceGeneric *)this)->fnToastNotify(((InterfaceGeneric *)this)->arg, appUserModelId, invokedArgs, data, count);
		return S_OK;
	}

	return S_FALSE;
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		Impl_INotificationActivationCallback_AddRef
 \date		Created  on Thu Nov  9 15:00:07 2023
 \date		Modified on Thu Nov  9 15:00:07 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static ULONG STDMETHODCALLTYPE Impl_INotificationActivationCallback_AddRef(INotificationActivationCallback *this)
{
	return Impl_IGeneric_AddRef((InterfaceGeneric *)this);
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		Impl_INotificationActivationCallback_Release
 \date		Created  on Thu Nov  9 15:00:14 2023
 \date		Modified on Thu Nov  9 15:00:14 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static ULONG STDMETHODCALLTYPE Impl_INotificationActivationCallback_Release(INotificationActivationCallback *this)
{
	return Impl_IGeneric_Release((InterfaceGeneric *)this);
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		Impl_INotificationActivationCallback_SetUserFn
 \date		Created  on Sun Nov 19 12:03:39 2023
 \date		Modified on Sun Nov 19 12:03:39 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static HRESULT STDMETHODCALLTYPE Impl_INotificationActivationCallback_SetUserFn(INotificationActivationCallback *this, FN_TOAST_NOTIFY fnToastNotify)
{
	((InterfaceGeneric *)this)->fnToastNotify = fnToastNotify;
	return S_OK;
}

/*
 * Create modified Vtable for the class.
 */
static const InterfaceNotifyVtbl Impl_InterfaceNotifyVtbl =
{
	{
		.QueryInterface = Impl_INotificationActivationCallback_QueryInterface,
		.AddRef         = Impl_INotificationActivationCallback_AddRef,
		.Release        = Impl_INotificationActivationCallback_Release,
		.Activate       = Impl_INotificationActivationCallback_Activate
	},
	Impl_INotificationActivationCallback_SetUserFn
};
#pragma endregion

/*
 * IClassFactory implementation.
 */
#pragma region "IClassFactory Generic implementation"
typedef struct This_factory
{
	IClassFactoryVtbl *lpVtbl;
	LONG64             dwRefCount;
} This_factory;

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		Impl_IClassFactory_QueryInterface
 \date		Created  on Thu Nov  9 15:00:40 2023
 \date		Modified on Thu Nov  9 15:00:40 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static HRESULT STDMETHODCALLTYPE Impl_IClassFactory_QueryInterface(LPCLASSFACTORY this, REFIID riid, void** ppvObject)
{
	if (!IsEqualIID(riid, &IID_IClassFactory) && !IsEqualIID(riid, &IID_IUnknown))
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	*ppvObject = this;
	((This_factory *)this)->lpVtbl->AddRef(this);
	return S_OK;
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		Impl_IClassFactory_LockServer
 \date		Created  on Thu Nov  9 15:00:48 2023
 \date		Modified on Thu Nov  9 15:00:48 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static HRESULT STDMETHODCALLTYPE Impl_IClassFactory_LockServer(LPCLASSFACTORY this_, BOOL flock)
{
	return S_OK;
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		Impl_IClassFactory_CreateInstance
 \date		Created  on Thu Nov  9 15:00:55 2023
 \date		Modified on Thu Nov  9 15:00:55 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static HRESULT STDMETHODCALLTYPE Impl_IClassFactory_CreateInstance(LPCLASSFACTORY this, IUnknown *punkOuter, REFIID vTableGuid, void **ppv)
{
	*ppv = NULL;

	if (punkOuter)
	{
		return CLASS_E_NOAGGREGATION;
	}

	InterfaceGeneric *thisobj = malloc(sizeof(InterfaceGeneric));
	if (!thisobj)
		return E_OUTOFMEMORY;

	thisobj->lpVtbl        = (IUnknownVtbl *)&Impl_InterfaceNotifyVtbl;
	thisobj->dwRefCount    = 1;
	thisobj->fnToastNotify = ((InterfaceGeneric *)this)->fnToastNotify;
	thisobj->arg           = ((InterfaceGeneric *)this)->arg;
	HRESULT hr             = thisobj->lpVtbl->QueryInterface((LPUNKNOWN)thisobj, vTableGuid, ppv);
	thisobj->lpVtbl->Release((LPUNKNOWN)thisobj);

	return hr;
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		Impl_IClassFactory_AddRef
 \date		Created  on Thu Nov  9 15:01:02 2023
 \date		Modified on Thu Nov  9 15:01:02 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static ULONG STDMETHODCALLTYPE Impl_IClassFactory_AddRef(LPCLASSFACTORY this)
{
	return Impl_IGeneric_AddRef((InterfaceGeneric *)this);
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		Impl_IClassFactory_Release
 \date		Created  on Thu Nov  9 15:01:09 2023
 \date		Modified on Thu Nov  9 15:01:09 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static ULONG STDMETHODCALLTYPE Impl_IClassFactory_Release(LPCLASSFACTORY this)
{
	return Impl_IGeneric_Release((InterfaceGeneric *)this);
}


static const IClassFactoryVtbl Impl_IClassFactory_Vtbl =
{
	.QueryInterface = Impl_IClassFactory_QueryInterface,
	.AddRef         = Impl_IClassFactory_AddRef,
	.Release        = Impl_IClassFactory_Release,
	.LockServer     = Impl_IClassFactory_LockServer,
	.CreateInstance = Impl_IClassFactory_CreateInstance
};

typedef struct TOAST_CALLBACK
{
	LPCLASSFACTORY lpClassFactory;
	DWORD          dwRegister;
} TOAST_CALLBACK;

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		fToastRegisterCallback
 \date		Created  on Fri Nov 17 17:53:59 2023
 \date		Modified on Fri Nov 17 17:53:59 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
HRESULT WINAPI fToastRegisterCallback(H_CALLBACK *hCallback, const GUID *pGUID_INotificationActivationCallback, FN_TOAST_NOTIFY fnToastNotify, INT_PTR arg)
{
	*hCallback = calloc(1, sizeof(TOAST_CALLBACK));
	if (!*hCallback)
		return S_FALSE;

	/*
	 * Allocate class factory.
	 * This factory produces our implementation of the INotificationActivationCallback interface.
	 * This interface has an ::Activate member method that gets called when someone interacts with
	 * the toast notification.
	 */
	InterfaceGeneric *pClass  = malloc(sizeof(InterfaceGeneric));
	if (!pClass)
		return E_OUTOFMEMORY;
	pClass->lpVtbl            = (IUnknownVtbl *)&Impl_IClassFactory_Vtbl;
	pClass->dwRefCount        = 1;
	pClass->fnToastNotify     = fnToastNotify;
	pClass->arg               = arg;
	(*hCallback)->lpClassFactory = (LPCLASSFACTORY)pClass;

	/*
	 * And register the class object.
	 */
	if (!SUCCEEDED(CoRegisterClassObject(pGUID_INotificationActivationCallback,
					(LPUNKNOWN)(*hCallback)->lpClassFactory, CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE, &((*hCallback)->dwRegister))))
	{
		(*hCallback)->lpClassFactory->lpVtbl->Release((*hCallback)->lpClassFactory);
		return S_FALSE;
	}

	return S_OK;
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		fToastUnRegisterCallback
 \date		Created  on Sun Nov 19 23:10:57 2023
 \date		Modified on Sun Nov 19 23:10:57 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
VOID WINAPI fToastUnRegisterCallback(H_CALLBACK hCallback)
{
	if (hCallback->dwRegister)
	{
		CoRevokeClassObject(hCallback->dwRegister);
	}
	if (hCallback->lpClassFactory)
	{
		hCallback->lpClassFactory->lpVtbl->Release(hCallback->lpClassFactory);
	}
	free(hCallback);
}
#pragma endregion
