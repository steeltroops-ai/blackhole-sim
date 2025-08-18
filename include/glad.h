#ifndef __glad_h_
#define __glad_h_

#ifdef __gl_h_
#error OpenGL header already included, remove this include, glad already provides it
#endif

#define __gl_h_

#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>
#endif

#include <stddef.h>

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif

// OpenGL Constants
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_FALSE                          0
#define GL_TRUE                           1
#define GL_TRIANGLES                      0x0004
#define GL_ONE                            1
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_FRONT_AND_BACK                 0x0408
#define GL_FILL                           0x1B02
#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_EXTENSIONS                     0x1F03
#define GL_NEAREST                        0x2600
#define GL_LINEAR                         0x2601
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_REPEAT                         0x2901
#define GL_POLYGON_MODE                   0x0B40
#define GL_CULL_FACE                      0x0B44
#define GL_DEPTH_TEST                     0x0B71
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_BLEND                          0x0BE2
#define GL_TEXTURE_2D                     0x0DE1
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_RGBA                           0x1908
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_MULTISAMPLE                    0x809D

// Framebuffer constants
#define GL_FRAMEBUFFER                    0x8D40
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_RENDERBUFFER                   0x8D41
#define GL_DEPTH_COMPONENT                0x1902
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5

// Additional constants for complete OpenGL support
#define GL_RGB                            0x1907
#define GL_BGR                            0x80E0
#define GL_BGRA                           0x80E1
#define GL_CLAMP_TO_EDGE                  0x812F

typedef void GLvoid;
typedef unsigned int GLenum;
typedef float GLfloat;
typedef int GLint;
typedef int GLsizei;
typedef unsigned int GLbitfield;
typedef double GLdouble;
typedef unsigned int GLuint;
typedef unsigned char GLboolean;
typedef unsigned char GLubyte;
typedef char GLchar;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

// Core OpenGL function typedefs
typedef void (APIENTRYP PFNGLCLEARPROC) (GLbitfield mask);
typedef void (APIENTRYP PFNGLCLEARCOLORPROC) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (APIENTRYP PFNGLENABLEPROC) (GLenum cap);
typedef void (APIENTRYP PFNGLDISABLEPROC) (GLenum cap);
typedef void (APIENTRYP PFNGLVIEWPORTPROC) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLDRAWARRAYSPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRYP PFNGLDRAWELEMENTSPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices);
typedef void (APIENTRYP PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRYP PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRYP PFNGLBUFFERDATAPROC) (GLenum target, GLsizei size, const void *data, GLenum usage);
typedef void (APIENTRYP PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);
typedef void (APIENTRYP PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef GLuint (APIENTRYP PFNGLCREATESHADERPROC) (GLenum type);
typedef void (APIENTRYP PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (APIENTRYP PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef void (APIENTRYP PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
typedef GLuint (APIENTRYP PFNGLCREATEPROGRAMPROC) (void);

// Additional function typedefs for complete OpenGL support
typedef void (APIENTRYP PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRYP PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRYP PFNGLDELETESHADERPROC) (GLuint shader);
typedef void (APIENTRYP PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLUSEPROGRAMPROC) (GLuint program);
typedef GLint (APIENTRYP PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar *name);
typedef void (APIENTRYP PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void (APIENTRYP PFNGLUNIFORM3FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRYP PFNGLUNIFORM4FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRYP PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

// Framebuffer function typedefs
typedef void (APIENTRYP PFNGLGENFRAMEBUFFERSPROC) (GLsizei n, GLuint *framebuffers);
typedef void (APIENTRYP PFNGLBINDFRAMEBUFFERPROC) (GLenum target, GLuint framebuffer);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (APIENTRYP PFNGLGENRENDERBUFFERSPROC) (GLsizei n, GLuint *renderbuffers);
typedef void (APIENTRYP PFNGLBINDRENDERBUFFERPROC) (GLenum target, GLuint renderbuffer);
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLFRAMEBUFFERRENDERBUFFERPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef GLenum (APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSPROC) (GLenum target);
typedef void (APIENTRYP PFNGLDELETEFRAMEBUFFERSPROC) (GLsizei n, const GLuint *framebuffers);
typedef void (APIENTRYP PFNGLDELETERENDERBUFFERSPROC) (GLsizei n, const GLuint *renderbuffers);

// Additional utility functions
typedef void (APIENTRYP PFNGLBLENDFUNCPROC) (GLenum sfactor, GLenum dfactor);
typedef const GLubyte* (APIENTRYP PFNGLGETSTRINGPROC) (GLenum name);
typedef void (APIENTRYP PFNGLREADPIXELSPROC) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
typedef void (APIENTRYP PFNGLPOINTSIZEPROC) (GLfloat size);
typedef void (APIENTRYP PFNGLLINEWIDTHPROC) (GLfloat width);
typedef void (APIENTRYP PFNGLPOLYGONMODEPROC) (GLenum face, GLenum mode);
typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRYP PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint *arrays);
// Function pointer declarations
GLAPI PFNGLCLEARPROC glad_glClear;
GLAPI PFNGLCLEARCOLORPROC glad_glClearColor;
GLAPI PFNGLENABLEPROC glad_glEnable;
GLAPI PFNGLDISABLEPROC glad_glDisable;
GLAPI PFNGLVIEWPORTPROC glad_glViewport;
GLAPI PFNGLDRAWARRAYSPROC glad_glDrawArrays;
GLAPI PFNGLDRAWELEMENTSPROC glad_glDrawElements;
GLAPI PFNGLGENBUFFERSPROC glad_glGenBuffers;
GLAPI PFNGLBINDBUFFERPROC glad_glBindBuffer;
GLAPI PFNGLBUFFERDATAPROC glad_glBufferData;
GLAPI PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays;
GLAPI PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray;
GLAPI PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray;
GLAPI PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer;
GLAPI PFNGLCREATESHADERPROC glad_glCreateShader;
GLAPI PFNGLSHADERSOURCEPROC glad_glShaderSource;
GLAPI PFNGLCOMPILESHADERPROC glad_glCompileShader;
GLAPI PFNGLGETSHADERIVPROC glad_glGetShaderiv;
GLAPI PFNGLCREATEPROGRAMPROC glad_glCreateProgram;
GLAPI PFNGLATTACHSHADERPROC glad_glAttachShader;
GLAPI PFNGLLINKPROGRAMPROC glad_glLinkProgram;
GLAPI PFNGLGETPROGRAMIVPROC glad_glGetProgramiv;
GLAPI PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog;
GLAPI PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog;
GLAPI PFNGLDELETESHADERPROC glad_glDeleteShader;
GLAPI PFNGLDELETEPROGRAMPROC glad_glDeleteProgram;
GLAPI PFNGLUSEPROGRAMPROC glad_glUseProgram;
GLAPI PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation;
GLAPI PFNGLUNIFORM1FPROC glad_glUniform1f;
GLAPI PFNGLUNIFORM3FPROC glad_glUniform3f;
GLAPI PFNGLUNIFORM4FPROC glad_glUniform4f;
GLAPI PFNGLUNIFORM1IPROC glad_glUniform1i;
GLAPI PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv;
GLAPI PFNGLGENFRAMEBUFFERSPROC glad_glGenFramebuffers;
GLAPI PFNGLBINDFRAMEBUFFERPROC glad_glBindFramebuffer;
GLAPI PFNGLFRAMEBUFFERTEXTURE2DPROC glad_glFramebufferTexture2D;
GLAPI PFNGLGENRENDERBUFFERSPROC glad_glGenRenderbuffers;
GLAPI PFNGLBINDRENDERBUFFERPROC glad_glBindRenderbuffer;
GLAPI PFNGLRENDERBUFFERSTORAGEPROC glad_glRenderbufferStorage;
GLAPI PFNGLFRAMEBUFFERRENDERBUFFERPROC glad_glFramebufferRenderbuffer;
GLAPI PFNGLCHECKFRAMEBUFFERSTATUSPROC glad_glCheckFramebufferStatus;
GLAPI PFNGLDELETEFRAMEBUFFERSPROC glad_glDeleteFramebuffers;
GLAPI PFNGLDELETERENDERBUFFERSPROC glad_glDeleteRenderbuffers;
GLAPI PFNGLBLENDFUNCPROC glad_glBlendFunc;
GLAPI PFNGLGETSTRINGPROC glad_glGetString;
GLAPI PFNGLREADPIXELSPROC glad_glReadPixels;
GLAPI PFNGLPOINTSIZEPROC glad_glPointSize;
GLAPI PFNGLLINEWIDTHPROC glad_glLineWidth;
GLAPI PFNGLPOLYGONMODEPROC glad_glPolygonMode;
GLAPI PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers;
GLAPI PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays;

GLAPI PFNGLCLEARPROC glad_glClear;
GLAPI PFNGLCLEARCOLORPROC glad_glClearColor;
GLAPI PFNGLENABLEPROC glad_glEnable;
GLAPI PFNGLDISABLEPROC glad_glDisable;
GLAPI PFNGLVIEWPORTPROC glad_glViewport;
GLAPI PFNGLDRAWARRAYSPROC glad_glDrawArrays;
GLAPI PFNGLDRAWELEMENTSPROC glad_glDrawElements;
GLAPI PFNGLGENBUFFERSPROC glad_glGenBuffers;
GLAPI PFNGLBINDBUFFERPROC glad_glBindBuffer;
GLAPI PFNGLBUFFERDATAPROC glad_glBufferData;
GLAPI PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays;
GLAPI PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray;
GLAPI PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray;
GLAPI PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer;
GLAPI PFNGLCREATESHADERPROC glad_glCreateShader;
GLAPI PFNGLSHADERSOURCEPROC glad_glShaderSource;
GLAPI PFNGLCOMPILESHADERPROC glad_glCompileShader;
GLAPI PFNGLGETSHADERIVPROC glad_glGetShaderiv;
GLAPI PFNGLCREATEPROGRAMPROC glad_glCreateProgram;
GLAPI PFNGLATTACHSHADERPROC glad_glAttachShader;
GLAPI PFNGLLINKPROGRAMPROC glad_glLinkProgram;
GLAPI PFNGLGETPROGRAMIVPROC glad_glGetProgramiv;
GLAPI PFNGLUSEPROGRAMPROC glad_glUseProgram;
GLAPI PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation;
GLAPI PFNGLDELETESHADERPROC glad_glDeleteShader;
GLAPI PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog;
GLAPI PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog;
GLAPI PFNGLPOINTSIZEPROC glad_glPointSize;
GLAPI PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays;
GLAPI PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers;
GLAPI PFNGLDELETEPROGRAMPROC glad_glDeleteProgram;
GLAPI PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv;
GLAPI PFNGLUNIFORM1FPROC glad_glUniform1f;
GLAPI PFNGLUNIFORM1IPROC glad_glUniform1i;
GLAPI PFNGLBLENDFUNCPROC glad_glBlendFunc;
GLAPI PFNGLGETSTRINGPROC glad_glGetString;
GLAPI PFNGLREADPIXELSPROC glad_glReadPixels;
GLAPI PFNGLLINEWIDTHPROC glad_glLineWidth;
GLAPI PFNGLPOLYGONMODEPROC glad_glPolygonMode;
GLAPI PFNGLUNIFORM3FPROC glad_glUniform3f;
GLAPI PFNGLUNIFORM4FPROC glad_glUniform4f;
GLAPI PFNGLGENFRAMEBUFFERSPROC glad_glGenFramebuffers;
GLAPI PFNGLBINDFRAMEBUFFERPROC glad_glBindFramebuffer;
GLAPI PFNGLFRAMEBUFFERTEXTURE2DPROC glad_glFramebufferTexture2D;
GLAPI PFNGLGENRENDERBUFFERSPROC glad_glGenRenderbuffers;
GLAPI PFNGLBINDRENDERBUFFERPROC glad_glBindRenderbuffer;
GLAPI PFNGLRENDERBUFFERSTORAGEPROC glad_glRenderbufferStorage;
GLAPI PFNGLFRAMEBUFFERRENDERBUFFERPROC glad_glFramebufferRenderbuffer;
GLAPI PFNGLCHECKFRAMEBUFFERSTATUSPROC glad_glCheckFramebufferStatus;
GLAPI PFNGLDELETEFRAMEBUFFERSPROC glad_glDeleteFramebuffers;
GLAPI PFNGLDELETERENDERBUFFERSPROC glad_glDeleteRenderbuffers;

#define glClear glad_glClear
#define glClearColor glad_glClearColor
#define glEnable glad_glEnable
#define glDisable glad_glDisable
#define glViewport glad_glViewport
#define glDrawArrays glad_glDrawArrays
#define glDrawElements glad_glDrawElements
#define glGenBuffers glad_glGenBuffers
#define glBindBuffer glad_glBindBuffer
#define glBufferData glad_glBufferData
#define glGenVertexArrays glad_glGenVertexArrays
#define glBindVertexArray glad_glBindVertexArray
#define glEnableVertexAttribArray glad_glEnableVertexAttribArray
#define glVertexAttribPointer glad_glVertexAttribPointer
#define glCreateShader glad_glCreateShader
#define glShaderSource glad_glShaderSource
#define glCompileShader glad_glCompileShader
#define glGetShaderiv glad_glGetShaderiv
#define glCreateProgram glad_glCreateProgram
#define glAttachShader glad_glAttachShader
#define glLinkProgram glad_glLinkProgram
#define glGetProgramiv glad_glGetProgramiv
#define glUseProgram glad_glUseProgram
#define glGetUniformLocation glad_glGetUniformLocation
#define glUniformMatrix4fv glad_glUniformMatrix4fv
#define glUniform1f glad_glUniform1f
#define glUniform1i glad_glUniform1i
#define glDeleteShader glad_glDeleteShader
#define glGetShaderInfoLog glad_glGetShaderInfoLog
#define glGetProgramInfoLog glad_glGetProgramInfoLog
#define glPointSize glad_glPointSize
#define glDeleteVertexArrays glad_glDeleteVertexArrays
#define glDeleteBuffers glad_glDeleteBuffers
#define glDeleteProgram glad_glDeleteProgram
#define glUniformMatrix4fv glad_glUniformMatrix4fv
#define glUniform1f glad_glUniform1f
#define glUniform1i glad_glUniform1i
#define glUniform3f glad_glUniform3f
#define glUniform4f glad_glUniform4f
#define glBlendFunc glad_glBlendFunc
#define glGetString glad_glGetString
#define glReadPixels glad_glReadPixels
#define glLineWidth glad_glLineWidth
#define glPolygonMode glad_glPolygonMode
#define glGenFramebuffers glad_glGenFramebuffers
#define glBindFramebuffer glad_glBindFramebuffer
#define glFramebufferTexture2D glad_glFramebufferTexture2D
#define glGenRenderbuffers glad_glGenRenderbuffers
#define glBindRenderbuffer glad_glBindRenderbuffer
#define glRenderbufferStorage glad_glRenderbufferStorage
#define glFramebufferRenderbuffer glad_glFramebufferRenderbuffer
#define glCheckFramebufferStatus glad_glCheckFramebufferStatus
#define glDeleteFramebuffers glad_glDeleteFramebuffers
#define glDeleteRenderbuffers glad_glDeleteRenderbuffers

#ifdef __cplusplus
extern "C" {
#endif

int gladLoadGL(void);

#ifdef __cplusplus
}
#endif

#endif