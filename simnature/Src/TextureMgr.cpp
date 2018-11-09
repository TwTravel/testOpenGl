#include "TextureMgr.h"



CTextureMgr::CTextureMgr()
{

}

CTextureMgr::~CTextureMgr()
{
	Clear();
}

GLuint CTextureMgr::CreateTexture()
{
	GLuint tex;
	glGenTextures(1,&tex);
	m_textures.push_back(tex);
	return tex;
}

void CTextureMgr::CreateTextures(GLuint *out, int n)
{
	glGenTextures(n,out);
	for(int i=0;i<n;i++)
	{
		m_textures.push_back(out[i]); 
	}
}

void CTextureMgr::Clear()
{
	GLuint n;
	for(int i=0;i<m_textures.size();i++)
	{
		n = m_textures[i];
		if(glIsTexture(n))
			glDeleteTextures(1,&n);
	}
}

BOOL CTextureMgr::AddTexture(GLuint tex)
{
	if( !glIsTexture(tex))
		return FALSE;
	for(int i=0;i<m_textures.size();i++)
	{
		if(m_textures[i] == tex)
		{
			if( !glIsTexture(tex))
				return TRUE;
			else
				return FALSE;
		}
	}
	m_textures.push_back(tex);
	return TRUE;
}
GLuint CTextureMgr::LoadTexture(char* file)
{
    IMAGESIZE size;
	GLubyte* pData;
	GLuint texture;

	
	pData = LoadPictureAsTexture(file,&size,0);
    if(pData == NULL)
		return -1;
	
	glGenTextures(1,&texture);
	m_textures.push_back(texture);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);


    glTexImage2D(GL_TEXTURE_2D,0,3,
		size.width,
		size.height,0,
		GL_BGR_EXT,GL_UNSIGNED_BYTE,pData);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);


	FreeImageData(pData);
	return texture;
}
GLuint CTextureMgr::CreateTexture(PDIBTEXDATA pdib)
{
	GLuint texture;
	glGenTextures(1,&texture);
	m_textures.push_back(texture);
	glBindTexture(GL_TEXTURE_2D,texture);

    if(pdib->bits==24)
	{
    glTexImage2D(GL_TEXTURE_2D,0,3,
		pdib->width,
		pdib->height,0,
		pdib->format ,GL_UNSIGNED_BYTE,
		pdib->pdata);
	}
    if(pdib->bits==32)
	{
    glTexImage2D(GL_TEXTURE_2D,0,4,
		pdib->width,
		pdib->height,0,
		pdib->format,GL_UNSIGNED_BYTE,
		pdib->pdata);
    }
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glEnable(GL_TEXTURE_2D);

	return texture;
}
