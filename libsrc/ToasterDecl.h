/*+@@file@@----------------------------------------------------------------*//*!
 \file		ToasterDecl.h
 \par Description 
    This file contains all definitions necesary to use the sample.
    It includes interfaces and types.
    Not all interfaces are fully defined, in case of extensions of 
    the the code they could require complete definition.
 \par Project: 
            ToastLib
 \date		Created  on Thu Nov  9 11:58:51 2023
 \date		Modified on Thu Nov  9 11:58:51 2023
 \author	frankie
\*//*-@@file@@----------------------------------------------------------------*/
#ifndef ToasterDecl_h_
#define ToasterDecl_h_
#if __POCC__ >= 500
#pragma 500
#endif

/*
 * IIDs of external interfaces used.
 */
#pragma region
extern __declspec(selectany) const GUID IID_INotificationActivationCallback  = {0x53E31837, 0x6600, 0x4A81, 0x93, 0x95, 0x75, 0xCF, 0xFE, 0x74, 0x6F, 0x94};
extern __declspec(selectany) const GUID IID_IToastNotificationManagerStatics = {0x50ac103f, 0xd235, 0x4598, 0xbb, 0xef, 0x98, 0xfe, 0x4d, 0x1a, 0x3a, 0xd4};
extern __declspec(selectany) const GUID IID_IToastNotificationFactory        = {0x04124b20, 0x82c6, 0x4229, 0xb1, 0x09, 0xfd, 0x9e, 0xd4, 0x66, 0x2b, 0x53};
extern __declspec(selectany) const GUID IID_IXmlDocument                     = {0xf7f3a506, 0x1e87, 0x42d6, 0xbc, 0xfb, 0xb8, 0xc8, 0x09, 0xfa, 0x54, 0x94};
extern __declspec(selectany) const GUID IID_IXmlDocumentIO                   = {0x6cd0e74e, 0xee65, 0x4489, 0x9e, 0xbf, 0xca, 0x43, 0xe8, 0x7b, 0xa6, 0x37};
#pragma endregion

#pragma region winstring.h
/*
 * String declarations
 */
typedef struct HSTRING__
{
	int unused;
} HSTRING__;
typedef HSTRING__ *HSTRING;
typedef struct HSTRING_HEADER
{
	union
	{
		PVOID Reserved1;
		#if defined(_WIN64)
			char Reserved2[24];
		#else
			char Reserved2[20];
		#endif
	} Reserved;
} HSTRING_HEADER;
STDAPI WindowsCreateStringReference(PCWSTR sourceString, UINT32 length, HSTRING_HEADER *hstringHeader, HSTRING *string);
STDAPI WindowsDeleteString(HSTRING string);
#pragma endregion

#pragma region inspectable.h
typedef interface IInspectable IInspectable;
typedef IInspectable *LPINSPECTABLE;
typedef enum TrustLevel
{
    BaseTrust	 = 0,
    PartialTrust = ( BaseTrust    + 1 ) ,
    FullTrust	 = ( PartialTrust + 1 ) 
} 	TrustLevel;
extern const IID IID_IInspectable;
typedef struct IInspectableVtbl
{
    BEGIN_INTERFACE
        HRESULT (STDMETHODCALLTYPE *QueryInterface)(IInspectable * This, REFIID riid, void **ppvObject);
        ULONG   (STDMETHODCALLTYPE *AddRef )(IInspectable * This);
        ULONG   (STDMETHODCALLTYPE *Release )(IInspectable * This);
        HRESULT (STDMETHODCALLTYPE *GetIids )(IInspectable * This, ULONG *iidCount, IID **iids);
        HRESULT (STDMETHODCALLTYPE *GetRuntimeClassName )(IInspectable * This, HSTRING *className);
        HRESULT (STDMETHODCALLTYPE *GetTrustLevel )(IInspectable * This, TrustLevel *trustLevel);
    END_INTERFACE
} IInspectableVtbl;
interface IInspectable
{
    CONST_VTBL struct IInspectableVtbl *lpVtbl;
};
#pragma endregion

#pragma region EventToken.h
typedef struct EventRegistrationToken
{
	__int64 value;
} EventRegistrationToken;
#pragma endregion

#pragma region Incomplete definitions.
/*
 * Incomplete definitions.
 * These works here because we just use reference to the objects.
 * If you want use some of these interfaces you must define the whole
 * object and related referenced objects.
 */
typedef interface __FIReference_1_Windows__CFoundation__CDateTime __FIReference_1_Windows__CFoundation__CDateTime;

typedef interface __x_ABI_CWindows_CUI_CNotifications_CIToastNotifier __x_ABI_CWindows_CUI_CNotifications_CIToastNotifier;
typedef interface __x_ABI_CWindows_CUI_CNotifications_CToastTemplateType __x_ABI_CWindows_CUI_CNotifications_CToastTemplateType;
typedef interface __x_ABI_CWindows_CUI_CNotifications_CIToastNotificationFactory __x_ABI_CWindows_CUI_CNotifications_CIToastNotificationFactory;

typedef interface __x_ABI_CWindows_CStorage_CIStorageFile __x_ABI_CWindows_CStorage_CIStorageFile;

typedef interface __x_ABI_CWindows_CFoundation_CIAsyncAction __x_ABI_CWindows_CFoundation_CIAsyncAction;

typedef interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlLoadSettings __x_ABI_CWindows_CData_CXml_CDom_CIXmlLoadSettings;
typedef interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentType __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentType;
typedef interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlDomImplementation __x_ABI_CWindows_CData_CXml_CDom_CIXmlDomImplementation;
typedef interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlElement __x_ABI_CWindows_CData_CXml_CDom_CIXmlElement;
typedef interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentFragment __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentFragment;
typedef interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlText __x_ABI_CWindows_CData_CXml_CDom_CIXmlText;
typedef interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlComment __x_ABI_CWindows_CData_CXml_CDom_CIXmlComment;
typedef interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlProcessingInstruction __x_ABI_CWindows_CData_CXml_CDom_CIXmlProcessingInstruction;
typedef interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlAttribute __x_ABI_CWindows_CData_CXml_CDom_CIXmlAttribute;
typedef interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlEntityReference __x_ABI_CWindows_CData_CXml_CDom_CIXmlEntityReference;
typedef interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlNodeList __x_ABI_CWindows_CData_CXml_CDom_CIXmlNodeList;
typedef interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlCDataSection __x_ABI_CWindows_CData_CXml_CDom_CIXmlCDataSection;
typedef interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlNode __x_ABI_CWindows_CData_CXml_CDom_CIXmlNode;
typedef interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlNode __x_ABI_CWindows_CData_CXml_CDom_CIXmlNode;

typedef interface __x_ABI_CWindows_CUI_CNotifications_CNotificationSetting __x_ABI_CWindows_CUI_CNotifications_CNotificationSetting;
typedef interface __x_ABI_CWindows_CUI_CNotifications_CIScheduledToastNotification __x_ABI_CWindows_CUI_CNotifications_CIScheduledToastNotification;

typedef interface __FITypedEventHandler_2_Windows__CUI__CNotifications__CToastNotification_Windows__CUI__CNotifications__CToastDismissedEventArgs __FITypedEventHandler_2_Windows__CUI__CNotifications__CToastNotification_Windows__CUI__CNotifications__CToastDismissedEventArgs;
typedef interface __FITypedEventHandler_2_Windows__CUI__CNotifications__CToastNotification_IInspectable __FITypedEventHandler_2_Windows__CUI__CNotifications__CToastNotification_IInspectable;
typedef interface __FITypedEventHandler_2_Windows__CUI__CNotifications__CToastNotification_Windows__CUI__CNotifications__CToastFailedEventArgs __FITypedEventHandler_2_Windows__CUI__CNotifications__CToastNotification_Windows__CUI__CNotifications__CToastFailedEventArgs;

typedef interface __FIVectorView_1_Windows__CUI__CNotifications__CScheduledToastNotification __FIVectorView_1_Windows__CUI__CNotifications__CScheduledToastNotification;
#pragma endregion

/*
 * Xml document interfaces
 */
#pragma region Xml document interfaces
typedef interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument;	//IXmlDocument
typedef struct __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentVtbl
{
	BEGIN_INTERFACE
		HRESULT(STDMETHODCALLTYPE *QueryInterface)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, REFIID riid, void * *ppvObject);
		ULONG  (STDMETHODCALLTYPE *AddRef)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This);
		ULONG  (STDMETHODCALLTYPE *Release)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This);
		HRESULT(STDMETHODCALLTYPE *GetIids)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, ULONG *iidCount, IID * *iids);
		HRESULT(STDMETHODCALLTYPE *GetRuntimeClassName)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, HSTRING *className);
		HRESULT(STDMETHODCALLTYPE *GetTrustLevel)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, TrustLevel *trustLevel);
		HRESULT(STDMETHODCALLTYPE *get_Doctype)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentType * *value);
		HRESULT(STDMETHODCALLTYPE *get_Implementation)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, __x_ABI_CWindows_CData_CXml_CDom_CIXmlDomImplementation * *value);
		HRESULT(STDMETHODCALLTYPE *get_DocumentElement)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, __x_ABI_CWindows_CData_CXml_CDom_CIXmlElement * *value);
		HRESULT(STDMETHODCALLTYPE *CreateElement)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, HSTRING tagName, __x_ABI_CWindows_CData_CXml_CDom_CIXmlElement * *newElement);
		HRESULT(STDMETHODCALLTYPE *CreateDocumentFragment)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentFragment * *newDocumentFragment);
		HRESULT(STDMETHODCALLTYPE *CreateTextNode)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, HSTRING data, __x_ABI_CWindows_CData_CXml_CDom_CIXmlText * *newTextNode);
		HRESULT(STDMETHODCALLTYPE *CreateComment)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, HSTRING data, __x_ABI_CWindows_CData_CXml_CDom_CIXmlComment * *newComment);
		HRESULT(STDMETHODCALLTYPE *CreateProcessingInstruction)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, HSTRING target, HSTRING data, __x_ABI_CWindows_CData_CXml_CDom_CIXmlProcessingInstruction * *newProcessingInstruction);
		HRESULT(STDMETHODCALLTYPE *CreateAttribute)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, HSTRING name, __x_ABI_CWindows_CData_CXml_CDom_CIXmlAttribute * *newAttribute);
		HRESULT(STDMETHODCALLTYPE *CreateEntityReference)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, HSTRING name, __x_ABI_CWindows_CData_CXml_CDom_CIXmlEntityReference * *newEntityReference);
		HRESULT(STDMETHODCALLTYPE *GetElementsByTagName)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, HSTRING tagName, __x_ABI_CWindows_CData_CXml_CDom_CIXmlNodeList * *elements);
		HRESULT(STDMETHODCALLTYPE *CreateCDataSection)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, HSTRING data, __x_ABI_CWindows_CData_CXml_CDom_CIXmlCDataSection * *newCDataSection);
		HRESULT(STDMETHODCALLTYPE *get_DocumentUri)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, HSTRING *value);
		HRESULT(STDMETHODCALLTYPE *CreateAttributeNS)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, IInspectable *namespaceUri, HSTRING qualifiedName, __x_ABI_CWindows_CData_CXml_CDom_CIXmlAttribute * *newAttribute);
		HRESULT(STDMETHODCALLTYPE *CreateElementNS)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, IInspectable *namespaceUri, HSTRING qualifiedName, __x_ABI_CWindows_CData_CXml_CDom_CIXmlElement * *newElement);
		HRESULT(STDMETHODCALLTYPE *GetElementById)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, HSTRING elementId, __x_ABI_CWindows_CData_CXml_CDom_CIXmlElement * *element);
		HRESULT(STDMETHODCALLTYPE *ImportNode)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * This, __x_ABI_CWindows_CData_CXml_CDom_CIXmlNode *node, boolean deep, __x_ABI_CWindows_CData_CXml_CDom_CIXmlNode * *newNode);
	END_INTERFACE
} __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentVtbl;

interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument
{
	CONST_VTBL struct __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentVtbl *lpVtbl;
};

typedef interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentIO __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentIO;
typedef struct __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentIOVtbl
{
	BEGIN_INTERFACE
		HRESULT(STDMETHODCALLTYPE *QueryInterface)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentIO * This, REFIID riid, void **ppvObject);
		ULONG  (STDMETHODCALLTYPE *AddRef)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentIO * This);
		ULONG  (STDMETHODCALLTYPE *Release)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentIO * This);
		HRESULT(STDMETHODCALLTYPE *GetIids)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentIO * This, ULONG *iidCount, IID **iids);
		HRESULT(STDMETHODCALLTYPE *GetRuntimeClassName)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentIO * This, HSTRING *className);
		HRESULT(STDMETHODCALLTYPE *GetTrustLevel)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentIO * This, TrustLevel *trustLevel);
		HRESULT(STDMETHODCALLTYPE *LoadXml)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentIO * This, HSTRING xml);
		HRESULT(STDMETHODCALLTYPE *LoadXmlWithSettings)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentIO * This, HSTRING xml, __x_ABI_CWindows_CData_CXml_CDom_CIXmlLoadSettings *loadSettings);
		HRESULT(STDMETHODCALLTYPE *SaveToFileAsync)(__x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentIO * This, __x_ABI_CWindows_CStorage_CIStorageFile *file, __x_ABI_CWindows_CFoundation_CIAsyncAction **asyncInfo);
	END_INTERFACE
} __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentIOVtbl;

interface __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentIO
{
	CONST_VTBL struct __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocumentIOVtbl *lpVtbl;
};
#pragma endregion


/*
 * Notifications interfaces
 */
#pragma region Notifications interfaces
extern __declspec(selectany) const WCHAR RuntimeClass_Windows_UI_Notifications_ToastNotificationManager[] = L"Windows.UI.Notifications.ToastNotificationManager";
extern __declspec(selectany) const WCHAR RuntimeClass_Windows_UI_Notifications_ToastNotification[] = L"Windows.UI.Notifications.ToastNotification";
extern __declspec(selectany) const WCHAR RuntimeClass_Windows_Data_Xml_Dom_XmlDocument[] = L"Windows.Data.Xml.Dom.XmlDocument";

typedef interface __x_ABI_CWindows_CUI_CNotifications_CIToastNotification __x_ABI_CWindows_CUI_CNotifications_CIToastNotification;
typedef struct __x_ABI_CWindows_CUI_CNotifications_CIToastNotificationVtbl
{
	BEGIN_INTERFACE
		HRESULT(STDMETHODCALLTYPE *QueryInterface)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotification * This, REFIID riid, void * *ppvObject);
		ULONG  (STDMETHODCALLTYPE *AddRef)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotification * This);
		ULONG  (STDMETHODCALLTYPE *Release)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotification * This);
		HRESULT(STDMETHODCALLTYPE *GetIids)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotification * This, ULONG *iidCount, IID * *iids);
		HRESULT(STDMETHODCALLTYPE *GetRuntimeClassName)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotification * This, HSTRING *className);
		HRESULT(STDMETHODCALLTYPE *GetTrustLevel)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotification * This, TrustLevel *trustLevel);
		HRESULT(STDMETHODCALLTYPE *get_Content)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotification * This, __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * *value);
		HRESULT(STDMETHODCALLTYPE *put_ExpirationTime)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotification * This, __FIReference_1_Windows__CFoundation__CDateTime *value);
		HRESULT(STDMETHODCALLTYPE *get_ExpirationTime)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotification * This, __FIReference_1_Windows__CFoundation__CDateTime * *value);
		HRESULT(STDMETHODCALLTYPE *add_Dismissed)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotification * This, __FITypedEventHandler_2_Windows__CUI__CNotifications__CToastNotification_Windows__CUI__CNotifications__CToastDismissedEventArgs *handler, EventRegistrationToken *cookie);
		HRESULT(STDMETHODCALLTYPE *remove_Dismissed)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotification * This, EventRegistrationToken cookie);
		HRESULT(STDMETHODCALLTYPE *add_Activated)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotification * This, __FITypedEventHandler_2_Windows__CUI__CNotifications__CToastNotification_IInspectable *handler, EventRegistrationToken *cookie);
		HRESULT(STDMETHODCALLTYPE *remove_Activated)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotification * This, EventRegistrationToken cookie);
		HRESULT(STDMETHODCALLTYPE *add_Failed)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotification * This, __FITypedEventHandler_2_Windows__CUI__CNotifications__CToastNotification_Windows__CUI__CNotifications__CToastFailedEventArgs *handler, EventRegistrationToken *token);
		HRESULT(STDMETHODCALLTYPE *remove_Failed)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotification * This, EventRegistrationToken token);
	END_INTERFACE
} __x_ABI_CWindows_CUI_CNotifications_CIToastNotificationVtbl;

interface __x_ABI_CWindows_CUI_CNotifications_CIToastNotification
{
	CONST_VTBL struct __x_ABI_CWindows_CUI_CNotifications_CIToastNotificationVtbl *lpVtbl;
};

typedef interface __x_ABI_CWindows_CUI_CNotifications_CIToastNotifier __x_ABI_CWindows_CUI_CNotifications_CIToastNotifier;
typedef struct __x_ABI_CWindows_CUI_CNotifications_CIToastNotifierVtbl
{
	BEGIN_INTERFACE
		HRESULT(STDMETHODCALLTYPE *QueryInterface)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotifier * This, REFIID riid, void * *ppvObject);
		ULONG (STDMETHODCALLTYPE *AddRef)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotifier * This);
		ULONG (STDMETHODCALLTYPE *Release)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotifier * This);
		HRESULT(STDMETHODCALLTYPE *GetIids)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotifier * This, ULONG *iidCount, IID * *iids);
		HRESULT(STDMETHODCALLTYPE *GetRuntimeClassName)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotifier * This, HSTRING *className);
		HRESULT(STDMETHODCALLTYPE *GetTrustLevel)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotifier * This, TrustLevel *trustLevel);
		HRESULT(STDMETHODCALLTYPE *Show)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotifier * This, __x_ABI_CWindows_CUI_CNotifications_CIToastNotification *notification);
		HRESULT(STDMETHODCALLTYPE *Hide)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotifier * This, __x_ABI_CWindows_CUI_CNotifications_CIToastNotification *notification);
		HRESULT(STDMETHODCALLTYPE *get_Setting)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotifier * This, __x_ABI_CWindows_CUI_CNotifications_CNotificationSetting *value);
		HRESULT(STDMETHODCALLTYPE *AddToSchedule)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotifier * This, __x_ABI_CWindows_CUI_CNotifications_CIScheduledToastNotification *scheduledToast);
		HRESULT(STDMETHODCALLTYPE *RemoveFromSchedule)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotifier * This, __x_ABI_CWindows_CUI_CNotifications_CIScheduledToastNotification *scheduledToast);
		HRESULT(STDMETHODCALLTYPE *GetScheduledToastNotifications)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotifier * This, __FIVectorView_1_Windows__CUI__CNotifications__CScheduledToastNotification * *scheduledToasts);
	END_INTERFACE
} __x_ABI_CWindows_CUI_CNotifications_CIToastNotifierVtbl;

interface __x_ABI_CWindows_CUI_CNotifications_CIToastNotifier
{
	CONST_VTBL struct __x_ABI_CWindows_CUI_CNotifications_CIToastNotifierVtbl *lpVtbl;
};

typedef interface __x_ABI_CWindows_CUI_CNotifications_CIToastNotificationFactory __x_ABI_CWindows_CUI_CNotifications_CIToastNotificationFactory;
typedef struct __x_ABI_CWindows_CUI_CNotifications_CIToastNotificationFactoryVtbl
{
	BEGIN_INTERFACE
		HRESULT(STDMETHODCALLTYPE *QueryInterface)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationFactory * This, REFIID riid, void * *ppvObject);
		ULONG  (STDMETHODCALLTYPE *AddRef)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationFactory * This);
		ULONG  (STDMETHODCALLTYPE *Release)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationFactory * This);
		HRESULT(STDMETHODCALLTYPE *GetIids)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationFactory * This, ULONG *iidCount, IID * *iids);
		HRESULT(STDMETHODCALLTYPE *GetRuntimeClassName)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationFactory * This, HSTRING *className);
		HRESULT(STDMETHODCALLTYPE *GetTrustLevel)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationFactory * This, TrustLevel *trustLevel);
		HRESULT(STDMETHODCALLTYPE *CreateToastNotification)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationFactory * This, __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument *content, __x_ABI_CWindows_CUI_CNotifications_CIToastNotification * *notification);
	END_INTERFACE
} __x_ABI_CWindows_CUI_CNotifications_CIToastNotificationFactoryVtbl;

interface __x_ABI_CWindows_CUI_CNotifications_CIToastNotificationFactory
{
	CONST_VTBL struct __x_ABI_CWindows_CUI_CNotifications_CIToastNotificationFactoryVtbl *lpVtbl;
};

typedef interface __x_ABI_CWindows_CUI_CNotifications_CIToastNotificationManagerStatics __x_ABI_CWindows_CUI_CNotifications_CIToastNotificationManagerStatics;
typedef struct __x_ABI_CWindows_CUI_CNotifications_CIToastNotificationManagerStaticsVtbl
{
	BEGIN_INTERFACE
		HRESULT(STDMETHODCALLTYPE *QueryInterface)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationManagerStatics * This, REFIID riid, void * *ppvObject);
		ULONG  (STDMETHODCALLTYPE *AddRef)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationManagerStatics * This);
		ULONG  (STDMETHODCALLTYPE *Release)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationManagerStatics * This);
		HRESULT(STDMETHODCALLTYPE *GetIids)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationManagerStatics * This, ULONG *iidCount, IID * *iids);
		HRESULT(STDMETHODCALLTYPE *GetRuntimeClassName)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationManagerStatics * This, HSTRING *className);
		HRESULT(STDMETHODCALLTYPE *GetTrustLevel)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationManagerStatics * This, TrustLevel *trustLevel);
		HRESULT(STDMETHODCALLTYPE *CreateToastNotifier)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationManagerStatics * This, __x_ABI_CWindows_CUI_CNotifications_CIToastNotifier * *notifier);
		HRESULT(STDMETHODCALLTYPE *CreateToastNotifierWithId)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationManagerStatics * This, HSTRING applicationId, __x_ABI_CWindows_CUI_CNotifications_CIToastNotifier * *notifier);
		HRESULT(STDMETHODCALLTYPE *GetTemplateContent)(__x_ABI_CWindows_CUI_CNotifications_CIToastNotificationManagerStatics * This, __x_ABI_CWindows_CUI_CNotifications_CToastTemplateType type, __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument * *content);
	END_INTERFACE
} __x_ABI_CWindows_CUI_CNotifications_CIToastNotificationManagerStaticsVtbl;

interface __x_ABI_CWindows_CUI_CNotifications_CIToastNotificationManagerStatics
{
	CONST_VTBL struct __x_ABI_CWindows_CUI_CNotifications_CIToastNotificationManagerStaticsVtbl *lpVtbl;
};
#pragma endregion

#pragma region INotificationActivationCallback
typedef interface INotificationActivationCallback INotificationActivationCallback;
#ifndef NOTIFICATION_USER_INPUT_DATA_DEFINED_
#define NOTIFICATION_USER_INPUT_DATA_DEFINED_
typedef struct NOTIFICATION_USER_INPUT_DATA
{
	LPCWSTR Key;
	LPCWSTR Value;
} NOTIFICATION_USER_INPUT_DATA;
#endif
EXTERN_C const IID IID_INotificationActivationCallback;
typedef struct INotificationActivationCallbackVtbl
{
    BEGIN_INTERFACE
        HRESULT (STDMETHODCALLTYPE *QueryInterface)(INotificationActivationCallback * This, REFIID riid, void **ppvObject);
        ULONG   (STDMETHODCALLTYPE *AddRef)(INotificationActivationCallback * This);
        ULONG   (STDMETHODCALLTYPE *Release)(INotificationActivationCallback * This);
        HRESULT (STDMETHODCALLTYPE *Activate)(INotificationActivationCallback * This, LPCWSTR appUserModelId, LPCWSTR invokedArgs, const NOTIFICATION_USER_INPUT_DATA *data, ULONG count);
    END_INTERFACE
} INotificationActivationCallbackVtbl;
interface INotificationActivationCallback
{
    CONST_VTBL struct INotificationActivationCallbackVtbl *lpVtbl;
};
#pragma endregion

/*
 * ROAPI definitions
 */
#pragma region roapi.h
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
ROAPI HRESULT WINAPI RoGetActivationFactory(HSTRING activatableClassId, REFIID iid, void ** factory);
ROAPI HRESULT WINAPI RoActivateInstance(HSTRING activatableClassId, IInspectable ** instance);
ROAPI void WINAPI RoUninitialize(void);
#pragma endregion
#endif

#endif
