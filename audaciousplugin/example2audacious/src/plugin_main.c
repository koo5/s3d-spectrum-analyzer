#include <audacious/plugin.h>
#include "config.h"



static void freq(gint16 data[2][256])
{
     FILE *f;
     if((f=fopen("/tmp/somefunnyname", "w")))
     {
        fwrite(data,2 , 512, f);
	fclose(f);
     }
}

VisPlugin example2_vp =
{
	.description = "Autotools example 2",
	.num_pcm_chs_wanted = 0,
	.num_freq_chs_wanted = 1,
	.render_freq = freq
};
