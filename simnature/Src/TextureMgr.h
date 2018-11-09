/**********************************************************
  SimNature Ver1.0.0
  Copyright : Panliliang (Stanly Lee)
  2002 - 2003
  Alright reserved!
  All Noncommeric use is Free!

  潘李亮版权所有 
             2003-2003
  本程序的所有非商业化使用为免费代码。

  E- Mial: Panliliang@etang.com
		   Xheartblue@etang.com 




  模块名称: Texture Manager
  功能描述：提供对纹理对象管理。如果把一个纹理交给它管理。
            可以自动把创建的纹理对象给释放掉

  版本：Ver l.0
  作者：潘李亮 2003 - 3

**********************************************************/

#ifndef    _TEXTURE_MGR_
#define   _TEXTURE_MGR_
#include "gltexture.h"
#include <vector>
using namespace std;
#include <windows.h>
#include <gl\glext.h>

extern PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB;
extern PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;


class CTextureMgr  
{

public:
	CTextureMgr();
	virtual ~CTextureMgr();

public:
	BOOL AddTexture(GLuint tex);
	void Clear();
	void CreateTextures(GLuint* out,int n);
	GLuint CreateTexture();
public:
	GLuint LoadTexture(char* file);
	GLuint CreateTexture(PDIBTEXDATA pdib);

private:
	vector<GLTEXTURE> m_textures;
};

#endif 