#include "LOD.h"
#include "MMath.h"
#include "terrain.h"
#include "image.h"

int g_w=0;
void setW(int w)
{
	g_w = w;
}
#define INDEX(x,y)	(y)*g_w+(x)

void CTerrain::SetCamera(CCamera* pcm)
{
     m_camera_info= pcm->GetCameraInfo(); 
}

int CTerrain::LoadMap(char* file_name,float scaleW,float scaleH,float startH)
{
	m_map = ::LoadRPMap(file_name);
	if(m_map._data==NULL)
		return TE_FILE_ERROR;

	if(!Math.IsMagicNumber(m_map._info._height - 1) )
	{
		FreeRPMap(m_map);
		return TE_INVALIDATE_SIZE;
	}

	m_data_array.SetDataInfo(m_map._info._width ,m_map._info._height ,m_map._data);
	m_w = m_map._info._width;
	m_h = m_map._info._height;
	
	m_scale =scaleH;
	setW(m_map._info._width);


    return TE_OK;
}
int CTerrain::LaadFromRAW(char* file_name,float scaleW,float scaleH,float startH)
{
	return 1;
}

int CTerrain::SetTexture(GLuint tex[],int texNumb)
{
	for(int i=0;i<texNumb;i++)
		m_texID[i]=tex[i];
    m_texNum = texNumb;
	return 1;
}

CLOD* CTerrain::GetLODControl()
{
	return m_pLodControl;
}

CLOD* CTerrain::CreateLODCntrl()
{
	m_pLodControl=new CLOD(this);
	return m_pLodControl;
}

float CTerrain::GetHeight(float x,float y)
{
	int xl,xh,yl,yh;
	if (x<0 || x> m_w ||y<0||y>m_h)
		return 0;
	xl = x;
	xh = xl+1;
	yl = y;
	yh = yl+1;
    
	float dx = x - xl ;
	float dy = y - yl ;

	float hll =(    dy  * dx    )  * m_data_array.GetValue(xh,yh);// yh * m_w + xh];   
	float hlh =( (1-dy) * dx    )  * m_data_array.GetValue(xh,yl);// yl * m_w + xh];
    float hhl =(    dy  * (1-dx))  * m_data_array.GetValue(xl,yh);// yh * m_w + xl];
    float hhh =( (1-dy) * (1-dx))  * m_data_array.GetValue(xl,yl);// yl * m_w + xl];

	return (hll+hlh+hhh+hhl) * m_scale;

}
//***************************************************************
//以下是地形的渲染函数，目前该函数为简单的渲染，。
//在以后将加入：
//       LOD, View Frustum Culling,
//       Multi-Texture Maping
//       Water simulate
//***************************************************************
int CTerrain::Render()
{
	
	if(m_pLodControl)
	{
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glScalef(1.0,1.0,m_scale);
		m_pLodControl->Render(m_texID); 
		glPopMatrix();
		return 1;	
	}
	else
	{
		MessageBox(NULL,"No LOD Control created,SimNature will exit!","Falt error",MB_OK);
		exit(0);
	}
	
}

CTerrain::CTerrain()
{
//	m_data = NULL;
	m_map._data = NULL;
	m_pLodControl = NULL;
}

void CTerrain::Strecth(float s)
{
	m_scale *= s;
}

void CTerrain::Smooth(int level)
{	
	float h ;
	for(int i=0;i<level;i++)
	{
	for(int x=0;x<m_w;x++)
		for(int y =0;y<m_w;y++)
		{
		  int lx = x-1;
		  if(lx<0) lx= m_w-1;
		  int ly = y-1;
		  if(ly<0) ly= m_h-1;
		  int ux = x+1;
		  if(ux >= m_w)ux =0;
		  int uy = y+1;
		  if(uy >= m_h)uy =0;

          h  =  4 * m_data_array.GetValue( (lx) , y    );
	      h  += 4 * m_data_array.GetValue( (ux) , y    );
		  h  += 4 * m_data_array.GetValue( x    , (uy) );
          h  += 4 * m_data_array.GetValue( x    , (ly) );
		  h  += 2 * m_data_array.GetValue( (lx) , (lx) );
	      h  += 2 * m_data_array.GetValue( (ux) , (ly) );
		  h  += 2 * m_data_array.GetValue( (lx) , (uy) );
          h  += 2 * m_data_array.GetValue( (ux) , (uy) );
		  h  += 6 * m_data_array.GetValue( x    , y    );
		  h /= 30.0;
		  m_data_array.SetValue(x    , y  , h );
		}
	}

}

int CTerrain::GetRawValue(int x, int y)
{
	return  m_data_array.GetValue(x , y)  *  m_scale;
}

void GetNormal(float* p1,float* p2,float* p3,float* out)
{
	float v1[3] = {p2[0]-p1[0],p2[1]-p1[1],p2[2]-p1[2]};
	float v2[3] = {p3[0]-p1[0],p3[1]-p1[1],p3[2]-p1[2]};

	out[0] = v1[1]*v2[2] - v1[2]*v2[1];
	out[1] = v1[2]*v2[0] - v1[0]*v2[2];
	out[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

unsigned char* CTerrain::CaculateLightMap(float light_x,float light_y,float light_h,float scale)
{
	unsigned char* ldata = new unsigned char[ (m_map._info._width) * (m_map._info._height) ]; 

	memset((void*)ldata,0,m_map._info._height * m_map._info._width * sizeof(char)); 
	float n[3];
	float ldir[3] = {light_x,light_y,light_h};
	float llen  = ldir[0]*ldir[0] + ldir[1]*ldir[1] + ldir[2]*ldir[2];
	llen = sqrt(llen);

	float p1[3],p2[3],p3[3];
	int w = m_map._info._width;
	int h = m_map._info._height;

	unsigned char cl;
	for(int x = 0 ; x< w - 1;x++)
		for(int y =0 ;y< h - 1;y++)
		{

			p1[0] = x     ; p1[1] = y     ; p1[2] = m_data_array.GetValue( x    , y    ) * scale;
			p2[0] = x + 1 ; p2[1] = y     ; p2[2] = m_data_array.GetValue( x + 1, y    )  * scale;
			p3[0] = x     ; p3[1] = y + 1 ; p3[2] = m_data_array.GetValue( x    , y + 1)  * scale;
			GetNormal(p1,p2,p3,n);

			float dot = (ldir[0]*n[0] + ldir[1]*n[1] + ldir[2]*n[2])
				        / (sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]) * llen);
			if(dot < 0)
				cl = 0;//( 55 - fabs(dot) * 55);
			else
			{
				cl = dot * 255;
			}
			ldata[x + y* (m_map._info._width)] = cl;
		}

	//光滑亮度图
		for(int i=0;i<0;i++)
		{
			for(int x=0;x<m_w;x++)
				for(int y =0;y<m_w;y++)
				{
					int lx = x-1;
					if(lx<0) lx= m_w-1;
					int ly = y-1;
					if(ly<0) ly= m_h-1;
					int ux = x+1;
					if(ux >= m_w)ux =0;
					int uy = y+1;
					if(uy >= m_h)uy =0;
					
					h  =  4 * ldata[ (lx) + y    * m_w ];
					h  += 4 * ldata[ (ux) + y    * m_w ];
					h  += 4 * ldata[ x    + (uy) * m_w ];
					h  += 4 * ldata[ x    + (ly) * m_w ];
					h  += 2 * ldata[ (lx) + (lx) * m_w ];
					h  += 2 * ldata[ (ux) + (ly) * m_w ];
					h  += 2 * ldata[ (lx) + (uy) * m_w ];
					h  += 2 * ldata[ (ux) + (uy) * m_w ];
					h  += 6 * ldata[ x     + y   * m_w ];
					h /= 30.0;

					h*=1.1;
					if(h>255)
						h=255;
					ldata[ x     + y     * m_w ] = h;
				}
		}
		return ldata;

}

GLuint CTerrain::BuilMainTexture(char *filename, float light_x,float light_y,float light_z,float scale)
{
	IMAGESIZE size;

	//Caculate the light map the light map is the same size as the terrain map
	unsigned char *ldata = CaculateLightMap(light_x,light_y,light_z,scale);

	if(ldata == NULL)
		return -1;

	//Load texture data (Use NeHe's iPicture OLE Picture Loader ,it can load any picture format);
	IMAGEDATA image = ::LoadPictureAsTexture(filename,&size,0);

	//Failed load texture data
	if(image == NULL )
	{
		delete [] ldata;
		return -1;
	}

	//缩放亮度图，使其和纹理的尺寸相同
	float sx =  (m_map._info._width  - 1)  / (size.width - 1);
	float sy =  (m_map._info._height - 1) / (size.height- 1);
	for(int x = 0;x<size.width;x++)
		for(int y=0;y<size.height;y++)
		{
			int lx = sx*x;
			int ly = sy*y;
			
			float st = ldata[lx + m_map._info._width*ly] / 255.;
			st = sqrt(st);
			//调制纹理亮度
			image[ (3*x    ) + size.width * 3 * y] *= (0.9*st);
			image[ (3*x + 1) + size.width * 3 * y] *= (.9*st);

			if(image[ (3*x + 1) + size.width * 3 * y]>255)
				image[ (3*x + 1) + size.width * 3 * y] = 255;

			image[ (3*x + 2) + size.width * 3 * y] *= (0.9*st);
		} 

	//Free light map
	delete[] ldata;
	GLuint texture;
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);

    glTexImage2D(GL_TEXTURE_2D,0,3,	size.width,	size.height,0,GL_BGR_EXT ,GL_UNSIGNED_BYTE,image);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glEnable(GL_TEXTURE_2D);

	FreeImageData(image);
	return texture;

}
