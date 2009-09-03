/*  XMMS - Cross-platform multimedia player
 *  Copyright (C) 1998-2000  Peter Alm, Mikael Alm, Olle Hallnas, Thomas Nilsson and 4Front Technologies
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 *  Wed May 24 10:49:37 CDT 2000
 *  Fixes to threading/context creation for the nVidia X4 drivers by 
 *  Christian Zander <phoenix@minion.de>
 */
#include "glib.h"
#include "s3d.h"
#include "stdio.h"
#include <math.h>

int modE=1;
#define NUM_BANDS 16
float width = 0.1;

float mats[16*16*3*12];
float *mp;
float vectors[16*16*4*3];
float base[16*16*4*3];
float *vp;
float *bp;
int polys[16*16*6*2*4];

int o=-1;

void glColor3f(float r, float g , float b)
{
    *mp=r;
    mp++;
    *mp=g;
    mp++;
    *mp=b;
    mp++;
    *mp=1;
    mp++;
    *mp=r;
    mp++;
    *mp=g;
    mp++;
    *mp=b;
    mp++;
    *mp=1;
    mp++;
    *mp=r;
    mp++;
    *mp=g;
    mp++;
    *mp=b;
    mp++;
    *mp=1;
    mp++;
}


static void gen_mats(void)
{
	mp=mats;	

	gint x,y;
	float x_offset, z_offset, r_base, b_base;
	float red,green,blue;
	for(y = 0; y < 16; y++)
	{
		z_offset = -1.6 + ((15 - y) * 0.2);

		b_base = y * (1.0 / 15);
		r_base = 1.0 - b_base;
			
		for(x = 0; x < 16; x++)
		{
			x_offset = -1.6 + (x * 0.2);			

			red= r_base - (x * (r_base / 15.0));
			green= x * (1.0 / 15);
			blue= b_base;
			glColor3f(red,green,blue);
			glColor3f(0.5 * red, 0.5 * green, 0.5 * blue);
			glColor3f(0.25 * red, 0.25 * green, 0.25 * blue);

		}
	}
}

void drVertex3f(float x, float y, float z)
{
    *vp=x*9;
    vp++;
    *vp=y*9-7;
    vp++;
    *vp=z*9;
    vp++;
}
void baVertex3f(float x, float y, float z)
{
    *bp=x*9;
    bp++;
    *bp=y*9-7;
    bp++;
    *bp=z*9;
    bp++;
}

static float heights[16][16], scale;

static void vert_bar(float x_offset, float z_offset, float y_offset)
{
	drVertex3f(x_offset, y_offset, z_offset);
	drVertex3f(x_offset+width, y_offset, z_offset);
	drVertex3f(x_offset+width, y_offset, z_offset+width);
	drVertex3f(x_offset, y_offset, z_offset+width);
}

static void vert_barbase(float x_offset, float z_offset, float y_offset)
{
	baVertex3f(x_offset, 0, z_offset);
	baVertex3f(x_offset+width, 0, z_offset);
	baVertex3f(x_offset+width, 0, z_offset+width);
	baVertex3f(x_offset, 0, z_offset+width);
}

void makepolys(void)
{
    int * pp;
    pp=polys;
    int i,j,c;
#define nob 16*16*4
    switch (modE)
    {
    case 1:
    case 2:
    case 4:
    for(i=0;i<16*16;i++)
    {
	    j=i*4;
	    c=i*3;
/*top*/
	    *pp=nob+j+0;pp++;
	    *pp=nob+j+1;pp++;
	    *pp=nob+j+2;pp++;
	    *pp=c;pp++;
	    *pp=nob+j+0;pp++;
	    *pp=nob+j+2;pp++;
	    *pp=nob+j+3;pp++;
	    *pp=c;pp++;
/*bottom*/
	    *pp=j+2;pp++;
	    *pp=j+1;pp++;
	    *pp=j+0;pp++;
	    *pp=c;pp++;
	    *pp=j+2;pp++;
	    *pp=j+0;pp++;
	    *pp=j+3;pp++;
	    *pp=c;pp++;

	    *pp=j+0;pp++;
	    *pp=j+1;pp++;
	    *pp=nob+j+1;pp++;
	    *pp=c;pp++;
	    *pp=j+0;pp++;
	    *pp=nob+j+1;pp++;
	    *pp=nob+j+0;pp++;
	    *pp=c;pp++;

	    *pp=j+1;pp++;
	    *pp=j+2;pp++;
	    *pp=nob+j+2;pp++;
	    *pp=c;pp++;
	    *pp=j+1;pp++;
	    *pp=nob+j+2;pp++;
	    *pp=nob+j+1;pp++;
	    *pp=c;pp++;

	    *pp=j+2;pp++;
	    *pp=j+3;pp++;
	    *pp=nob+j+3;pp++;
	    *pp=c;pp++;
	    *pp=j+2;pp++;
	    *pp=nob+j+3;pp++;
	    *pp=nob+j+2;pp++;
	    *pp=c;pp++;

	    *pp=j+3;pp++;
	    *pp=j+0;pp++;
	    *pp=nob+j+0;pp++;
	    *pp=c;pp++;
	    *pp=j+3;pp++;
	    *pp=nob+j+0;pp++;
	    *pp=nob+j+3;pp++;
	    *pp=c;pp++;
    }
    break;
    case 3:
    case 5:
    for(i=0;i<16*16;i++)
    {
	    j=i*8;
	    c=i*3;
/*top*/
	    *pp=j+4;pp++;
	    *pp=j+5;pp++;
	    *pp=j+6;pp++;
	    *pp=c;pp++;
	    *pp=j+4;pp++;
	    *pp=j+6;pp++;
	    *pp=j+7;pp++;
	    *pp=c;pp++;
/*bottom*/
	    *pp=j+2;pp++;
	    *pp=j+1;pp++;
	    *pp=j+0;pp++;
	    *pp=c;pp++;
	    *pp=j+2;pp++;
	    *pp=j+0;pp++;
	    *pp=j+3;pp++;
	    *pp=c;pp++;

	    *pp=j+0;pp++;
	    *pp=j+1;pp++;
	    *pp=j+5;pp++;
	    *pp=c;pp++;
	    *pp=j+0;pp++;
	    *pp=j+5;pp++;
	    *pp=j+4;pp++;
	    *pp=c;pp++;

	    *pp=j+1;pp++;
	    *pp=j+2;pp++;
	    *pp=j+6;pp++;
	    *pp=c;pp++;
	    *pp=j+1;pp++;
	    *pp=j+6;pp++;
	    *pp=j+5;pp++;
	    *pp=c;pp++;

	    *pp=j+2;pp++;
	    *pp=j+3;pp++;
	    *pp=j+7;pp++;
	    *pp=c;pp++;
	    *pp=j+2;pp++;
	    *pp=j+7;pp++;
	    *pp=j+6;pp++;
	    *pp=c;pp++;

	    *pp=j+3;pp++;
	    *pp=j+0;pp++;
	    *pp=j+4;pp++;
	    *pp=c;pp++;
	    *pp=j+3;pp++;
	    *pp=j+4;pp++;
	    *pp=j+7;pp++;
	    *pp=c;pp++;
    }
    break;
    }
}

void bars(void (*baf)(float,float,float))
{
	int x,y;
	float x_offset; float z_offset; float y_offset;
	for(y = 0; y < 16; y++)
	{
		z_offset = -1.6 + ((15 - y) * 0.2);

		float b_base = y * (1.0 / 15);
		float r_base = 1.0 - b_base;
			
		for(x = 0; x < 16; x++)
		{
			x_offset = -1.6 + (x * 0.2);

			baf(x_offset, z_offset, heights[y][x]);
		}
	}
}

static void draw_bars(void)
{
	vp=vectors;
	bars(vert_bar);
	
	if(o==-1)
	{
	    o=s3d_new_object();
	    s3d_flags_on(o, S3D_OF_VISIBLE|S3D_OF_SELECTABLE);
	    makepolys();
	    gen_mats();
	    s3d_push_polygons(o, polys, 16*16*12);
	    s3d_push_materials_a(o,mats,3*16*16);
	    bp=base;
	    bars(vert_barbase);
	    s3d_push_vertices(o,base,4*16*16);
	    
	    s3d_push_vertices(o,vectors,4*16*16);
	}
	else
	    s3d_pep_vertices(o,vectors,4*16*16);
}

static void oglspectrum_gen_heights(gint16 data[2][256])
{
	gint i,c;
	gint y;
	float val;
	static float scale=0;
	if(!scale)
	    scale = 1.0 / log(256.0);
	gint xscale[] = {0, 1, 2, 3, 5, 7, 10, 14, 20, 28, 40, 54, 74, 101, 137, 187, 255};

	for(y = 15; y > 0; y--)
	{
		for(i = 0; i < 16; i++)
		{
			heights[y][i] = heights[y - 1][i];
		}
	}

	for(i = 0; i < NUM_BANDS; i++)
	{
		for(c = xscale[i], y = 0; c < xscale[i + 1]; c++)
		{
			if(data[0][c] > y)
				y = data[0][c];
		}
		y >>= 7;
		if(y > 0)
			val = (log(y) * scale);
		else
			val = 0;

				
		heights[0][i] = val;
	}
}






























int key(struct s3d_evt *e)
{
modE++;
if (modE>5)modE=1;
makepolys();
if(modE==3)
width=0.111;
else
width = modE/10.0;
s3d_pop_polygon(o, 16*16*12);
s3d_push_polygons(o, polys, 16*16*12);
}


void mainloop(void)
{
    gint16 buf[2][256];
    
    FILE *f;
    //printf("opening\n");
    if(f=fopen("/tmp/somefunnyname", "r"))
    {
	//printf("open\n");
	fread(buf,512,2,f);

	fclose(f);
	//printf("closed\n");
	remove("/tmp/somefunnyname");
	oglspectrum_gen_heights(buf);
//printf("vvv\n");
	draw_bars();
    }
   usleep(1000); // sleep is good
   //printf("wake\n");
}

static int stop(struct s3d_evt *event)
{
    s3d_quit();
    return(0);
}


int main(int a, char **v)
{
    if(s3d_init(&a,&v,"test"))
    {
	printf("opsie\n");
	s3d_usage();
	s3d_quit();
    }
    draw_bars();
    s3d_set_callback(S3D_EVENT_QUIT, stop);
    s3d_set_callback(S3D_EVENT_KEY, key);
    s3d_mainloop(mainloop);
    printf("Bye.\n");
}

