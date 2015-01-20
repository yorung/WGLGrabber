// WGLGrabber.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

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


void ParseHeader(const char* healderFileName, const char* regExp, const char* conventions)
{
	char* h = (char*)LoadFile(healderFileName);
	std::regex pattern(regExp);
	std::cmatch match;
	std::string str = h;
	auto funcBegin = std::sregex_iterator(str.begin(), str.end(), pattern);
	auto End = std::sregex_iterator();
	int dist = std::distance(funcBegin, End);
	printf("%d functions found in %s\n", dist, healderFileName);
	int i = 0;
	for (auto it = funcBegin; it != End; it++) {
		std::smatch m = *it;
		if (m.size() < 4) {
			continue;
		}
		GLFunc func;
		func.name = m[2].str();
		func.decl = m[1].str() + " (" + conventions + "*" + m[2].str() + ")" + m[3].str();
		func.caster = m[1].str() + " (" + conventions + "*)" + m[3].str();
		glFuncs.push_back(func);
		printf("\r%d/%d", ++i, dist);
	}
	printf("\n");
	free(h);
}

void CodeGen()
{
	std::string hdr, cpp;
	hdr += "#include <windows.h>\r\n";
//	hdr += "#include <gl/gl.h>\r\n";
//	hdr += "#include \"glheaders/glext.h\"\r\n";
	hdr += "#include \"glheaders/glcorearb.h\"\r\n";
	hdr += "#include \"glheaders/wglext.h\"\r\n";
	for (auto it : glFuncs) {
		hdr += std::string("extern ") + it.decl + ";\r\n";
	}
	hdr += "void WGLGrabberInit();\r\n";
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
//	ParseHeader("glheaders/glext.h", "^GLAPI\\s+(\\w+)\\s+APIENTRY\\s+(gl\\w+)\\s*(\\(.*\\))", "APIENTRY");
	ParseHeader("glheaders/glcorearb.h", "^GLAPI\\s+(\\w+)\\s+APIENTRY\\s+(gl\\w+)\\s*(\\(.*\\))", "APIENTRY");
	ParseHeader("glheaders/wglext.h", "^(\\w+(?:\\s+\\w+)*)\\s+WINAPI\\s+(wgl\\w+)\\s*(\\(.*\\))", "WINAPI");
	CodeGen();
	return 0;
}

