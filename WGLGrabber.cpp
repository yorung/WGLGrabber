// WGLGrabber.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "opengl32.lib")

HGLRC hglrc;

static void err(char *msg)
{
	puts(msg);
}

void Create()
{
	HWND hWnd = GetDesktopWindow();
	HDC hdc = GetDC(hWnd);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int pixelFormat;
	if (!(pixelFormat = ChoosePixelFormat(hdc, &pfd))){
		err("ChoosePixelFormat failed.");
		goto END;
	}
	if (!SetPixelFormat(hdc, pixelFormat, &pfd)){
		err("SetPixelFormat failed.");
		goto END;
	}
	if (!(hglrc = wglCreateContext(hdc))){
		err("wglCreateContext failed.");
		goto END;
	}
	if (!wglMakeCurrent(hdc, hglrc)){
		err("wglMakeCurrent failed.");
		goto END;
	}
	void* glpu = wglGetProcAddress("glProgramUniform1f");
	void* gldei = wglGetProcAddress("glDrawElementsIndirect");
	void* glen1 = glEnable;
	void* glen2 = wglGetProcAddress("glEnable");
	GLuint (*glCreateProgram)(void);
	glCreateProgram = (GLuint (*)(void))wglGetProcAddress("glCreateProgram");
	GLuint (*glCreateShader)(GLenum type);
	glCreateShader = (GLuint(*)(GLenum type))wglGetProcAddress("glCreateShader");
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	static const int attribList[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 0,
		WGL_CONTEXT_FLAGS_ARB, 0,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_ES2_PROFILE_BIT_EXT,
		0,
	};
	HGLRC hglrcNew = wglCreateContextAttribsARB(hdc, 0, attribList);
	if (hglrcNew) {
		wglDeleteContext(hglrc);
		hglrc = hglrcNew;
	}
END:
	ReleaseDC(hWnd, hdc);
}

void Destroy()
{
	wglMakeCurrent(nullptr, nullptr);
	if (hglrc) {
		wglDeleteContext(hglrc);
		hglrc = nullptr;
	}
}

void Grab()
{
	//	char* h = (char*)LoadFile("glheaders/wglext.h");
	char* h = (char*)LoadFile("glheaders/glext.h");
	//	std::regex pattern("wglCreateContextAttribsARB");
	std::regex pattern("^GLAPI\\s+(\\w+)\\s+APIENTRY\\s+(\\w+)\\s*(\\(.*\\))");
	//	std::regex pattern("PFN[A-Za-Z0-9_]+PROC");
	std::cmatch match;
	std::string str = h;
	auto funcBegin = std::sregex_iterator(str.begin(), str.end(), pattern);
	auto End = std::sregex_iterator();
	int dist = std::distance(funcBegin, End);
	for (auto it = funcBegin; it != End; it++) {
		std::smatch m = *it;
		if (m.size() < 4) {
			continue;
		}
		printf("%s\n", m.str().c_str());
		printf("%s %s%s\n", m[1].str().c_str(), m[2].str().c_str(), m[3].str().c_str());
		printf("%s (*%s)%s\n", m[1].str().c_str(), m[2].str().c_str(), m[3].str().c_str());
		printf("%s (*)%s\n", m[1].str().c_str(), m[3].str().c_str());
	}
	free(h);
}

int _tmain(int argc, _TCHAR* argv[])
{
	GoMyDir();
	Create();
	Grab();
	Destroy();
	return 0;
}

