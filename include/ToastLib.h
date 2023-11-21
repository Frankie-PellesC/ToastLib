/*+@@file@@----------------------------------------------------------------*//*!
 \file		ToastLib.h
 \par Description 
            Toast notification library based on former work of 
            Valentin-Gabriel Radu
 \par Project: 
            ToastLib
 \date		Created  on Mon Nov 20 12:44:57 2023
 \date		Modified on Mon Nov 20 12:44:57 2023
 \author	frankie
\*//*-@@file@@----------------------------------------------------------------*/
#ifndef ToastLib_h_
#define ToastLib_h_
#if __POCC__ >= 500
#pragma once
#endif

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "uuid.lib")

#ifndef NOTIFICATION_USER_INPUT_DATA_DEFINED_
#define NOTIFICATION_USER_INPUT_DATA_DEFINED_
typedef struct NOTIFICATION_USER_INPUT_DATA
{
	LPCWSTR Key;
	LPCWSTR Value;
} NOTIFICATION_USER_INPUT_DATA;
#endif
typedef struct TOAST_NOTIFY TOAST_NOTIFY;
typedef TOAST_NOTIFY *H_TOAST;
typedef struct TOAST_CALLBACK TOAST_CALLBACK;
typedef TOAST_CALLBACK *H_CALLBACK;
typedef INT_PTR (WINAPI *FN_TOAST_NOTIFY)(INT_PTR args, LPCWSTR appUserModelId, LPCWSTR invokedArgs, const NOTIFICATION_USER_INPUT_DATA* data, ULONG count);
/*
 * ROAPI definitions
 */
#ifndef ROAPI
#ifdef _ROAPI_
#define ROAPI
#else
#define ROAPI DECLSPEC_IMPORT
#endif // _ROAPI_
typedef enum
{
	RO_INIT_MULTITHREADED = 1
} RO_INIT_TYPE;
ROAPI HRESULT WINAPI RoInitialize(RO_INIT_TYPE initType);
ROAPI void WINAPI RoUninitialize(void);
#endif

HRESULT  WINAPI  fToastRegisterCallback(H_CALLBACK *hCallback, const GUID *pGUID_INotificationActivationCallback, FN_TOAST_NOTIFY fnToastNotify, INT_PTR arg);
VOID     WINAPI  fToastUnRegisterCallback(H_CALLBACK hCallback);
PCWCH    WINAPI  Guid2Wtext(const GUID *guid, BOOL bBraces);
BOOL     WINAPI  fToastRegisterApp(WCHAR  *wszAppPath, WCHAR  *wszAppid, WCHAR  *wszDescription, WCHAR  *IconBackgroundColor, const GUID *pGUID_INotificationActivationCallback);
HRESULT  WINAPI  fToastCreateNotifyManager(H_TOAST *hToast, WCHAR *app_id, const GUID *GuidNotificationActivationCallback);
VOID     WINAPI  fToastDeleteNotifyManager(H_TOAST hToast);
BOOL     WINAPI  fToastSendNotification(H_TOAST hToast, LPCWSTR wszTemplate);

#endif	//ToastLib_h_
