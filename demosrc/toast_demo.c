/*+@@file@@----------------------------------------------------------------*//*!
 \file		toast_demo.c
 \par Description 
            Toast notification library based on former work of 
            Valentin-Gabriel Radu
 \par Project: 
            ToastLib
 \date		Created  on Sun Nov 19 23:54:05 2023
 \date		Modified on Sun Nov 19 23:54:05 2023
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
#include "include/ToastLib.h"
#ifdef __POCC__
#ifdef _WIN64
#pragma comment(lib, "WRlibs/x64/runtimeobject.lib")
#pragma comment(lib, "bin/x64/ToastLib.lib")
#else
#pragma comment(lib, "WRlibs/x32/runtimeobject.lib")
#pragma comment(lib, "bin/x32/ToastLib.lib")
#endif
#else
#pragma comment(lib, "runtimeobject.lib")
#pragma comment(lib, "ToastLib.lib")
#endif
#pragma comment(lib, "ComCtl32.lib")

DWORD dwMainThreadId = 0;

/*
 * AUMID (Application User Model ID) for current app.
 */
#define APP_AUMID L"ToastActivatorPlainC"
/*
 * To distinguish between user invocation and Toast notify invocation we add a parameter to command line.
 */
#define TOAST_ACTIVATED_LAUNCH_ARG "-ToastActivated"

/*
 * The GUID "0F82E845-CB89-4039-BDBF-67CA33254C76" that we associate with our factory that produces our
 * INotificationActivationCallback interface.
 */
static GUID GUID_Impl_INotificationActivationCallback = {0xf82e845, 0xcb89, 0x4039, 0xbd, 0xbf, 0x67, 0xca, 0x33, 0x25, 0x4c, 0x76};

/*
 * The XML that describes the notification that will be shown.
 * Of course, this can be built at runtime, and more can be done with it, but for this basic example, this will suffice.
 */
const wchar_t wszText[] =
		L"<toast scenario=\"reminder\" "
		L"activationType=\"foreground\" launch=\"action=mainContent\" duration=\"short\">\r\n"
		L"	<visual>\r\n"
		L"		<binding template=\"ToastGeneric\">\r\n"
		L"			<text><![CDATA[This is a demo notification for PellesC]]></text>\r\n"
		L"			<text><![CDATA[It contains 2 lines of text]]></text>\r\n"
		L"			<text placement=\"attribution\"><![CDATA[Created by frankie.\r\nBased on Valentin-Gabriel Radu demo]]></text>\r\n"
		L"		</binding>\r\n"
		L"	</visual>\r\n"
		L"  <actions>\r\n"
		L"	  <input id=\"tbReply\" type=\"text\" placeHolderContent=\"Send a message to the app\"/>\r\n"
		L"	  <action content=\"Send\" activationType=\"foreground\" arguments=\"action=reply\"/>\r\n"
		L"	  <action content=\"Visit forum\" activationType=\"protocol\" arguments=\"https://forum.pellesc.de\"/>\r\n"
		L"	  <action content=\"Close app\" activationType=\"foreground\" arguments=\"action=closeApp\"/>\r\n"
		L"  </actions>\r\n"
		L"	<audio src=\"ms-winsoundevent:Notification.Default\" loop=\"false\" silent=\"false\"/>\r\n"
		L"</toast>\r\n";


/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		MyNotificationCallback
 \date		Created  on Sun Nov 19 12:25:50 2023
 \date		Modified on Sun Nov 19 12:25:50 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static INT_PTR WINAPI fnNotificationCallback(INT_PTR args, LPCWSTR appUserModelId, LPCWSTR invokedArgs, const NOTIFICATION_USER_INPUT_DATA* data, ULONG count)
{
	/*
	 * The user callback function is called from the toast notify callback and receives the same data
	 * passed from framework to the COM callback with another parameter 'args' that holds data passed
	 * by user application in the 'fToastRegisterCallback()' function when registering app for callback.
	 */
	wprintf(L"User argument: \"%ls\"\n", (WCHAR *)args);
	wprintf(L"Interacted with notification from AUMID \"%ls\" with arguments: \"%ls\". User input count: %d.\n", appUserModelId, invokedArgs, count);
	if (!_wcsicmp(invokedArgs, L"action=closeApp"))
	{
		PostThreadMessageW(dwMainThreadId, WM_QUIT, 0, 0);
	}
	else
		if (!_wcsicmp(invokedArgs, L"action=reply"))
		{
			for (unsigned int i = 0; i < count; ++i)
			{
				if (!_wcsicmp(data[i].Key, L"tbReply"))
				{
					wprintf(L"Reply was \"%ls\".\n", data[i].Value);
				}
			}
		}
	return S_OK;
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		GetExePath
 \date		Created  on Mon Nov 20 01:25:32 2023
 \date		Modified on Mon Nov 20 01:25:32 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
static BOOL GetExePath(WCHAR *wszExePath, DWORD size, WCHAR *wszArg)
{
	if (!GetModuleFileNameW(NULL, wszExePath + 1, size - (DWORD)wcslen(wszArg) - 2))
		return FALSE;
	wszExePath[0] = L'"';
	(void)wcscat_s(wszExePath, size, L"\" ");
	(void)wcscat_s(wszExePath, size, wszArg);
	return TRUE;
}

/*+@@fnc@@----------------------------------------------------------------*//*!
 \brief		main
 \date		Created  on Thu Nov  9 14:27:41 2023
 \date		Modified on Thu Nov  9 14:27:41 2023
\*//*-@@fnc@@----------------------------------------------------------------*/
int main(int argc, char** argv)
{
	int ret = 0;
	BOOL bInvokedFromToast = (argc > 1);	//TRUE if any command line parameter

	dwMainThreadId = GetCurrentThreadId();

	/*
	 * Initialize COM and Windows Runtime on this thread.
	 * Make sure that the threading models of the two match.
	 */
	if (!SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
		return 1;

	if (!SUCCEEDED(RoInitialize(RO_INIT_MULTITHREADED)))
		return 1;

	/*
	 * Register our notification callback function
	 */
	H_CALLBACK h_Callback;
	if (S_OK != fToastRegisterCallback(&h_Callback, &GUID_Impl_INotificationActivationCallback, fnNotificationCallback, (INT_PTR)L"PellesC argument"))
	{
		ret = 1;
		goto end;
	}

	if (!bInvokedFromToast)
	{
		/*
		 * In this demo the COM class is registered at runtime.
		 * The user GUID is associated to the class factory providing the INotificationActivationCallback interface.
		 *
		 * The EXE path is used to find the executable that handles notifications and run it to CoRegisterClassObject
		 * the notification class that implements the interface.
		 * Without all this clicking on notifications will do nothing.
		 */
		wchar_t  wszExePath[MAX_PATH + 100] = {0};
		if (!GetExePath(wszExePath, MAX_PATH + 100, _T(TOAST_ACTIVATED_LAUNCH_ARG)))
		{
			ret = 1;
			goto end;
		}
		if (!fToastRegisterApp(wszExePath, APP_AUMID, L"Toast Activator Plain C Example for PellesC", L"FF00FF00", &GUID_Impl_INotificationActivationCallback))
		{
			ret = 1;
			goto end;
		}

		/*
		 * Create a toast notify manager for notifications to show.
		 */
		H_TOAST hToast = NULL;
		if (!SUCCEEDED(fToastCreateNotifyManager(&hToast, APP_AUMID, &GUID_Impl_INotificationActivationCallback)))
		{
			ret = 1;
			goto end;
		}

		/*
		 * Show some sample notifications.
		 */
		for (int i=0; i<7; i++)
		{
			if (!fToastSendNotification(hToast, wszText))
			{
				PostQuitMessage(1);
				break;
			}
			Sleep(250);
		}

		/*
		 * Dispose the toast notify manager.
		 */
		fToastDeleteNotifyManager(hToast);
	}

	wprintf(L"Toast notification Demo invoked by %ls\n", bInvokedFromToast ? L"Toast server" : L"user");

	/*
	 * Start message loop to wait for toast notifications.
	 */
	MSG msg;
	while (GetMessageW(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

end:
	/*
	 * Clean out all data set.
	 */
	fToastUnRegisterCallback(h_Callback);

	/*
	 * De-initialize and exit.
	 */
	RoUninitialize();
	CoUninitialize();

	return ret;
}
