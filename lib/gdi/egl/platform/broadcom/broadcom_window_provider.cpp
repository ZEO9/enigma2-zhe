#include <lib/base/eerror.h>
#include <lib/gdi/egl/platform/broadcom/broadcom_window_provider.h>

#include <cstring>
#include <cstdio>

/* Broadcom refsw / NXPL headers (from the Vu+ SDK: broadcom-refsw + the
   vuplus-libgles -dev package that ships libnxpl.so). Kept behind the platform
   guard so a non-Broadcom build never sees them. */
extern "C" {
#include <refsw/nxclient.h>
#include <refsw/default_nexus.h>
#include <nxpl.h>
}

BroadcomWindowProvider::BroadcomWindowProvider(int width, int height)
	: m_nxpl_handle(nullptr), m_native_window(0),
	  m_width(width), m_height(height), m_joined(false), m_registered(false)
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

	/* 1. Join the Nexus graphics server as a client (enigma2 runs multi-process
	      against the nexus server; NxClient is the standard client entry). */
	NxClient_JoinSettings joinSettings;
	NxClient_GetDefaultJoinSettings(&joinSettings);
	snprintf(joinSettings.name, sizeof(joinSettings.name), "enigma2-zhe");
	if (NxClient_Join(&joinSettings) != NEXUS_SUCCESS)
	{
		eWarning("[BroadcomWindowProvider] NxClient_Join failed -> no GLES OSD");
		return false;
	}
	m_joined = true;

	/* 2. Register the NXPL display platform (NULL display => default). */
	NXPL_RegisterNexusDisplayPlatform(&m_nxpl_handle, NULL);
	m_registered = true;

	/* 3. Create the native window at the requested OSD resolution. Using the EXT
	      info struct so we can set position/size/zorder explicitly. The graphics
	      feeder hardware-scales this surface to the HDMI output, so 2560x1440 is
	      valid even though fb0 caps at 1920 wide. */
	NXPL_NativeWindowInfoEXT windowInfo;
	memset(&windowInfo, 0, sizeof(windowInfo));
	NXPL_GetDefaultNativeWindowInfoEXT(&windowInfo);
	windowInfo.width = m_width;
	windowInfo.height = m_height;
	windowInfo.x = 0;
	windowInfo.y = 0;
	windowInfo.stretch = true;   /* let the display composition scale to output  */
	windowInfo.zOrder = 0;       /* OSD graphics plane                            */

	m_native_window = (EGLNativeWindowType)NXPL_CreateNativeWindowEXT(&windowInfo);
	if (!m_native_window)
	{
		eWarning("[BroadcomWindowProvider] NXPL_CreateNativeWindowEXT(%dx%d) failed",
			m_width, m_height);
		return false;
	}

	eDebug("[BroadcomWindowProvider] NXPL native window %dx%d created", m_width, m_height);
	return true;
}

EGLNativeDisplayType BroadcomWindowProvider::getNativeDisplay()
{
	/* NXPL resolves the Nexus display from the registered platform. */
	return EGL_DEFAULT_DISPLAY;
}

EGLNativeWindowType BroadcomWindowProvider::getNativeWindow()
{
	return m_native_window;
}

void BroadcomWindowProvider::cleanup()
{
	if (m_native_window)
	{
		NXPL_DestroyNativeWindow(m_native_window);
		m_native_window = 0;
	}
	if (m_registered)
	{
		NXPL_UnregisterNexusDisplayPlatform(&m_nxpl_handle);
		m_registered = false;
		m_nxpl_handle = nullptr;
	}
	if (m_joined)
	{
		NxClient_Uninit();
		m_joined = false;
	}
}
