#include <lib/base/init.h>
#include <lib/base/init_num.h>
#include <lib/base/eerror.h>
#include <lib/gdi/egl/egl_config.h>
#include <lib/gdi/egl/gegldc.h>

#ifdef DREAMNEXTGEN
#include <lib/gdi/egl/platform/amlogic/amlogic_window_provider.h>
#endif
#ifdef HAVE_BCM_NXPL
#include <lib/gdi/egl/platform/broadcom/broadcom_window_provider.h>
#include <lib/gdi/egl/zhe_osd_config.h>
#endif

class gEGLDCAutoInit : protected eAutoInit
{
	gEGLDC *m_dc;
	void initNow() override
	{
		if (egl_config::disable_egl)
		{
			eDebug("[gEGLDC] EGL disabled via command line");
			return;
		}

		INativeWindowProvider *provider = nullptr;
		int osd_w = 1280, osd_h = 720;

#ifdef DREAMNEXTGEN
		provider = new AmlogicWindowProvider();
		osd_w = 1920; osd_h = 1080;
#elif defined(HAVE_BCM_NXPL)
		/* Vu+/Broadcom native WQHD OSD via NXPL. Resolution is read from
		   /etc/enigma2/zhe-osd.conf (default 2560x1440), so it can be lowered to
		   1920x1080 on the box if the driver refuses a WQHD Nexus surface —
		   without a rebuild. The provider's init(w,h) creates the NXPL window. */
		zhe_osd::load();
		osd_w = zhe_osd::width;
		osd_h = zhe_osd::height;
		{
			BroadcomWindowProvider *bcm = new BroadcomWindowProvider(osd_w, osd_h);
			if (bcm->init(osd_w, osd_h))
				provider = bcm;
			else
			{
				eWarning("[eInit] Broadcom NXPL provider init failed -> software fallback");
				delete bcm;
			}
		}
#else
		// Fallback for other platforms (SDL/Wayland) once implemented
		// For now, if not HWDREAMONE, we don't have a default provider here
		// unless we add SDLWindowProvider or WaylandWindowProvider detection.
#endif

		if (provider)
		{
			eDebug("[eInit] + (%d) gEGLDC %dx%d", rl, osd_w, osd_h);
			m_dc = new gEGLDC(provider, osd_w, osd_h);
			if (!m_dc->initEGL())
			{
				eDebug("[gEGLDC] initEGL failed, falling back...");
				delete m_dc;
				m_dc = nullptr;
			}
		}
	}

	void closeNow() override
	{
		if (m_dc)
		{
			delete m_dc;
			m_dc = nullptr;
		}
	}

public:
	gEGLDCAutoInit()
		: eAutoInit(eAutoInitNumbers::graphic - 2, "gEGLDC"), m_dc(nullptr)
	{
		eInit::add(rl, this);
	}

	~gEGLDCAutoInit()
	{
		eInit::remove(rl, this);
	}
};

static gEGLDCAutoInit init_gEGLDC_custom;
