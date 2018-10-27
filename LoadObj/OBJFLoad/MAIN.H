#ifndef _MAIN_H
#define _MAIN_H


extern HWND  g_hWnd;										
extern RECT  g_rRect;										
extern HDC   g_hDC;											
extern HGLRC g_hRC;											
extern HINSTANCE g_hInstance;								

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow);
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WPARAM MainLoop();

// 生成纹理，并将其保存在纹理队列中
void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID);

// 改变显示为全屏模式
void ChangeToFullScreen();

// 生成用户窗口
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);

// 设置像素格式
bool bSetupPixelFormat(HDC hdc);

// 初始化投影变换
void SizeOpenGLScreen(int width, int height);

// 初始化OpenGL
void InitializeOpenGL(int width, int height);

// 初始化整个程序
void Init(HWND hWnd);

// 渲染场景
void RenderScene();

// 释放程序占用的内存空间
void DeInit();

#endif 


