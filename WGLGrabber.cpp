// WGLGrabber.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "opengl32.lib")

HGLRC hglrc;

struct GLFunc
{
	GLFunc(){}
	GLFunc(const GLFunc& r) {
		*this = r;
	}
	const GLFunc& operator=(const GLFunc& r) {
		name = r.name;
		decl = r.decl;
		caster = r.caster;
		return *this;
	}
	std::string name;
	std::string decl;
	std::string caster;
};
std::vector<GLFunc> glFuncs;

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

	wglMakeCurrent(nullptr, nullptr);
	if (hglrcNew) {
		wglDeleteContext(hglrc);
		hglrc = hglrcNew;
	}
	if (!wglMakeCurrent(hdc, hglrc)){
		err("wglMakeCurrent failed.");
		goto END;
	}

	void WGLGrabberInit();
	WGLGrabberInit();
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

void ParseHeader()
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
	printf("%d functions found\n", dist);
	int i = 0;
	for (auto it = funcBegin; it != End; it++) {
		std::smatch m = *it;
		if (m.size() < 4) {
			continue;
		}
		GLFunc func;
		func.name = m[2].str();
		func.decl = m[1].str() + " (*" + m[2].str() + ")" + m[3].str();
		func.caster = m[1].str() + " (*)" + m[3].str();
		glFuncs.push_back(func);
		printf("\r%d/%d", i++, dist);
	}
	printf("\n");
	free(h);
}

void CodeGen()
{
	std::string hdr, cpp;
	hdr += "#include <windows.h>\r\n";
	hdr += "#include <gl/gl.h>\r\n";
	hdr += "#include \"glheaders/glext.h\"\r\n";
	for (auto it : glFuncs) {
		hdr += std::string("extern ") + it.decl + ";\r\n";
	}
	SaveFile("WGLGrabberGen.h", (uint8_t*)hdr.c_str(), hdr.size());

	cpp = "#include \"WGLGrabberGen.h\"\r\n";
	for (auto it : glFuncs) {
		cpp += it.decl + ";\r\n";
	}

	cpp += "void WGLGrabberInit()\r\n";
	cpp += "{\r\n";
	for (auto it : glFuncs) {
		cpp += std::string("\t") + it.name + " = (" + it.caster + ")wglGetProcAddress(\"" + it.name + "\");\r\n";
	}
	cpp += "}\r\n";

	SaveFile("WGLGrabberGen.cpp", (uint8_t*)cpp.c_str(), cpp.size());
}

int _tmain(int argc, _TCHAR* argv[])
{
	GoMyDir();
	Create();
	ParseHeader();
	CodeGen();
	Destroy();
	return 0;
}

