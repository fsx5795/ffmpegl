#ifndef GLEXP_H
#define GLEXP_H

#include <GL/gl.h>
#include <GL/glext.h>

#ifdef WIN32
#define GLGetProcAddress wglGetProcAddress
#else
#define GLGetProcAddress glXGetProcAddress
#endif

static inline void APIENTRY glDeleteProgram(GLuint program)
{
    return ((PFNGLDELETEPROGRAMPROC)GLGetProcAddress("glDeleteProgram"))(program);
}
static inline void APIENTRY glDeleteBuffers(GLsizei n, const GLuint *buffers)
{
    return ((PFNGLDELETEBUFFERSPROC)GLGetProcAddress("glDeleteBuffers"))(n, buffers);
}
static inline void APIENTRY glDeleteVertexArrays(GLsizei n, const GLuint *arrays)
{
    return ((PFNGLDELETEVERTEXARRAYSPROC)GLGetProcAddress("glDeleteVertexArrays"))(n, arrays);
}
static inline GLuint APIENTRY glCreateShader(GLenum type)
{
    return ((PFNGLCREATESHADERPROC)GLGetProcAddress("glCreateShader"))(type);
}
static inline void APIENTRY glShaderSource(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length)
{
    return ((PFNGLSHADERSOURCEPROC)GLGetProcAddress("glShaderSource"))(shader, count, string, length);
}
static inline void APIENTRY glCompileShader(GLuint shader)
{
    return ((PFNGLCOMPILESHADERPROC)GLGetProcAddress("glCompileShader"))(shader);
}
static inline void APIENTRY glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
    return ((PFNGLGETSHADERIVPROC)GLGetProcAddress("glGetShaderiv"))(shader, pname, params);
}
static inline void APIENTRY glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    return ((PFNGLGETSHADERINFOLOGPROC)(GLGetProcAddress("glGetShaderInfoLog")))(shader, bufSize, length, infoLog);
}
static inline void APIENTRY glGenVertexArrays(GLsizei n, GLuint *arrays)
{
    return ((PFNGLGENVERTEXARRAYSPROC)(GLGetProcAddress("glGenVertexArrays")))(n, arrays);
}
static inline void APIENTRY glBindVertexArray(GLuint array)
{
    return ((PFNGLBINDVERTEXARRAYPROC)(GLGetProcAddress("glBindVertexArray")))(array);
}
static inline void APIENTRY glGenBuffers(GLsizei n, GLuint *buffers)
{
    return ((PFNGLGENBUFFERSPROC)(GLGetProcAddress("glGenBuffers")))(n, buffers);
}
static inline void APIENTRY glBindBuffer(GLenum target, GLuint buffer)
{
    return ((PFNGLBINDBUFFERPROC)(GLGetProcAddress("glBindBuffer")))(target, buffer);
}
static inline void APIENTRY glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
{
    return ((PFNGLBUFFERDATAPROC)(GLGetProcAddress("glBufferData")))(target, size, data, usage);
}
static inline void APIENTRY glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
{
    return ((PFNGLVERTEXATTRIBPOINTERPROC)(GLGetProcAddress("glVertexAttribPointer")))(index, size, type, normalized, stride, pointer);
}
static inline void APIENTRY glEnableVertexAttribArray(GLuint index)
{
    return ((PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLGetProcAddress("glEnableVertexAttribArray")))(index);
}
static inline GLuint APIENTRY glCreateProgram(void)
{
    return ((PFNGLCREATEPROGRAMPROC)(GLGetProcAddress("glCreateProgram")))();
}
static inline void APIENTRY glAttachShader(GLuint program, GLuint shader)
{
    return ((PFNGLATTACHSHADERPROC)(GLGetProcAddress("glAttachShader")))(program, shader);
}
static inline void APIENTRY glLinkProgram(GLuint program)
{
    return ((PFNGLLINKPROGRAMPROC)(GLGetProcAddress("glLinkProgram")))(program);
}
static inline void APIENTRY glGetProgramiv(GLuint program, GLenum pname, GLint *params)
{
    return ((PFNGLGETPROGRAMIVPROC)(GLGetProcAddress("glGetProgramiv")))(program, pname, params);
}
static inline void APIENTRY glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    return ((PFNGLGETPROGRAMINFOLOGPROC)(GLGetProcAddress("glGetProgramInfoLog")))(program, bufSize, length, infoLog);
}
static inline void APIENTRY glDetachShader(GLuint program, GLuint shader)
{
    return ((PFNGLDETACHSHADERPROC)(GLGetProcAddress("glDetachShader")))(program, shader);
}
static inline void APIENTRY glDeleteShader(GLuint shader)
{
    return ((PFNGLDELETESHADERPROC)(GLGetProcAddress("glDeleteShader")))(shader);
}
static inline void APIENTRY glUseProgram(GLuint program)
{
    return ((PFNGLUSEPROGRAMPROC)(GLGetProcAddress("glUseProgram")))(program);
}
static inline void APIENTRY glActiveTexture(GLenum texture)
{
    return ((PFNGLACTIVETEXTUREPROC)(GLGetProcAddress("glActiveTexture")))(texture);
}
static inline void APIENTRY glUniform1i(GLint location, GLint v0)
{
    return ((PFNGLUNIFORM1IPROC)(GLGetProcAddress("glUniform1i")))(location, v0);
}
static inline GLint APIENTRY glGetUniformLocation(GLuint program, const GLchar *name)
{
    return ((PFNGLGETUNIFORMLOCATIONPROC)(GLGetProcAddress("glGetUniformLocation")))(program, name);
}
static inline void APIENTRY glGenerateMipmap(GLenum target)
{
    return ((PFNGLGENERATEMIPMAPPROC)(GLGetProcAddress("glGenerateMipmap")))(target);
}

#endif
