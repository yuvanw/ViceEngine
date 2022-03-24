#include "WindowsPlatformOpenGL.h"

#include "CoreMinimal.h"
#include "AssertionMacros.h"

// System includes for Win32 and OpenGL functions.
#include "Windows.h"
#include <GL/gl.h>
#include "GL/glext.h"
#include "GL/wglext.h"

// WGL extensions
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;

// OpenGL extensions
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = nullptr;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = nullptr;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = nullptr;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = nullptr;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = nullptr;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = nullptr;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = nullptr;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = nullptr;
PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
PFNGLUNIFORM1IPROC glUniform1i = nullptr;
PFNGLUNIFORM1FPROC glUniform1f = nullptr;
PFNGLUNIFORM2FPROC glUniform2f = nullptr;
PFNGLUNIFORM3FPROC glUniform3f = nullptr;
PFNGLUNIFORM4FPROC glUniform4f = nullptr;
PFNGLUNIFORM2FVPROC glUniform2fv = nullptr;
PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;
PFNGLUNIFORM4FVPROC glUniform4fv = nullptr;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = nullptr;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;

static HWND WindowHandle = nullptr;
static HDC DeviceContext = nullptr;
static HGLRC RenderingContext = nullptr;

static void LoadOpenGLExtensions()
{
	// WGL functions
	wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
	ensure(wglChoosePixelFormatARB);

	wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
	ensure(wglCreateContextAttribsARB);

	// OpenGL extensions
	glCreateShader = reinterpret_cast<PFNGLCREATESHADERPROC>(wglGetProcAddress("glCreateShader"));
	ensure(glCreateShader);

	glDeleteShader = reinterpret_cast<PFNGLDELETESHADERPROC>(wglGetProcAddress("glDeleteShader"));
	ensure(glDeleteShader);

	glShaderSource = reinterpret_cast<PFNGLSHADERSOURCEPROC>(wglGetProcAddress("glShaderSource"));
	ensure(glShaderSource);

	glCompileShader = reinterpret_cast<PFNGLCOMPILESHADERPROC>(wglGetProcAddress("glCompileShader"));
	ensure(glCompileShader);

	glGetShaderiv = reinterpret_cast<PFNGLGETSHADERIVPROC>(wglGetProcAddress("glGetShaderiv"));
	ensure(glGetShaderiv);

	glGetShaderInfoLog = reinterpret_cast<PFNGLGETSHADERINFOLOGPROC>(wglGetProcAddress("glGetShaderInfoLog"));
	ensure(glGetShaderInfoLog);

	glCreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMPROC>(wglGetProcAddress("glCreateProgram"));
	ensure(glCreateProgram);

	glDeleteProgram = reinterpret_cast<PFNGLDELETEPROGRAMPROC>(wglGetProcAddress("glDeleteProgram"));
	ensure(glDeleteProgram);

	glAttachShader = reinterpret_cast<PFNGLATTACHSHADERPROC>(wglGetProcAddress("glAttachShader"));
	ensure(glAttachShader);

	glLinkProgram = reinterpret_cast<PFNGLLINKPROGRAMPROC>(wglGetProcAddress("glLinkProgram"));
	ensure(glLinkProgram);

	glUseProgram = reinterpret_cast<PFNGLUSEPROGRAMPROC>(wglGetProcAddress("glUseProgram"));
	ensure(glUseProgram);

	glGetProgramiv = reinterpret_cast<PFNGLGETPROGRAMIVPROC>(wglGetProcAddress("glGetProgramiv"));
	ensure(glGetProgramiv);

	glGetProgramInfoLog = reinterpret_cast<PFNGLGETPROGRAMINFOLOGPROC>(wglGetProcAddress("glGetProgramInfoLog"));
	ensure(glGetProgramInfoLog);

	glGenVertexArrays = reinterpret_cast<PFNGLGENVERTEXARRAYSPROC>(wglGetProcAddress("glGenVertexArrays"));
	ensure(glGenVertexArrays);

	glDeleteVertexArrays = reinterpret_cast<PFNGLDELETEVERTEXARRAYSPROC>(wglGetProcAddress("glDeleteVertexArrays"));
	ensure(glDeleteVertexArrays);

	glBindVertexArray = reinterpret_cast<PFNGLBINDVERTEXARRAYPROC>(wglGetProcAddress("glBindVertexArray"));
	ensure(glBindVertexArray);

	glGenBuffers = reinterpret_cast<PFNGLGENBUFFERSPROC>(wglGetProcAddress("glGenBuffers"));
	ensure(glGenBuffers);

	glDeleteBuffers = reinterpret_cast<PFNGLDELETEBUFFERSPROC>(wglGetProcAddress("glDeleteBuffers"));
	ensure(glDeleteBuffers);

	glBindBuffer = reinterpret_cast<PFNGLBINDBUFFERPROC>(wglGetProcAddress("glBindBuffer"));
	ensure(glBindBuffer);

	glBufferData = reinterpret_cast<PFNGLBUFFERDATAPROC>(wglGetProcAddress("glBufferData"));
	ensure(glBufferData);

	glGenFramebuffers = reinterpret_cast<PFNGLGENFRAMEBUFFERSPROC>(wglGetProcAddress("glGenFramebuffers"));
	ensure(glGenFramebuffers);

	glBindFramebuffer = reinterpret_cast<PFNGLBINDFRAMEBUFFERPROC>(wglGetProcAddress("glBindFramebuffer"));
	ensure(glBindFramebuffer);

	glFramebufferTexture2D = reinterpret_cast<PFNGLFRAMEBUFFERTEXTURE2DPROC>(wglGetProcAddress("glFramebufferTexture2D"));
	ensure(glFramebufferTexture2D);

	glFramebufferRenderbuffer = reinterpret_cast<PFNGLFRAMEBUFFERRENDERBUFFERPROC>(wglGetProcAddress("glFramebufferRenderbuffer"));
	ensure(glFramebufferRenderbuffer);

	glCheckFramebufferStatus = reinterpret_cast<PFNGLCHECKFRAMEBUFFERSTATUSPROC>(wglGetProcAddress("glCheckFramebufferStatus"));
	ensure(glCheckFramebufferStatus);

	glDeleteFramebuffers = reinterpret_cast<PFNGLDELETEFRAMEBUFFERSPROC>(wglGetProcAddress("glDeleteFramebuffers"));
	ensure(glDeleteFramebuffers);

	glGenRenderbuffers = reinterpret_cast<PFNGLGENRENDERBUFFERSPROC>(wglGetProcAddress("glGenRenderbuffers"));
	ensure(glGenRenderbuffers);

	glBindRenderbuffer = reinterpret_cast<PFNGLBINDRENDERBUFFERPROC>(wglGetProcAddress("glBindRenderbuffer"));
	ensure(glBindRenderbuffer);

	glRenderbufferStorage = reinterpret_cast<PFNGLRENDERBUFFERSTORAGEPROC>(wglGetProcAddress("glRenderbufferStorage"));
	ensure(glRenderbufferStorage);

	glDeleteRenderbuffers = reinterpret_cast<PFNGLDELETERENDERBUFFERSPROC>(wglGetProcAddress("glDeleteRenderbuffers"));
	ensure(glDeleteRenderbuffers);

	glVertexAttribPointer = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERPROC>(wglGetProcAddress("glVertexAttribPointer"));
	ensure(glVertexAttribPointer);

	glEnableVertexAttribArray = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYPROC>(wglGetProcAddress("glEnableVertexAttribArray"));
	ensure(glEnableVertexAttribArray);

	glGenerateMipmap = reinterpret_cast<PFNGLGENERATEMIPMAPPROC>(wglGetProcAddress("glGenerateMipmap"));
	ensure(glGenerateMipmap);

	glActiveTexture = reinterpret_cast<PFNGLACTIVETEXTUREPROC>(wglGetProcAddress("glActiveTexture"));
	ensure(glActiveTexture);

	glUniform1i = reinterpret_cast<PFNGLUNIFORM1IPROC>(wglGetProcAddress("glUniform1i"));
	ensure(glUniform1i);

	glUniform1f = reinterpret_cast<PFNGLUNIFORM1FPROC>(wglGetProcAddress("glUniform1f"));
	ensure(glUniform1f);

	glUniform2f = reinterpret_cast<PFNGLUNIFORM2FPROC>(wglGetProcAddress("glUniform2f"));
	ensure(glUniform2f);

	glUniform3f = reinterpret_cast<PFNGLUNIFORM3FPROC>(wglGetProcAddress("glUniform3f"));
	ensure(glUniform3f);

	glUniform4f = reinterpret_cast<PFNGLUNIFORM4FPROC>(wglGetProcAddress("glUniform4f"));
	ensure(glUniform4f);

	glUniform2fv = reinterpret_cast<PFNGLUNIFORM2FVPROC>(wglGetProcAddress("glUniform2fv"));
	ensure(glUniform2fv);

	glUniform3fv = reinterpret_cast<PFNGLUNIFORM3FVPROC>(wglGetProcAddress("glUniform3fv"));
	ensure(glUniform3fv);

	glUniform4fv = reinterpret_cast<PFNGLUNIFORM4FVPROC>(wglGetProcAddress("glUniform4fv"));
	ensure(glUniform4fv);

	glUniformMatrix2fv = reinterpret_cast<PFNGLUNIFORMMATRIX2FVPROC>(wglGetProcAddress("glUniformMatrix2fv"));
	ensure(glUniformMatrix2fv);

	glUniformMatrix3fv = reinterpret_cast<PFNGLUNIFORMMATRIX3FVPROC>(wglGetProcAddress("glUniformMatrix3fv"));
	ensure(glUniformMatrix3fv);

	glUniformMatrix4fv = reinterpret_cast<PFNGLUNIFORMMATRIX4FVPROC>(wglGetProcAddress("glUniformMatrix4fv"));
	ensure(glUniformMatrix4fv);

	glGetUniformLocation = reinterpret_cast<PFNGLGETUNIFORMLOCATIONPROC>(wglGetProcAddress("glGetUniformLocation"));
	ensure(glGetUniformLocation);
}

/*static*/ void FWindowsPlatformOpenGL::Init(void* InNativeWindowHandle)
{
	static bool bIsInitialized = false;
	if (bIsInitialized)
	{
		return;
	}

	// Create dummy window.
	HINSTANCE InstanceHandle = GetModuleHandle(NULL);
	const char* DummyWindowClassName = "DummyWindow";

	WNDCLASS DummyWindowClass;
	ZeroMemory(&DummyWindowClass, sizeof(DummyWindowClass));
	DummyWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	DummyWindowClass.lpfnWndProc = DefWindowProc;
	DummyWindowClass.hInstance = InstanceHandle;
	DummyWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	DummyWindowClass.lpszClassName = DummyWindowClassName;

	RegisterClass(&DummyWindowClass);

	HWND DummyWindowHandle = CreateWindow(
		DummyWindowClassName,
		"Dummy Window",
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0,
		1, 1,
		NULL, NULL,
		InstanceHandle,
		NULL
	);

	HDC DummyDeviceContext = GetDC(DummyWindowHandle);

	// Create dummy rendering context.
	PIXELFORMATDESCRIPTOR DummyPixelFormatDescriptor;
	ZeroMemory(&DummyPixelFormatDescriptor, sizeof(DummyPixelFormatDescriptor));
	DummyPixelFormatDescriptor.nSize = sizeof(DummyPixelFormatDescriptor);
	DummyPixelFormatDescriptor.nVersion = 1;
	DummyPixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	DummyPixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
	DummyPixelFormatDescriptor.cColorBits = 32;
	DummyPixelFormatDescriptor.cAlphaBits = 8;
	DummyPixelFormatDescriptor.cDepthBits = 24;

	int32 DummyPixelFormatDescriptorId = ChoosePixelFormat(DummyDeviceContext, &DummyPixelFormatDescriptor);
	SetPixelFormat(DummyDeviceContext, DummyPixelFormatDescriptorId, &DummyPixelFormatDescriptor);

	HGLRC DummyRenderingContext = wglCreateContext(DummyDeviceContext);
	wglMakeCurrent(DummyDeviceContext, DummyRenderingContext);

	// Load function pointers for modern OpenGL functions.
	LoadOpenGLExtensions();

	// Create rendering context using input window handle.
	WindowHandle = reinterpret_cast<HWND>(InNativeWindowHandle);
	DeviceContext = GetDC(WindowHandle);

	const int32 PixelAttributes[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, 4,
		0
	};

	int32 PixelFormatId;
	uint32 NumFormats;
	bool bIsPixelFormatChosen = wglChoosePixelFormatARB(DeviceContext, PixelAttributes, NULL, 1, &PixelFormatId, &NumFormats);
	ensure(bIsPixelFormatChosen);
	ensure(NumFormats != 0);

	PIXELFORMATDESCRIPTOR PixelFormatDescriptor;
	DescribePixelFormat(DeviceContext, PixelFormatId, sizeof(PixelFormatDescriptor), &PixelFormatDescriptor);
	SetPixelFormat(DeviceContext, PixelFormatId, &PixelFormatDescriptor);

	// Request an OpenGL 3.3 context.
	const int32 OpenGLMajorVersion = 3;
	const int32 OpenGLMinorVersion = 3;
	int32  ContextAttributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, OpenGLMajorVersion,
		WGL_CONTEXT_MINOR_VERSION_ARB, OpenGLMinorVersion,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	RenderingContext = wglCreateContextAttribsARB(DeviceContext, 0, ContextAttributes);
	ensure(RenderingContext);

	// Release dummy resources.
	wglDeleteContext(DummyRenderingContext);
	ReleaseDC(DummyWindowHandle, DummyDeviceContext);
	DestroyWindow(DummyWindowHandle);

	// Make the OpenGL 3.3 context this thread's current context.
	wglMakeCurrent(DeviceContext, RenderingContext);

	bIsInitialized = true;
}

/*static*/ void FWindowsPlatformOpenGL::Shutdown()
{
	wglDeleteContext(RenderingContext);
	ReleaseDC(WindowHandle, DeviceContext);
}

/*static*/ void FWindowsPlatformOpenGL::SwapBuffers()
{
	::SwapBuffers(DeviceContext);
}
