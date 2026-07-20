#pragma once

#include <EGL/egl.h>
#include <lib/gdi/egl/inative_window_provider.h>

/* ZHE Vision — Broadcom/Nexus native-window provider for the e2egl gEGLDC.
 *
 * Uses the Vu+ vendor VUGLES_* helpers ONLY for native-window + platform plumbing
 * (join nexus, register the NXPL display platform, create the NXPL native window at
 * the requested OSD resolution, set its virtual-display/position/visibility). The
 * ACTUAL rendering stays in gEGLDC: its own EGL context + per-widget GLES shaders,
 * texture manager and font atlas. This is NOT the libvugles2 whole-surface
 * gles_set_buffer presenter.
 *
 * The full VUGLES/NXPL ABI was reconstructed by disassembly (see re/NXPL_ABI_MAP.md);
 * the standalone eglCreateWindowSurface bind fails only for lack of process/global
 * platform state, so it is validated inside the fully-initialised enigma2 process.
 */
class BroadcomWindowProvider : public INativeWindowProvider
{
private:
	void *m_native_window;   /* NXPL window from VUGLES_CreateNativeWindow */
	void *m_platform_handle;  /* VUGLES/NXPL display-platform handle        */
	int m_width;
	int m_height;
	bool m_platform_up;

public:
	BroadcomWindowProvider(int width = 2560, int height = 1440);
	virtual ~BroadcomWindowProvider();

	// INativeWindowProvider
	bool init(int width, int height) override;
	EGLNativeDisplayType getNativeDisplay() override;
	EGLNativeWindowType getNativeWindow() override;
	void cleanup() override;
};
