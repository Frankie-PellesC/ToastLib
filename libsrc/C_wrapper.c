/*+@@file@@----------------------------------------------------------------*//*!
 \file		C_wrapper.c
 \par Description 
 \par Description 
            Toast notification library based on former work of 
            Valentin-Gabriel Radu
 \par Project: 
            ToastLib
 \date		Created  on Sun Nov 19 23:56:17 2023
 \date		Modified on Sun Nov 19 23:56:17 2023
 \author	frankie
\*//*-@@file@@----------------------------------------------------------------*/
#ifdef __POCC__
#define  UNICODE
#define _UNICODE
#define __STDC_WANT_LIB_EXT1__ 1
#endif

#include <guiddef.h>
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <wchar.h>
#ifndef USE_WINDOWS_RUNTIME
#include "ToasterDecl.h"
#else
#include <winstring.h>
#include <roapi.h>
#include <Windows.ui.notifications.h>
#include <notificationactivationcallback.h>
#endif
#include "include/ToastLib.h"

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "uuid.lib")

#define wcsbytes(x)		(((DWORD)wcslen(x) + 1)<<2)

#define TOAST_NOTIFICATION_FACTORY		__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationFactory
#define TOAST_NOTIFIER					__x_ABI_CWindows_CUI_CNotifications_CIToastNotifier
typedef struct TOAST_NOTIFY
{
	WCHAR                      *wszAppId;
	TOAST_NOTIFICATION_FACTORY *pNotificationFactory;
	TOAST_NOTIFIER             *pToastNotifier;
	GUID                        GuidNotificationActivationCallback;
} TOAST_NOTIFY;

#define WSZ_GUID_BUFF_SIZE 64
/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		Guid2Wtext
 \date		Created  on Sat Nov 18 18:03:43 2023
 \date		Modified on Sat Nov 18 18:03:43 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
PCWCH WINAPI Guid2Wtext(const GUID *guid, BOOL bBraces)
{
	static WCHAR wszGuid[WSZ_GUID_BUFF_SIZE];
	swprintf(wszGuid, WSZ_GUID_BUFF_SIZE, L"%ls%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X%ls", bBraces ? L"{" : L"",
						guid->Data1, guid->Data2, guid->Data3, guid->Data4[0], guid->Data4[1], guid->Data4[2], guid->Data4[3],
													guid->Data4[4], guid->Data4[5], guid->Data4[6], guid->Data4[7], bBraces ? L"}" : L"");
	return (PCWCH)wszGuid;
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		fToastRegisterApp
 \details	The info here are used by COM manager, if the app isn't running 
            and someone interacts with the toast notification to run the 
            implementation of the interface.
            If these settings are missing, interacting with the 
            notifications will do nothing.
 \date		Created  on Sat Nov 18 16:59:37 2023
 \date		Modified on Sun Nov 19 21:37:57 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
BOOL WINAPI fToastRegisterApp(WCHAR  *wszAppPath, WCHAR  *wszAppid, WCHAR  *wszDescription, WCHAR  *IconBackgroundColor, const GUID *pGUID_INotificationActivationCallback)
{
	wchar_t  wszBuf[MAX_PATH];

	/*
	 * Construct the path to the EXE that will handle requests for this interface, so COM knows
	 * that this app implements the INotificationActivationCallback interface.
	 */
	swprintf(wszBuf, MAX_PATH, L"SOFTWARE\\Classes\\CLSID\\{%ls}\\LocalServer32", Guid2Wtext(pGUID_INotificationActivationCallback, FALSE));
	if (RegSetValueW(HKEY_CURRENT_USER, wszBuf, REG_SZ, wszAppPath, wcsbytes(wszAppPath)) != ERROR_SUCCESS)
		return FALSE;

	/*
	 * Associate our AUMID (Application User Model ID) with the app GUID above (the one that is associated with class factory
	 * which produces our INotificationActivationCallback interface) and set some properties.
	 */
	swprintf(wszBuf, MAX_PATH, L"SOFTWARE\\Classes\\AppUserModelId\\%ls", wszAppid);
	if (RegSetKeyValueW(HKEY_CURRENT_USER, wszBuf, L"DisplayName", REG_SZ, wszDescription, wcsbytes(wszDescription)) != ERROR_SUCCESS)
		return FALSE;

	if (RegSetKeyValueW(HKEY_CURRENT_USER, wszBuf, L"IconBackgroundColor", REG_SZ, IconBackgroundColor, wcsbytes(IconBackgroundColor)) != ERROR_SUCCESS)
		return FALSE;

	PCWCH wszGuid = Guid2Wtext(pGUID_INotificationActivationCallback, TRUE);
	if (RegSetKeyValueW(HKEY_CURRENT_USER, wszBuf, L"CustomActivator", REG_SZ, wszGuid, wcsbytes(wszGuid)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		GetNotificationFactory
 \date		Created  on Sat Nov 18 22:37:34 2023
 \date		Modified on Sat Nov 18 22:37:34 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static HRESULT GetNotificationFactory(H_TOAST hToast)
{
	HRESULT  hr;

	hToast->pToastNotifier       = NULL;
	hToast->pNotificationFactory = NULL;

	/*
	 * Obtain a runtime Toast manager handle.
	 */
	HSTRING_HEADER hshToastNotificationManager;
	HSTRING        hsToastNotificationManager = NULL;
	__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationManagerStatics *pToastNotificationManager = NULL;
	if (!SUCCEEDED(WindowsCreateStringReference(RuntimeClass_Windows_UI_Notifications_ToastNotificationManager,
							(UINT32)wcslen(RuntimeClass_Windows_UI_Notifications_ToastNotificationManager),
											&hshToastNotificationManager, &hsToastNotificationManager)))
		return S_FALSE;
	hr = RoGetActivationFactory(hsToastNotificationManager, &IID_IToastNotificationManagerStatics, (LPVOID*)&pToastNotificationManager);
	WindowsDeleteString(hsToastNotificationManager);
	if (!SUCCEEDED(hr))
		return S_FALSE;

	/*
	 * Creates and initializes a new instance of the ToastNotification bound to a specified app.
	 */
	HSTRING_HEADER hshAppId;
	HSTRING        hsAppId = NULL;
	if (!SUCCEEDED(WindowsCreateStringReference(hToast->wszAppId, (DWORD)wcslen(hToast->wszAppId), &hshAppId, &hsAppId)))
		return S_FALSE;
	hr = pToastNotificationManager->lpVtbl->CreateToastNotifierWithId(pToastNotificationManager, hsAppId, &hToast->pToastNotifier);
	WindowsDeleteString(hsAppId);
	pToastNotificationManager->lpVtbl->Release(pToastNotificationManager);
	if (!SUCCEEDED(hr))
		return S_FALSE;

	/*
	 * Obtain a runtime toast notification handler
	 */
	HSTRING_HEADER hshToastNotification;
	HSTRING        hsToastNotification = NULL;
	if (!SUCCEEDED(WindowsCreateStringReference(RuntimeClass_Windows_UI_Notifications_ToastNotification,
								(UINT32)wcslen(RuntimeClass_Windows_UI_Notifications_ToastNotification),
																			&hshToastNotification, &hsToastNotification)))
		return S_FALSE;
	hr = RoGetActivationFactory(hsToastNotification, &IID_IToastNotificationFactory, (LPVOID*)&hToast->pNotificationFactory);
	WindowsDeleteString(hsToastNotification);
	if (!SUCCEEDED(hr))
		return S_FALSE;

	return S_OK;
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		fToastDeleteNotifyManager
 \date		Created  on Sat Nov 18 23:47:43 2023
 \date		Modified on Sat Nov 18 23:47:43 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
VOID WINAPI fToastDeleteNotifyManager(H_TOAST hToast)
{
	if (hToast->pNotificationFactory)
		hToast->pNotificationFactory->lpVtbl->Release(hToast->pNotificationFactory);
	if (hToast->pToastNotifier)
		hToast->pToastNotifier->lpVtbl->Release(hToast->pToastNotifier);

	if (hToast->wszAppId)
		free(hToast->wszAppId);

	free(hToast);
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		fToastCreateNotifyManager
 \date		Created  on Sat Nov 18 23:16:48 2023
 \date		Modified on Sat Nov 18 23:16:48 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
HRESULT WINAPI fToastCreateNotifyManager(H_TOAST *hToastNotify, WCHAR *app_id, const GUID *GuidNotificationActivationCallback)
{
	*hToastNotify = NULL;
	H_TOAST hToast = calloc(1, sizeof(TOAST_NOTIFY));
	if (!hToast)
		return E_OUTOFMEMORY;
	hToast->wszAppId = _wcsdup(app_id);
	if (!hToast->wszAppId)
	{
		free(hToast);
		return E_OUTOFMEMORY;
	}
	hToast->GuidNotificationActivationCallback = *GuidNotificationActivationCallback;
	HRESULT hr = GetNotificationFactory(hToast);
	if (!SUCCEEDED(hr))
	{
		fToastDeleteNotifyManager(hToast);
		return hr;
	}

	*hToastNotify = hToast;
	return S_OK;
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		fToastSendNotification
 \date		Created  on Sat Nov 18 16:47:00 2023
 \date		Modified on Sat Nov 18 16:47:00 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
BOOL WINAPI fToastSendNotification(H_TOAST hToast, LPCWSTR wszTemplate)
{
	HRESULT  hr;

	/*
	 * Activates an XML Windows Runtime class.
	 */
	HSTRING_HEADER hshXmlDocument;
	HSTRING        hsXmlDocument = NULL;
	IInspectable  *pInspectable  = NULL;
	if (!SUCCEEDED(WindowsCreateStringReference(RuntimeClass_Windows_Data_Xml_Dom_XmlDocument,
					(UINT32)wcslen(RuntimeClass_Windows_Data_Xml_Dom_XmlDocument), &hshXmlDocument, &hsXmlDocument)))
		return FALSE;
	hr = RoActivateInstance(hsXmlDocument, &pInspectable);
	WindowsDeleteString(hsXmlDocument);
	if (!SUCCEEDED(hr))
		return FALSE;
	/*
	 * Instanziate IXmlDocument class
	 */
	__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument *pXmlDocument = NULL;
	if (!SUCCEEDED(pInspectable->lpVtbl->QueryInterface(pInspectable, &IID_IXmlDocument, (void **)&pXmlDocument)))
		return FALSE;
	pInspectable->lpVtbl->Release(pInspectable);
	/*
	 * Instanziate IXmlDocumentIO class
	 */
	__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentIO *pXmlDocumentIO = NULL;
	if (!SUCCEEDED(pXmlDocument->lpVtbl->QueryInterface(pXmlDocument, &IID_IXmlDocumentIO, (void **)&pXmlDocumentIO)))
		return FALSE;

	HSTRING_HEADER hshBanner;
	HSTRING        hsBanner = NULL;
	if (!SUCCEEDED(WindowsCreateStringReference(wszTemplate, (UINT32)wcslen(wszTemplate), &hshBanner, &hsBanner)))
		return FALSE;
	hr = pXmlDocumentIO->lpVtbl->LoadXml(pXmlDocumentIO, hsBanner);
	WindowsDeleteString(hsBanner);
	pXmlDocumentIO->lpVtbl->Release(pXmlDocumentIO);
	if (!SUCCEEDED(hr))
		return FALSE;

	__x_ABI_CWindows_CUI_CNotifications_CIToastNotification *pToastNotification = NULL;
	if (!SUCCEEDED(hToast->pNotificationFactory->lpVtbl->CreateToastNotification(hToast->pNotificationFactory, pXmlDocument, &pToastNotification)))
		return FALSE;
	pXmlDocument->lpVtbl->Release(pXmlDocument);

	if (!SUCCEEDED(hToast->pToastNotifier->lpVtbl->Show(hToast->pToastNotifier, pToastNotification)))
		return FALSE;
	pToastNotification->lpVtbl->Release(pToastNotification);

	return TRUE;
}
