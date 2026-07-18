#pragma once

/* ZHE Vision — native OSD resolution for the Broadcom e2egl path.
 *
 * Read once from /etc/enigma2/zhe-osd.conf (key=value: width, height). Default is
 * native WQHD 2560x1440. If the NXPL/Nexus surface allocation fails at WQHD on a
 * given box, the resolution can be lowered on the box (e.g. width=1920 height=1080)
 * and the box rebooted — no rebuild — which is the safe runtime fallback for the
 * one hardware-dependent step (native 2560x1440 Nexus surface).
 */
namespace zhe_osd
{
	extern int width;
	extern int height;
	void load(const char *path = "/etc/enigma2/zhe-osd.conf");
}
