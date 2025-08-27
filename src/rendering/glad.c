#include "glad.h"
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>

static HMODULE libGL;

typedef void* (APIENTRYP PFNWGLGETPROCADDRESSPROC_PRIVATE)(const char*);
static PFNWGLGETPROCADDRESSPROC_PRIVATE gladGetProcAddressPtr;

static int open_gl(void) {
    libGL = LoadLibraryA("opengl32.dll");
    if(libGL != NULL) {
        gladGetProcAddressPtr = (PFNWGLGETPROCADDRESSPROC_PRIVATE)GetProcAddress(libGL, "wglGetProcAddress");
        return gladGetProcAddressPtr != NULL;
    }
    return 0;
}

static void close_gl(void) {
    if(libGL != NULL) {
        FreeLibrary(libGL);
        libGL = NULL;
    }
}

static void* get_proc(const char *namez) {
    void* result = NULL;
    if(libGL == NULL) return NULL;
    
    if(gladGetProcAddressPtr != NULL) {
        result = gladGetProcAddressPtr(namez);
    }
    if(result == NULL) {
        result = (void*)GetProcAddress(libGL, namez);
    }
    return result;
}
#else
#include <dlfcn.h>
static void* libGL;

static int open_gl(void) {
    libGL = dlopen("libGL.so.1", RTLD_LAZY | RTLD_GLOBAL);
    if (!libGL) {
        libGL = dlopen("libGL.so", RTLD_LAZY | RTLD_GLOBAL);
    }
    return libGL != NULL;
}

static void close_gl(void) {
    if(libGL != NULL) {
        dlclose(libGL);
        libGL = NULL;
    }
}

static void* get_proc(const char *namez) {
    void* result;
    if(libGL == NULL) return NULL;
    result = dlsym(libGL, namez);
    return result;
}
#endif

PFNGLCLEARPROC glad_glClear = NULL;
PFNGLCLEARCOLORPROC glad_glClearColor = NULL;
PFNGLENABLEPROC glad_glEnable = NULL;
PFNGLDISABLEPROC glad_glDisable = NULL;
PFNGLVIEWPORTPROC glad_glViewport = NULL;
PFNGLDRAWARRAYSPROC glad_glDrawArrays = NULL;
PFNGLDRAWELEMENTSPROC glad_glDrawElements = NULL;
PFNGLGENBUFFERSPROC glad_glGenBuffers = NULL;
PFNGLBINDBUFFERPROC glad_glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glad_glBufferData = NULL;
PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer = NULL;
PFNGLCREATESHADERPROC glad_glCreateShader = NULL;
PFNGLSHADERSOURCEPROC glad_glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glad_glCompileShader = NULL;
PFNGLGETSHADERIVPROC glad_glGetShaderiv = NULL;
PFNGLCREATEPROGRAMPROC glad_glCreateProgram = NULL;
PFNGLATTACHSHADERPROC glad_glAttachShader = NULL;
PFNGLLINKPROGRAMPROC glad_glLinkProgram = NULL;
PFNGLGETPROGRAMIVPROC glad_glGetProgramiv = NULL;
PFNGLUSEPROGRAMPROC glad_glUseProgram = NULL;
PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation = NULL;
PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv = NULL;
PFNGLUNIFORM1FPROC glad_glUniform1f = NULL;
PFNGLUNIFORM1IPROC glad_glUniform1i = NULL;
PFNGLDELETESHADERPROC glad_glDeleteShader = NULL;
PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog = NULL;
PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog = NULL;
PFNGLBLENDFUNCPROC glad_glBlendFunc = NULL;
PFNGLGETSTRINGPROC glad_glGetString = NULL;
PFNGLPOINTSIZEPROC glad_glPointSize = NULL;
PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays = NULL;
PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers = NULL;
PFNGLDELETEPROGRAMPROC glad_glDeleteProgram = NULL;
PFNGLUNIFORM3FPROC glad_glUniform3f = NULL;
PFNGLUNIFORM4FPROC glad_glUniform4f = NULL;
PFNGLREADPIXELSPROC glad_glReadPixels = NULL;
PFNGLLINEWIDTHPROC glad_glLineWidth = NULL;
PFNGLPOLYGONMODEPROC glad_glPolygonMode = NULL;
PFNGLGENFRAMEBUFFERSPROC glad_glGenFramebuffers = NULL;
PFNGLBINDFRAMEBUFFERPROC glad_glBindFramebuffer = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC glad_glFramebufferTexture2D = NULL;
PFNGLGENRENDERBUFFERSPROC glad_glGenRenderbuffers = NULL;
PFNGLBINDRENDERBUFFERPROC glad_glBindRenderbuffer = NULL;
PFNGLRENDERBUFFERSTORAGEPROC glad_glRenderbufferStorage = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glad_glFramebufferRenderbuffer = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glad_glCheckFramebufferStatus = NULL;
PFNGLDELETEFRAMEBUFFERSPROC glad_glDeleteFramebuffers = NULL;
PFNGLDELETERENDERBUFFERSPROC glad_glDeleteRenderbuffers = NULL;

static void load_GL_VERSION_1_0(void) {
    glad_glClear = (PFNGLCLEARPROC)get_proc("glClear");
    glad_glClearColor = (PFNGLCLEARCOLORPROC)get_proc("glClearColor");
    glad_glEnable = (PFNGLENABLEPROC)get_proc("glEnable");
    glad_glDisable = (PFNGLDISABLEPROC)get_proc("glDisable");
    glad_glViewport = (PFNGLVIEWPORTPROC)get_proc("glViewport");
    glad_glDrawArrays = (PFNGLDRAWARRAYSPROC)get_proc("glDrawArrays");
    glad_glDrawElements = (PFNGLDRAWELEMENTSPROC)get_proc("glDrawElements");
    glad_glBlendFunc = (PFNGLBLENDFUNCPROC)get_proc("glBlendFunc");
    glad_glGetString = (PFNGLGETSTRINGPROC)get_proc("glGetString");
}

static void load_GL_VERSION_1_5(void) {
    glad_glGenBuffers = (PFNGLGENBUFFERSPROC)get_proc("glGenBuffers");
    glad_glBindBuffer = (PFNGLBINDBUFFERPROC)get_proc("glBindBuffer");
    glad_glBufferData = (PFNGLBUFFERDATAPROC)get_proc("glBufferData");
    glad_glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)get_proc("glDeleteBuffers");
}

static void load_GL_VERSION_2_0(void) {
    glad_glCreateShader = (PFNGLCREATESHADERPROC)get_proc("glCreateShader");
    glad_glShaderSource = (PFNGLSHADERSOURCEPROC)get_proc("glShaderSource");
    glad_glCompileShader = (PFNGLCOMPILESHADERPROC)get_proc("glCompileShader");
    glad_glGetShaderiv = (PFNGLGETSHADERIVPROC)get_proc("glGetShaderiv");
    glad_glCreateProgram = (PFNGLCREATEPROGRAMPROC)get_proc("glCreateProgram");
    glad_glAttachShader = (PFNGLATTACHSHADERPROC)get_proc("glAttachShader");
    glad_glLinkProgram = (PFNGLLINKPROGRAMPROC)get_proc("glLinkProgram");
    glad_glGetProgramiv = (PFNGLGETPROGRAMIVPROC)get_proc("glGetProgramiv");
    glad_glUseProgram = (PFNGLUSEPROGRAMPROC)get_proc("glUseProgram");
    glad_glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)get_proc("glGetUniformLocation");
    glad_glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)get_proc("glUniformMatrix4fv");
    glad_glUniform1f = (PFNGLUNIFORM1FPROC)get_proc("glUniform1f");
    glad_glUniform1i = (PFNGLUNIFORM1IPROC)get_proc("glUniform1i");
    glad_glUniform3f = (PFNGLUNIFORM3FPROC)get_proc("glUniform3f");
    glad_glUniform4f = (PFNGLUNIFORM4FPROC)get_proc("glUniform4f");
    glad_glDeleteShader = (PFNGLDELETESHADERPROC)get_proc("glDeleteShader");
    glad_glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)get_proc("glGetShaderInfoLog");
    glad_glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)get_proc("glGetProgramInfoLog");
    glad_glPointSize = (PFNGLPOINTSIZEPROC)get_proc("glPointSize");
    glad_glDeleteProgram = (PFNGLDELETEPROGRAMPROC)get_proc("glDeleteProgram");
    glad_glReadPixels = (PFNGLREADPIXELSPROC)get_proc("glReadPixels");
    glad_glLineWidth = (PFNGLLINEWIDTHPROC)get_proc("glLineWidth");
    glad_glPolygonMode = (PFNGLPOLYGONMODEPROC)get_proc("glPolygonMode");
}

static void load_GL_VERSION_3_0(void) {
    glad_glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)get_proc("glGenVertexArrays");
    glad_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)get_proc("glBindVertexArray");
    glad_glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)get_proc("glEnableVertexAttribArray");
    glad_glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)get_proc("glVertexAttribPointer");
    glad_glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)get_proc("glDeleteVertexArrays");
    glad_glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)get_proc("glDeleteBuffers");

    // Framebuffer functions
    glad_glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)get_proc("glGenFramebuffers");
    glad_glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)get_proc("glBindFramebuffer");
    glad_glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)get_proc("glFramebufferTexture2D");
    glad_glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)get_proc("glGenRenderbuffers");
    glad_glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)get_proc("glBindRenderbuffer");
    glad_glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)get_proc("glRenderbufferStorage");
    glad_glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)get_proc("glFramebufferRenderbuffer");
    glad_glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)get_proc("glCheckFramebufferStatus");
    glad_glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)get_proc("glDeleteFramebuffers");
    glad_glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)get_proc("glDeleteRenderbuffers");
}

int gladLoadGL(void) {
    if(open_gl()) {
        load_GL_VERSION_1_0();
        load_GL_VERSION_1_5();
        load_GL_VERSION_2_0();
        load_GL_VERSION_3_0();
        close_gl();
        return 1;
    }
    return 0;
}