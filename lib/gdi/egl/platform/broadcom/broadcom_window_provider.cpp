#include <lib/base/eerror.h>
#include <lib/gdi/egl/platform/broadcom/broadcom_window_provider.h>

#include <cstring>

/* Vendor VUGLES_* ABI (libvugles2), self-declared per the reconstructed map
   (re/NXPL_ABI_MAP.md) — no vendor headers are shipped. Signatures verified from
   glClass::InitDisplay call sites in libvugles2.so. */
extern "C" {
void  VUGLES_InitPlatformAndDefaultDisplay(const char *name, float *out);
void  VUGLES_GetDefaultNativeWindowInfo(void *info);
void  VUGLES_RegisterDisplayPlatform(void *handle_out);
void *VUGLES_CreateNativeWindow(void *info, int width, int height);
unsigned VUGLES_GetClientID(void);
void  VUGLES_SetVirtualDisplay(int a, int width, int height, int d);
void  VUGLES_SetPosition(int x, int y, int width, int height);
void  VUGLES_SetVisible(int visible);
void  VUGLES_DestroyNativeWindow(void);
void  VUGLES_UnregisterDisplayPlatform(void);
void  VUGLES_TermPlatform(void);
}

BroadcomWindowProvider::BroadcomWindowProvider(int width, int height)
	: m_native_window(0), m_platform_handle(0),
	  m_width(width), m_height(height), m_platform_up(false)
{
}

BroadcomWindowProvider::~BroadcomWindowProvider()
{
	cleanup();
}

bool BroadcomWindowProvider::init(int width, int height)
{
	m_width = width;
	m_height = height;

	/* 1. Join the nexus server + bring up the default display (vendor plumbing). */
	float aspect = 0.0f;
	VUGLES_InitPlatformAndDefaultDisplay("enigma2-zhe", &aspect);
	m_platform_up = true;
	eDebug("[BroadcomWindowProvider] VUGLES platform up (aspect=%f)", aspect);

	/* 2. Default native-window info, then register the NXPL display platform. */
	unsigned char info[256];
	memset(info, 0, sizeof(info));
	VUGLES_GetDefaultNativeWindowInfo(info);
	VUGLES_RegisterDisplayPlatform(&m_platform_handle);
	eDebug("[BroadcomWindowProvider] NXPL display platform registered handle=%p", m_platform_handle);

	/* 3. Create the native window at the requested OSD resolution (e.g. 2560x1440). */
	m_native_window = VUGLES_CreateNativeWindow(info, m_width, m_height);
	if (!m_native_window)
	{
		eWarning("[BroadcomWindowProvider] VUGLES_CreateNativeWindow(%dx%d) failed -> software fallback",
			m_width, m_height);
		return false;
	}

	/* 4. Bind the surface client to a WQHD virtual display + full-window position,
	      and make it visible (the step the standalone probe could not do). */
	unsigned clientID = VUGLES_GetClientID();
	VUGLES_SetVirtualDisplay(0, m_width, m_height, 0);
	VUGLES_SetPosition(0, 0, m_width, m_height);
	VUGLES_SetVisible(1);
	eDebug("[BroadcomWindowProvider] native window %dx%d ready (clientID=%u)", m_width, m_height, clientID);
	return true;
}

EGLNativeDisplayType BroadcomWindowProvider::getNativeDisplay()
{
	/* The Broadcom EGL resolves the display from the registered NXPL platform. */
	return EGL_DEFAULT_DISPLAY;
}

EGLNativeWindowType BroadcomWindowProvider::getNativeWindow()
{
	return (EGLNativeWindowType)m_native_window;
}

void BroadcomWindowProvider::cleanup()
{
	if (m_native_window)
	{
		VUGLES_DestroyNativeWindow();
		m_native_window = 0;
	}
	if (m_platform_handle)
	{
		VUGLES_UnregisterDisplayPlatform();
		m_platform_handle = 0;
	}
	if (m_platform_up)
	{
		VUGLES_TermPlatform();
		m_platform_up = false;
	}
}
