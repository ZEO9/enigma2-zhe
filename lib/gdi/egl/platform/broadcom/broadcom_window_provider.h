#pragma once

#include <EGL/egl.h>
#include <lib/gdi/egl/inative_window_provider.h>

/* ZHE Vision — Broadcom/Nexus native-window provider for the e2egl backend.
 *
 * Implements INativeWindowProvider on the Vu+/Broadcom NXPL stack (libnxpl.so):
 * joins the Nexus graphics server (NxClient), registers the NXPL display
 * platform and creates a native EGL window at the requested OSD resolution
 * (e.g. 2560x1440 native WQHD). The window is presented by the BCM7444S graphics
 * feeder, which hardware-scales it to the HDMI output — so the OSD is authored at
 * true WQHD independent of the fb0 mode list (which caps at 1920 wide).
 *
 * Verified against the exact NXPL symbols exported by /usr/lib/libnxpl.so on the
 * target (NXPL_RegisterNexusDisplayPlatform, NXPL_GetDefaultNativeWindowInfoEXT,
 * NXPL_CreateNativeWindowEXT, NXPL_DestroyNativeWindow, NXPL_UnregisterNexus...).
 * Headers come from the Vu+ SDK (broadcom-refsw / vuplus-libgles -dev) at build
 * time; a couple of NXPL_NativeWindowInfoEXT field names are refsw-version
 * dependent and confirmed on the first cross-build.
 */
class BroadcomWindowProvider : public INativeWindowProvider
{
private:
	void *m_nxpl_handle;             /* NXPL_PlatformHandle */
	EGLNativeWindowType m_native_window;
	int m_width;
	int m_height;
	bool m_joined;                   /* NxClient_Join succeeded */
	bool m_registered;               /* NXPL platform registered */

public:
	BroadcomWindowProvider(int width = 2560, int height = 1440);
	virtual ~BroadcomWindowProvider();

	// INativeWindowProvider
	bool init(int width, int height) override;
	EGLNativeDisplayType getNativeDisplay() override;
	EGLNativeWindowType getNativeWindow() override;
	void cleanup() override;
};
