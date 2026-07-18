#include <lib/gdi/egl/zhe_osd_config.h>
#include <lib/base/eerror.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

namespace zhe_osd
{
	int width = 2560;
	int height = 1440;

	void load(const char *path)
	{
		FILE *f = fopen(path, "r");
		if (!f)
		{
			eDebug("[zhe_osd] %s not found; native WQHD default %dx%d", path, width, height);
			return;
		}
		char line[128];
		while (fgets(line, sizeof(line), f))
		{
			if (line[0] == '#')
				continue;
			char *eq = strchr(line, '=');
			if (!eq)
				continue;
			*eq = 0;
			int v = atoi(eq + 1);
			if (v <= 0)
				continue;
			if (!strncmp(line, "width", 5))
				width = v;
			else if (!strncmp(line, "height", 6))
				height = v;
		}
		fclose(f);
		eDebug("[zhe_osd] OSD resolution %dx%d (from %s)", width, height, path);
	}
}
