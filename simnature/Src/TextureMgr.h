/**********************************************************
  SimNature Ver1.0.0
  Copyright : Panliliang (Stanly Lee)
  2002 - 2003
  Alright reserved!
  All Noncommeric use is Free!

  ��������Ȩ���� 
             2003-2003
  ����������з���ҵ��ʹ��Ϊ��Ѵ��롣

  E- Mial: Panliliang@etang.com
		   Xheartblue@etang.com 




  ģ������: Texture Manager
  �����������ṩ�����������������һ��������������
            �����Զ��Ѵ��������������ͷŵ�

  �汾��Ver l.0
  ���ߣ������� 2003 - 3

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