noinst_LIBRARIES += gdi/libenigma_gdi.a

gdi_libenigma_gdi_a_CXXFLAGS = -O3 $(LIBSDL_CFLAGS) $(EGL_CFLAGS) $(GLES2_CFLAGS)

gdi_libenigma_gdi_a_SOURCES = \
	gdi/accel.cpp \
	gdi/bcm.cpp \
	gdi/color.cpp \
	gdi/compositing.cpp \
	gdi/drawing.cpp \
	gdi/epng.cpp \
	gdi/erect.cpp \
	gdi/fb.cpp \
	gdi/font.cpp \
	gdi/font_arabic.cpp \
	gdi/gfont.cpp \
	gdi/gfont_atlas.cpp \
	gdi/glcddc.cpp \
	gdi/gmaindc.cpp \
	gdi/gpixmap.cpp \
	gdi/grc.cpp \
	gdi/lcd.cpp \
	gdi/picexif.cpp \
	gdi/picload.cpp \
	gdi/pixmapcache.cpp \
	gdi/region.cpp

gdiincludedir = $(pkgincludedir)/lib/gdi
gdiinclude_HEADERS = \
	gdi/accel.h \
	gdi/color.h \
	gdi/compositing.h \
	gdi/drawing.h \
	gdi/epng.h \
	gdi/epoint.h \
	gdi/erect.h \
	gdi/esize.h \
	gdi/fb.h \
	gdi/font.h \
	gdi/gfont.h \
	gdi/gfont_atlas.h \
	gdi/glcddc.h \
	gdi/gpixmap.h \
	gdi/grc.h \
	gdi/lcd.h \
	gdi/picexif.h \
	gdi/picload.h \
	gdi/pixmapcache.h \
	gdi/region.h


if HAVE_EGL
gdi_libenigma_gdi_a_SOURCES += \
	gdi/egl/egl_config.cpp \
	gdi/egl/egl_init.cpp \
	gdi/egl/shader/gadvanced_shader.cpp \
	gdi/egl/shader/gshader.cpp \
	gdi/egl/shader/gtext_shader.cpp \
	gdi/egl/shader/gtexture_shader.cpp \
	gdi/egl/gegldc.cpp \
	gdi/egl/gles_version.cpp \
	gdi/egl/gtexture_manager.cpp
	gdi/egl/zhe_osd_config.cpp
gdiinclude_HEADERS += \
	gdi/egl/egl_config.h \
	gdi/egl/shader/gadvanced_shader.h \
	gdi/egl/shader/gshader.h \
	gdi/egl/shader/gtext_shader.h \
	gdi/egl/shader/gtexture_shader.h \
	gdi/egl/gegldc.h \
	gdi/egl/gles_version.h \
	gdi/egl/gtexture_manager.h
	gdi/egl/zhe_osd_config.h
endif

if DREAMNEXTGEN
gdi_libenigma_gdi_a_SOURCES += \
	gdi/egl/platform/amlogic/amlogic_window_provider.cpp
gdiinclude_HEADERS += \
	gdi/egl/platform/amlogic/amlogic_window_provider.h
endif

if HAVE_BCM_NXPL
gdi_libenigma_gdi_a_SOURCES += \
	gdi/egl/platform/broadcom/broadcom_window_provider.cpp
gdiinclude_HEADERS += \
	gdi/egl/platform/broadcom/broadcom_window_provider.h
endif


if HAVE_LIBSDL
gdi_libenigma_gdi_a_SOURCES += \
	gdi/sdl.cpp \
	gdi/sdl.h
else
gdi_libenigma_gdi_a_SOURCES += \
	gdi/gfbdc.cpp \
	gdi/gfbdc.h
endif

# ZHE Vision: OpenBH includes the LCD framebuffer driver unconditionally
# (openatv gated this behind LCD_FRAMEBUFFER_MODE, which OpenBH's configure.ac
# does not define). vuduo4kse has a colour LCD, so build it always.
gdi_libenigma_gdi_a_SOURCES += \
	gdi/fblcd.cpp \
	gdi/fblcd.h
