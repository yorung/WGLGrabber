# WGLGrabber
A very simple binder for OpenGL API on Windows(WGL)

# How to use
Add following 2 files to your project,

* WGLGrabberGen.h
* WGLGrabberGen.cpp

and just call WGLGrabberInit after activate your own WGL context by calling wglMakeCurrent.

# Project files
This repository includes two project.

* WGLGrabber
* WGLTest

WGLGrabber can compile without any extra files or libraries. "API and Extension Header Files" are needed distributed on https://www.opengl.org/registry/ to run WGLGrabber. Place them into glheaders before run. The generated above two files will be overwritten.

WGLTest is a simple test project using the generated files.

Both project was generated using Microsoft Visual Studio Community 2013.
