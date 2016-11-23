/* WARNING: This file was automatically generated */
/* Do not edit. */

#include "flextGL.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* No __stdcall on Linux */
#ifdef __linux
#define __stdcall
#endif

typedef void(__stdcall *GLPROC)();

void flextLoadOpenGLFunctions(void);

static void open_libgl(void);
static void close_libgl(void);
static GLPROC get_proc(const char *proc);
static void add_extension(const char *extension);

int flextInit(void) {
  GLint minor, major;
  GLint num_extensions;
  int i;

  open_libgl();
  flextLoadOpenGLFunctions();
  close_libgl();

  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);

  if (10 * major + minor < 43) {
    fprintf(stderr, "Error: OpenGL version 4.3 not supported.\n");
    fprintf(stderr, "       Your version is %d.%d.\n", major, minor);
    fprintf(stderr, "       Try updating your graphics driver.\n");
    return GL_FALSE;
  }

  /* --- Check for minimal version and profile --- */

  if (major * 10 + minor < 43) {
    fprintf(stderr, "Error: OpenGL version 4.3 not supported.\n");
    fprintf(stderr, "       Your version is %d.%d.\n", major, minor);
    fprintf(stderr, "       Try updating your graphics driver.\n");
    return GL_FALSE;
  }

  /* --- Check for extensions --- */

  glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);

  for (i = 0; i < num_extensions; i++) {
    add_extension((const char *)glGetStringi(GL_EXTENSIONS, i));
  }

  return GL_TRUE;
}

void flextLoadOpenGLFunctions(void) {
  /* --- Function pointer loading --- */

  /* GL_VERSION_1_2 */

  glpfDrawRangeElements =
      (PFNGLDRAWRANGEELEMENTS_PROC *)get_proc("glDrawRangeElements");
  glpfTexImage3D = (PFNGLTEXIMAGE3D_PROC *)get_proc("glTexImage3D");
  glpfTexSubImage3D = (PFNGLTEXSUBIMAGE3D_PROC *)get_proc("glTexSubImage3D");
  glpfCopyTexSubImage3D =
      (PFNGLCOPYTEXSUBIMAGE3D_PROC *)get_proc("glCopyTexSubImage3D");

  /* GL_VERSION_1_3 */

  glpfActiveTexture = (PFNGLACTIVETEXTURE_PROC *)get_proc("glActiveTexture");
  glpfSampleCoverage = (PFNGLSAMPLECOVERAGE_PROC *)get_proc("glSampleCoverage");
  glpfCompressedTexImage3D =
      (PFNGLCOMPRESSEDTEXIMAGE3D_PROC *)get_proc("glCompressedTexImage3D");
  glpfCompressedTexImage2D =
      (PFNGLCOMPRESSEDTEXIMAGE2D_PROC *)get_proc("glCompressedTexImage2D");
  glpfCompressedTexImage1D =
      (PFNGLCOMPRESSEDTEXIMAGE1D_PROC *)get_proc("glCompressedTexImage1D");
  glpfCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3D_PROC *)get_proc(
      "glCompressedTexSubImage3D");
  glpfCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2D_PROC *)get_proc(
      "glCompressedTexSubImage2D");
  glpfCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1D_PROC *)get_proc(
      "glCompressedTexSubImage1D");
  glpfGetCompressedTexImage =
      (PFNGLGETCOMPRESSEDTEXIMAGE_PROC *)get_proc("glGetCompressedTexImage");

  /* GL_VERSION_1_4 */

  glpfBlendFuncSeparate =
      (PFNGLBLENDFUNCSEPARATE_PROC *)get_proc("glBlendFuncSeparate");
  glpfMultiDrawArrays =
      (PFNGLMULTIDRAWARRAYS_PROC *)get_proc("glMultiDrawArrays");
  glpfMultiDrawElements =
      (PFNGLMULTIDRAWELEMENTS_PROC *)get_proc("glMultiDrawElements");
  glpfPointParameterf =
      (PFNGLPOINTPARAMETERF_PROC *)get_proc("glPointParameterf");
  glpfPointParameterfv =
      (PFNGLPOINTPARAMETERFV_PROC *)get_proc("glPointParameterfv");
  glpfPointParameteri =
      (PFNGLPOINTPARAMETERI_PROC *)get_proc("glPointParameteri");
  glpfPointParameteriv =
      (PFNGLPOINTPARAMETERIV_PROC *)get_proc("glPointParameteriv");
  glpfBlendColor = (PFNGLBLENDCOLOR_PROC *)get_proc("glBlendColor");
  glpfBlendEquation = (PFNGLBLENDEQUATION_PROC *)get_proc("glBlendEquation");

  /* GL_VERSION_1_5 */

  glpfGenQueries = (PFNGLGENQUERIES_PROC *)get_proc("glGenQueries");
  glpfDeleteQueries = (PFNGLDELETEQUERIES_PROC *)get_proc("glDeleteQueries");
  glpfIsQuery = (PFNGLISQUERY_PROC *)get_proc("glIsQuery");
  glpfBeginQuery = (PFNGLBEGINQUERY_PROC *)get_proc("glBeginQuery");
  glpfEndQuery = (PFNGLENDQUERY_PROC *)get_proc("glEndQuery");
  glpfGetQueryiv = (PFNGLGETQUERYIV_PROC *)get_proc("glGetQueryiv");
  glpfGetQueryObjectiv =
      (PFNGLGETQUERYOBJECTIV_PROC *)get_proc("glGetQueryObjectiv");
  glpfGetQueryObjectuiv =
      (PFNGLGETQUERYOBJECTUIV_PROC *)get_proc("glGetQueryObjectuiv");
  glpfBindBuffer = (PFNGLBINDBUFFER_PROC *)get_proc("glBindBuffer");
  glpfDeleteBuffers = (PFNGLDELETEBUFFERS_PROC *)get_proc("glDeleteBuffers");
  glpfGenBuffers = (PFNGLGENBUFFERS_PROC *)get_proc("glGenBuffers");
  glpfIsBuffer = (PFNGLISBUFFER_PROC *)get_proc("glIsBuffer");
  glpfBufferData = (PFNGLBUFFERDATA_PROC *)get_proc("glBufferData");
  glpfBufferSubData = (PFNGLBUFFERSUBDATA_PROC *)get_proc("glBufferSubData");
  glpfGetBufferSubData =
      (PFNGLGETBUFFERSUBDATA_PROC *)get_proc("glGetBufferSubData");
  glpfMapBuffer = (PFNGLMAPBUFFER_PROC *)get_proc("glMapBuffer");
  glpfUnmapBuffer = (PFNGLUNMAPBUFFER_PROC *)get_proc("glUnmapBuffer");
  glpfGetBufferParameteriv =
      (PFNGLGETBUFFERPARAMETERIV_PROC *)get_proc("glGetBufferParameteriv");
  glpfGetBufferPointerv =
      (PFNGLGETBUFFERPOINTERV_PROC *)get_proc("glGetBufferPointerv");

  /* GL_VERSION_2_0 */

  glpfBlendEquationSeparate =
      (PFNGLBLENDEQUATIONSEPARATE_PROC *)get_proc("glBlendEquationSeparate");
  glpfDrawBuffers = (PFNGLDRAWBUFFERS_PROC *)get_proc("glDrawBuffers");
  glpfStencilOpSeparate =
      (PFNGLSTENCILOPSEPARATE_PROC *)get_proc("glStencilOpSeparate");
  glpfStencilFuncSeparate =
      (PFNGLSTENCILFUNCSEPARATE_PROC *)get_proc("glStencilFuncSeparate");
  glpfStencilMaskSeparate =
      (PFNGLSTENCILMASKSEPARATE_PROC *)get_proc("glStencilMaskSeparate");
  glpfAttachShader = (PFNGLATTACHSHADER_PROC *)get_proc("glAttachShader");
  glpfBindAttribLocation =
      (PFNGLBINDATTRIBLOCATION_PROC *)get_proc("glBindAttribLocation");
  glpfCompileShader = (PFNGLCOMPILESHADER_PROC *)get_proc("glCompileShader");
  glpfCreateProgram = (PFNGLCREATEPROGRAM_PROC *)get_proc("glCreateProgram");
  glpfCreateShader = (PFNGLCREATESHADER_PROC *)get_proc("glCreateShader");
  glpfDeleteProgram = (PFNGLDELETEPROGRAM_PROC *)get_proc("glDeleteProgram");
  glpfDeleteShader = (PFNGLDELETESHADER_PROC *)get_proc("glDeleteShader");
  glpfDetachShader = (PFNGLDETACHSHADER_PROC *)get_proc("glDetachShader");
  glpfDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAY_PROC *)get_proc(
      "glDisableVertexAttribArray");
  glpfEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAY_PROC *)get_proc(
      "glEnableVertexAttribArray");
  glpfGetActiveAttrib =
      (PFNGLGETACTIVEATTRIB_PROC *)get_proc("glGetActiveAttrib");
  glpfGetActiveUniform =
      (PFNGLGETACTIVEUNIFORM_PROC *)get_proc("glGetActiveUniform");
  glpfGetAttachedShaders =
      (PFNGLGETATTACHEDSHADERS_PROC *)get_proc("glGetAttachedShaders");
  glpfGetAttribLocation =
      (PFNGLGETATTRIBLOCATION_PROC *)get_proc("glGetAttribLocation");
  glpfGetProgramiv = (PFNGLGETPROGRAMIV_PROC *)get_proc("glGetProgramiv");
  glpfGetProgramInfoLog =
      (PFNGLGETPROGRAMINFOLOG_PROC *)get_proc("glGetProgramInfoLog");
  glpfGetShaderiv = (PFNGLGETSHADERIV_PROC *)get_proc("glGetShaderiv");
  glpfGetShaderInfoLog =
      (PFNGLGETSHADERINFOLOG_PROC *)get_proc("glGetShaderInfoLog");
  glpfGetShaderSource =
      (PFNGLGETSHADERSOURCE_PROC *)get_proc("glGetShaderSource");
  glpfGetUniformLocation =
      (PFNGLGETUNIFORMLOCATION_PROC *)get_proc("glGetUniformLocation");
  glpfGetUniformfv = (PFNGLGETUNIFORMFV_PROC *)get_proc("glGetUniformfv");
  glpfGetUniformiv = (PFNGLGETUNIFORMIV_PROC *)get_proc("glGetUniformiv");
  glpfGetVertexAttribdv =
      (PFNGLGETVERTEXATTRIBDV_PROC *)get_proc("glGetVertexAttribdv");
  glpfGetVertexAttribfv =
      (PFNGLGETVERTEXATTRIBFV_PROC *)get_proc("glGetVertexAttribfv");
  glpfGetVertexAttribiv =
      (PFNGLGETVERTEXATTRIBIV_PROC *)get_proc("glGetVertexAttribiv");
  glpfGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERV_PROC *)get_proc(
      "glGetVertexAttribPointerv");
  glpfIsProgram = (PFNGLISPROGRAM_PROC *)get_proc("glIsProgram");
  glpfIsShader = (PFNGLISSHADER_PROC *)get_proc("glIsShader");
  glpfLinkProgram = (PFNGLLINKPROGRAM_PROC *)get_proc("glLinkProgram");
  glpfShaderSource = (PFNGLSHADERSOURCE_PROC *)get_proc("glShaderSource");
  glpfUseProgram = (PFNGLUSEPROGRAM_PROC *)get_proc("glUseProgram");
  glpfUniform1f = (PFNGLUNIFORM1F_PROC *)get_proc("glUniform1f");
  glpfUniform2f = (PFNGLUNIFORM2F_PROC *)get_proc("glUniform2f");
  glpfUniform3f = (PFNGLUNIFORM3F_PROC *)get_proc("glUniform3f");
  glpfUniform4f = (PFNGLUNIFORM4F_PROC *)get_proc("glUniform4f");
  glpfUniform1i = (PFNGLUNIFORM1I_PROC *)get_proc("glUniform1i");
  glpfUniform2i = (PFNGLUNIFORM2I_PROC *)get_proc("glUniform2i");
  glpfUniform3i = (PFNGLUNIFORM3I_PROC *)get_proc("glUniform3i");
  glpfUniform4i = (PFNGLUNIFORM4I_PROC *)get_proc("glUniform4i");
  glpfUniform1fv = (PFNGLUNIFORM1FV_PROC *)get_proc("glUniform1fv");
  glpfUniform2fv = (PFNGLUNIFORM2FV_PROC *)get_proc("glUniform2fv");
  glpfUniform3fv = (PFNGLUNIFORM3FV_PROC *)get_proc("glUniform3fv");
  glpfUniform4fv = (PFNGLUNIFORM4FV_PROC *)get_proc("glUniform4fv");
  glpfUniform1iv = (PFNGLUNIFORM1IV_PROC *)get_proc("glUniform1iv");
  glpfUniform2iv = (PFNGLUNIFORM2IV_PROC *)get_proc("glUniform2iv");
  glpfUniform3iv = (PFNGLUNIFORM3IV_PROC *)get_proc("glUniform3iv");
  glpfUniform4iv = (PFNGLUNIFORM4IV_PROC *)get_proc("glUniform4iv");
  glpfUniformMatrix2fv =
      (PFNGLUNIFORMMATRIX2FV_PROC *)get_proc("glUniformMatrix2fv");
  glpfUniformMatrix3fv =
      (PFNGLUNIFORMMATRIX3FV_PROC *)get_proc("glUniformMatrix3fv");
  glpfUniformMatrix4fv =
      (PFNGLUNIFORMMATRIX4FV_PROC *)get_proc("glUniformMatrix4fv");
  glpfValidateProgram =
      (PFNGLVALIDATEPROGRAM_PROC *)get_proc("glValidateProgram");
  glpfVertexAttrib1d = (PFNGLVERTEXATTRIB1D_PROC *)get_proc("glVertexAttrib1d");
  glpfVertexAttrib1dv =
      (PFNGLVERTEXATTRIB1DV_PROC *)get_proc("glVertexAttrib1dv");
  glpfVertexAttrib1f = (PFNGLVERTEXATTRIB1F_PROC *)get_proc("glVertexAttrib1f");
  glpfVertexAttrib1fv =
      (PFNGLVERTEXATTRIB1FV_PROC *)get_proc("glVertexAttrib1fv");
  glpfVertexAttrib1s = (PFNGLVERTEXATTRIB1S_PROC *)get_proc("glVertexAttrib1s");
  glpfVertexAttrib1sv =
      (PFNGLVERTEXATTRIB1SV_PROC *)get_proc("glVertexAttrib1sv");
  glpfVertexAttrib2d = (PFNGLVERTEXATTRIB2D_PROC *)get_proc("glVertexAttrib2d");
  glpfVertexAttrib2dv =
      (PFNGLVERTEXATTRIB2DV_PROC *)get_proc("glVertexAttrib2dv");
  glpfVertexAttrib2f = (PFNGLVERTEXATTRIB2F_PROC *)get_proc("glVertexAttrib2f");
  glpfVertexAttrib2fv =
      (PFNGLVERTEXATTRIB2FV_PROC *)get_proc("glVertexAttrib2fv");
  glpfVertexAttrib2s = (PFNGLVERTEXATTRIB2S_PROC *)get_proc("glVertexAttrib2s");
  glpfVertexAttrib2sv =
      (PFNGLVERTEXATTRIB2SV_PROC *)get_proc("glVertexAttrib2sv");
  glpfVertexAttrib3d = (PFNGLVERTEXATTRIB3D_PROC *)get_proc("glVertexAttrib3d");
  glpfVertexAttrib3dv =
      (PFNGLVERTEXATTRIB3DV_PROC *)get_proc("glVertexAttrib3dv");
  glpfVertexAttrib3f = (PFNGLVERTEXATTRIB3F_PROC *)get_proc("glVertexAttrib3f");
  glpfVertexAttrib3fv =
      (PFNGLVERTEXATTRIB3FV_PROC *)get_proc("glVertexAttrib3fv");
  glpfVertexAttrib3s = (PFNGLVERTEXATTRIB3S_PROC *)get_proc("glVertexAttrib3s");
  glpfVertexAttrib3sv =
      (PFNGLVERTEXATTRIB3SV_PROC *)get_proc("glVertexAttrib3sv");
  glpfVertexAttrib4Nbv =
      (PFNGLVERTEXATTRIB4NBV_PROC *)get_proc("glVertexAttrib4Nbv");
  glpfVertexAttrib4Niv =
      (PFNGLVERTEXATTRIB4NIV_PROC *)get_proc("glVertexAttrib4Niv");
  glpfVertexAttrib4Nsv =
      (PFNGLVERTEXATTRIB4NSV_PROC *)get_proc("glVertexAttrib4Nsv");
  glpfVertexAttrib4Nub =
      (PFNGLVERTEXATTRIB4NUB_PROC *)get_proc("glVertexAttrib4Nub");
  glpfVertexAttrib4Nubv =
      (PFNGLVERTEXATTRIB4NUBV_PROC *)get_proc("glVertexAttrib4Nubv");
  glpfVertexAttrib4Nuiv =
      (PFNGLVERTEXATTRIB4NUIV_PROC *)get_proc("glVertexAttrib4Nuiv");
  glpfVertexAttrib4Nusv =
      (PFNGLVERTEXATTRIB4NUSV_PROC *)get_proc("glVertexAttrib4Nusv");
  glpfVertexAttrib4bv =
      (PFNGLVERTEXATTRIB4BV_PROC *)get_proc("glVertexAttrib4bv");
  glpfVertexAttrib4d = (PFNGLVERTEXATTRIB4D_PROC *)get_proc("glVertexAttrib4d");
  glpfVertexAttrib4dv =
      (PFNGLVERTEXATTRIB4DV_PROC *)get_proc("glVertexAttrib4dv");
  glpfVertexAttrib4f = (PFNGLVERTEXATTRIB4F_PROC *)get_proc("glVertexAttrib4f");
  glpfVertexAttrib4fv =
      (PFNGLVERTEXATTRIB4FV_PROC *)get_proc("glVertexAttrib4fv");
  glpfVertexAttrib4iv =
      (PFNGLVERTEXATTRIB4IV_PROC *)get_proc("glVertexAttrib4iv");
  glpfVertexAttrib4s = (PFNGLVERTEXATTRIB4S_PROC *)get_proc("glVertexAttrib4s");
  glpfVertexAttrib4sv =
      (PFNGLVERTEXATTRIB4SV_PROC *)get_proc("glVertexAttrib4sv");
  glpfVertexAttrib4ubv =
      (PFNGLVERTEXATTRIB4UBV_PROC *)get_proc("glVertexAttrib4ubv");
  glpfVertexAttrib4uiv =
      (PFNGLVERTEXATTRIB4UIV_PROC *)get_proc("glVertexAttrib4uiv");
  glpfVertexAttrib4usv =
      (PFNGLVERTEXATTRIB4USV_PROC *)get_proc("glVertexAttrib4usv");
  glpfVertexAttribPointer =
      (PFNGLVERTEXATTRIBPOINTER_PROC *)get_proc("glVertexAttribPointer");

  /* GL_VERSION_2_1 */

  glpfUniformMatrix2x3fv =
      (PFNGLUNIFORMMATRIX2X3FV_PROC *)get_proc("glUniformMatrix2x3fv");
  glpfUniformMatrix3x2fv =
      (PFNGLUNIFORMMATRIX3X2FV_PROC *)get_proc("glUniformMatrix3x2fv");
  glpfUniformMatrix2x4fv =
      (PFNGLUNIFORMMATRIX2X4FV_PROC *)get_proc("glUniformMatrix2x4fv");
  glpfUniformMatrix4x2fv =
      (PFNGLUNIFORMMATRIX4X2FV_PROC *)get_proc("glUniformMatrix4x2fv");
  glpfUniformMatrix3x4fv =
      (PFNGLUNIFORMMATRIX3X4FV_PROC *)get_proc("glUniformMatrix3x4fv");
  glpfUniformMatrix4x3fv =
      (PFNGLUNIFORMMATRIX4X3FV_PROC *)get_proc("glUniformMatrix4x3fv");

  /* GL_VERSION_3_0 */

  glpfColorMaski = (PFNGLCOLORMASKI_PROC *)get_proc("glColorMaski");
  glpfGetBooleani_v = (PFNGLGETBOOLEANI_V_PROC *)get_proc("glGetBooleani_v");
  glpfGetIntegeri_v = (PFNGLGETINTEGERI_V_PROC *)get_proc("glGetIntegeri_v");
  glpfEnablei = (PFNGLENABLEI_PROC *)get_proc("glEnablei");
  glpfDisablei = (PFNGLDISABLEI_PROC *)get_proc("glDisablei");
  glpfIsEnabledi = (PFNGLISENABLEDI_PROC *)get_proc("glIsEnabledi");
  glpfBeginTransformFeedback =
      (PFNGLBEGINTRANSFORMFEEDBACK_PROC *)get_proc("glBeginTransformFeedback");
  glpfEndTransformFeedback =
      (PFNGLENDTRANSFORMFEEDBACK_PROC *)get_proc("glEndTransformFeedback");
  glpfBindBufferRange =
      (PFNGLBINDBUFFERRANGE_PROC *)get_proc("glBindBufferRange");
  glpfBindBufferBase = (PFNGLBINDBUFFERBASE_PROC *)get_proc("glBindBufferBase");
  glpfTransformFeedbackVaryings =
      (PFNGLTRANSFORMFEEDBACKVARYINGS_PROC *)get_proc(
          "glTransformFeedbackVaryings");
  glpfGetTransformFeedbackVarying =
      (PFNGLGETTRANSFORMFEEDBACKVARYING_PROC *)get_proc(
          "glGetTransformFeedbackVarying");
  glpfClampColor = (PFNGLCLAMPCOLOR_PROC *)get_proc("glClampColor");
  glpfBeginConditionalRender =
      (PFNGLBEGINCONDITIONALRENDER_PROC *)get_proc("glBeginConditionalRender");
  glpfEndConditionalRender =
      (PFNGLENDCONDITIONALRENDER_PROC *)get_proc("glEndConditionalRender");
  glpfVertexAttribIPointer =
      (PFNGLVERTEXATTRIBIPOINTER_PROC *)get_proc("glVertexAttribIPointer");
  glpfGetVertexAttribIiv =
      (PFNGLGETVERTEXATTRIBIIV_PROC *)get_proc("glGetVertexAttribIiv");
  glpfGetVertexAttribIuiv =
      (PFNGLGETVERTEXATTRIBIUIV_PROC *)get_proc("glGetVertexAttribIuiv");
  glpfVertexAttribI1i =
      (PFNGLVERTEXATTRIBI1I_PROC *)get_proc("glVertexAttribI1i");
  glpfVertexAttribI2i =
      (PFNGLVERTEXATTRIBI2I_PROC *)get_proc("glVertexAttribI2i");
  glpfVertexAttribI3i =
      (PFNGLVERTEXATTRIBI3I_PROC *)get_proc("glVertexAttribI3i");
  glpfVertexAttribI4i =
      (PFNGLVERTEXATTRIBI4I_PROC *)get_proc("glVertexAttribI4i");
  glpfVertexAttribI1ui =
      (PFNGLVERTEXATTRIBI1UI_PROC *)get_proc("glVertexAttribI1ui");
  glpfVertexAttribI2ui =
      (PFNGLVERTEXATTRIBI2UI_PROC *)get_proc("glVertexAttribI2ui");
  glpfVertexAttribI3ui =
      (PFNGLVERTEXATTRIBI3UI_PROC *)get_proc("glVertexAttribI3ui");
  glpfVertexAttribI4ui =
      (PFNGLVERTEXATTRIBI4UI_PROC *)get_proc("glVertexAttribI4ui");
  glpfVertexAttribI1iv =
      (PFNGLVERTEXATTRIBI1IV_PROC *)get_proc("glVertexAttribI1iv");
  glpfVertexAttribI2iv =
      (PFNGLVERTEXATTRIBI2IV_PROC *)get_proc("glVertexAttribI2iv");
  glpfVertexAttribI3iv =
      (PFNGLVERTEXATTRIBI3IV_PROC *)get_proc("glVertexAttribI3iv");
  glpfVertexAttribI4iv =
      (PFNGLVERTEXATTRIBI4IV_PROC *)get_proc("glVertexAttribI4iv");
  glpfVertexAttribI1uiv =
      (PFNGLVERTEXATTRIBI1UIV_PROC *)get_proc("glVertexAttribI1uiv");
  glpfVertexAttribI2uiv =
      (PFNGLVERTEXATTRIBI2UIV_PROC *)get_proc("glVertexAttribI2uiv");
  glpfVertexAttribI3uiv =
      (PFNGLVERTEXATTRIBI3UIV_PROC *)get_proc("glVertexAttribI3uiv");
  glpfVertexAttribI4uiv =
      (PFNGLVERTEXATTRIBI4UIV_PROC *)get_proc("glVertexAttribI4uiv");
  glpfVertexAttribI4bv =
      (PFNGLVERTEXATTRIBI4BV_PROC *)get_proc("glVertexAttribI4bv");
  glpfVertexAttribI4sv =
      (PFNGLVERTEXATTRIBI4SV_PROC *)get_proc("glVertexAttribI4sv");
  glpfVertexAttribI4ubv =
      (PFNGLVERTEXATTRIBI4UBV_PROC *)get_proc("glVertexAttribI4ubv");
  glpfVertexAttribI4usv =
      (PFNGLVERTEXATTRIBI4USV_PROC *)get_proc("glVertexAttribI4usv");
  glpfGetUniformuiv = (PFNGLGETUNIFORMUIV_PROC *)get_proc("glGetUniformuiv");
  glpfBindFragDataLocation =
      (PFNGLBINDFRAGDATALOCATION_PROC *)get_proc("glBindFragDataLocation");
  glpfGetFragDataLocation =
      (PFNGLGETFRAGDATALOCATION_PROC *)get_proc("glGetFragDataLocation");
  glpfUniform1ui = (PFNGLUNIFORM1UI_PROC *)get_proc("glUniform1ui");
  glpfUniform2ui = (PFNGLUNIFORM2UI_PROC *)get_proc("glUniform2ui");
  glpfUniform3ui = (PFNGLUNIFORM3UI_PROC *)get_proc("glUniform3ui");
  glpfUniform4ui = (PFNGLUNIFORM4UI_PROC *)get_proc("glUniform4ui");
  glpfUniform1uiv = (PFNGLUNIFORM1UIV_PROC *)get_proc("glUniform1uiv");
  glpfUniform2uiv = (PFNGLUNIFORM2UIV_PROC *)get_proc("glUniform2uiv");
  glpfUniform3uiv = (PFNGLUNIFORM3UIV_PROC *)get_proc("glUniform3uiv");
  glpfUniform4uiv = (PFNGLUNIFORM4UIV_PROC *)get_proc("glUniform4uiv");
  glpfTexParameterIiv =
      (PFNGLTEXPARAMETERIIV_PROC *)get_proc("glTexParameterIiv");
  glpfTexParameterIuiv =
      (PFNGLTEXPARAMETERIUIV_PROC *)get_proc("glTexParameterIuiv");
  glpfGetTexParameterIiv =
      (PFNGLGETTEXPARAMETERIIV_PROC *)get_proc("glGetTexParameterIiv");
  glpfGetTexParameterIuiv =
      (PFNGLGETTEXPARAMETERIUIV_PROC *)get_proc("glGetTexParameterIuiv");
  glpfClearBufferiv = (PFNGLCLEARBUFFERIV_PROC *)get_proc("glClearBufferiv");
  glpfClearBufferuiv = (PFNGLCLEARBUFFERUIV_PROC *)get_proc("glClearBufferuiv");
  glpfClearBufferfv = (PFNGLCLEARBUFFERFV_PROC *)get_proc("glClearBufferfv");
  glpfClearBufferfi = (PFNGLCLEARBUFFERFI_PROC *)get_proc("glClearBufferfi");
  glpfGetStringi = (PFNGLGETSTRINGI_PROC *)get_proc("glGetStringi");
  glpfIsRenderbuffer = (PFNGLISRENDERBUFFER_PROC *)get_proc("glIsRenderbuffer");
  glpfBindRenderbuffer =
      (PFNGLBINDRENDERBUFFER_PROC *)get_proc("glBindRenderbuffer");
  glpfDeleteRenderbuffers =
      (PFNGLDELETERENDERBUFFERS_PROC *)get_proc("glDeleteRenderbuffers");
  glpfGenRenderbuffers =
      (PFNGLGENRENDERBUFFERS_PROC *)get_proc("glGenRenderbuffers");
  glpfRenderbufferStorage =
      (PFNGLRENDERBUFFERSTORAGE_PROC *)get_proc("glRenderbufferStorage");
  glpfGetRenderbufferParameteriv =
      (PFNGLGETRENDERBUFFERPARAMETERIV_PROC *)get_proc(
          "glGetRenderbufferParameteriv");
  glpfIsFramebuffer = (PFNGLISFRAMEBUFFER_PROC *)get_proc("glIsFramebuffer");
  glpfBindFramebuffer =
      (PFNGLBINDFRAMEBUFFER_PROC *)get_proc("glBindFramebuffer");
  glpfDeleteFramebuffers =
      (PFNGLDELETEFRAMEBUFFERS_PROC *)get_proc("glDeleteFramebuffers");
  glpfGenFramebuffers =
      (PFNGLGENFRAMEBUFFERS_PROC *)get_proc("glGenFramebuffers");
  glpfCheckFramebufferStatus =
      (PFNGLCHECKFRAMEBUFFERSTATUS_PROC *)get_proc("glCheckFramebufferStatus");
  glpfFramebufferTexture1D =
      (PFNGLFRAMEBUFFERTEXTURE1D_PROC *)get_proc("glFramebufferTexture1D");
  glpfFramebufferTexture2D =
      (PFNGLFRAMEBUFFERTEXTURE2D_PROC *)get_proc("glFramebufferTexture2D");
  glpfFramebufferTexture3D =
      (PFNGLFRAMEBUFFERTEXTURE3D_PROC *)get_proc("glFramebufferTexture3D");
  glpfFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFER_PROC *)get_proc(
      "glFramebufferRenderbuffer");
  glpfGetFramebufferAttachmentParameteriv =
      (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIV_PROC *)get_proc(
          "glGetFramebufferAttachmentParameteriv");
  glpfGenerateMipmap = (PFNGLGENERATEMIPMAP_PROC *)get_proc("glGenerateMipmap");
  glpfBlitFramebuffer =
      (PFNGLBLITFRAMEBUFFER_PROC *)get_proc("glBlitFramebuffer");
  glpfRenderbufferStorageMultisample =
      (PFNGLRENDERBUFFERSTORAGEMULTISAMPLE_PROC *)get_proc(
          "glRenderbufferStorageMultisample");
  glpfFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYER_PROC *)get_proc(
      "glFramebufferTextureLayer");
  glpfMapBufferRange = (PFNGLMAPBUFFERRANGE_PROC *)get_proc("glMapBufferRange");
  glpfFlushMappedBufferRange =
      (PFNGLFLUSHMAPPEDBUFFERRANGE_PROC *)get_proc("glFlushMappedBufferRange");
  glpfBindVertexArray =
      (PFNGLBINDVERTEXARRAY_PROC *)get_proc("glBindVertexArray");
  glpfDeleteVertexArrays =
      (PFNGLDELETEVERTEXARRAYS_PROC *)get_proc("glDeleteVertexArrays");
  glpfGenVertexArrays =
      (PFNGLGENVERTEXARRAYS_PROC *)get_proc("glGenVertexArrays");
  glpfIsVertexArray = (PFNGLISVERTEXARRAY_PROC *)get_proc("glIsVertexArray");

  /* GL_VERSION_3_1 */

  glpfDrawArraysInstanced =
      (PFNGLDRAWARRAYSINSTANCED_PROC *)get_proc("glDrawArraysInstanced");
  glpfDrawElementsInstanced =
      (PFNGLDRAWELEMENTSINSTANCED_PROC *)get_proc("glDrawElementsInstanced");
  glpfTexBuffer = (PFNGLTEXBUFFER_PROC *)get_proc("glTexBuffer");
  glpfPrimitiveRestartIndex =
      (PFNGLPRIMITIVERESTARTINDEX_PROC *)get_proc("glPrimitiveRestartIndex");
  glpfCopyBufferSubData =
      (PFNGLCOPYBUFFERSUBDATA_PROC *)get_proc("glCopyBufferSubData");
  glpfGetUniformIndices =
      (PFNGLGETUNIFORMINDICES_PROC *)get_proc("glGetUniformIndices");
  glpfGetActiveUniformsiv =
      (PFNGLGETACTIVEUNIFORMSIV_PROC *)get_proc("glGetActiveUniformsiv");
  glpfGetActiveUniformName =
      (PFNGLGETACTIVEUNIFORMNAME_PROC *)get_proc("glGetActiveUniformName");
  glpfGetUniformBlockIndex =
      (PFNGLGETUNIFORMBLOCKINDEX_PROC *)get_proc("glGetUniformBlockIndex");
  glpfGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIV_PROC *)get_proc(
      "glGetActiveUniformBlockiv");
  glpfGetActiveUniformBlockName =
      (PFNGLGETACTIVEUNIFORMBLOCKNAME_PROC *)get_proc(
          "glGetActiveUniformBlockName");
  glpfUniformBlockBinding =
      (PFNGLUNIFORMBLOCKBINDING_PROC *)get_proc("glUniformBlockBinding");

  /* GL_VERSION_3_2 */

  glpfDrawElementsBaseVertex =
      (PFNGLDRAWELEMENTSBASEVERTEX_PROC *)get_proc("glDrawElementsBaseVertex");
  glpfDrawRangeElementsBaseVertex =
      (PFNGLDRAWRANGEELEMENTSBASEVERTEX_PROC *)get_proc(
          "glDrawRangeElementsBaseVertex");
  glpfDrawElementsInstancedBaseVertex =
      (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEX_PROC *)get_proc(
          "glDrawElementsInstancedBaseVertex");
  glpfMultiDrawElementsBaseVertex =
      (PFNGLMULTIDRAWELEMENTSBASEVERTEX_PROC *)get_proc(
          "glMultiDrawElementsBaseVertex");
  glpfProvokingVertex =
      (PFNGLPROVOKINGVERTEX_PROC *)get_proc("glProvokingVertex");
  glpfFenceSync = (PFNGLFENCESYNC_PROC *)get_proc("glFenceSync");
  glpfIsSync = (PFNGLISSYNC_PROC *)get_proc("glIsSync");
  glpfDeleteSync = (PFNGLDELETESYNC_PROC *)get_proc("glDeleteSync");
  glpfClientWaitSync = (PFNGLCLIENTWAITSYNC_PROC *)get_proc("glClientWaitSync");
  glpfWaitSync = (PFNGLWAITSYNC_PROC *)get_proc("glWaitSync");
  glpfGetInteger64v = (PFNGLGETINTEGER64V_PROC *)get_proc("glGetInteger64v");
  glpfGetSynciv = (PFNGLGETSYNCIV_PROC *)get_proc("glGetSynciv");
  glpfGetInteger64i_v =
      (PFNGLGETINTEGER64I_V_PROC *)get_proc("glGetInteger64i_v");
  glpfGetBufferParameteri64v =
      (PFNGLGETBUFFERPARAMETERI64V_PROC *)get_proc("glGetBufferParameteri64v");
  glpfFramebufferTexture =
      (PFNGLFRAMEBUFFERTEXTURE_PROC *)get_proc("glFramebufferTexture");
  glpfTexImage2DMultisample =
      (PFNGLTEXIMAGE2DMULTISAMPLE_PROC *)get_proc("glTexImage2DMultisample");
  glpfTexImage3DMultisample =
      (PFNGLTEXIMAGE3DMULTISAMPLE_PROC *)get_proc("glTexImage3DMultisample");
  glpfGetMultisamplefv =
      (PFNGLGETMULTISAMPLEFV_PROC *)get_proc("glGetMultisamplefv");
  glpfSampleMaski = (PFNGLSAMPLEMASKI_PROC *)get_proc("glSampleMaski");

  /* GL_VERSION_3_3 */

  glpfBindFragDataLocationIndexed =
      (PFNGLBINDFRAGDATALOCATIONINDEXED_PROC *)get_proc(
          "glBindFragDataLocationIndexed");
  glpfGetFragDataIndex =
      (PFNGLGETFRAGDATAINDEX_PROC *)get_proc("glGetFragDataIndex");
  glpfGenSamplers = (PFNGLGENSAMPLERS_PROC *)get_proc("glGenSamplers");
  glpfDeleteSamplers = (PFNGLDELETESAMPLERS_PROC *)get_proc("glDeleteSamplers");
  glpfIsSampler = (PFNGLISSAMPLER_PROC *)get_proc("glIsSampler");
  glpfBindSampler = (PFNGLBINDSAMPLER_PROC *)get_proc("glBindSampler");
  glpfSamplerParameteri =
      (PFNGLSAMPLERPARAMETERI_PROC *)get_proc("glSamplerParameteri");
  glpfSamplerParameteriv =
      (PFNGLSAMPLERPARAMETERIV_PROC *)get_proc("glSamplerParameteriv");
  glpfSamplerParameterf =
      (PFNGLSAMPLERPARAMETERF_PROC *)get_proc("glSamplerParameterf");
  glpfSamplerParameterfv =
      (PFNGLSAMPLERPARAMETERFV_PROC *)get_proc("glSamplerParameterfv");
  glpfSamplerParameterIiv =
      (PFNGLSAMPLERPARAMETERIIV_PROC *)get_proc("glSamplerParameterIiv");
  glpfSamplerParameterIuiv =
      (PFNGLSAMPLERPARAMETERIUIV_PROC *)get_proc("glSamplerParameterIuiv");
  glpfGetSamplerParameteriv =
      (PFNGLGETSAMPLERPARAMETERIV_PROC *)get_proc("glGetSamplerParameteriv");
  glpfGetSamplerParameterIiv =
      (PFNGLGETSAMPLERPARAMETERIIV_PROC *)get_proc("glGetSamplerParameterIiv");
  glpfGetSamplerParameterfv =
      (PFNGLGETSAMPLERPARAMETERFV_PROC *)get_proc("glGetSamplerParameterfv");
  glpfGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIV_PROC *)get_proc(
      "glGetSamplerParameterIuiv");
  glpfQueryCounter = (PFNGLQUERYCOUNTER_PROC *)get_proc("glQueryCounter");
  glpfGetQueryObjecti64v =
      (PFNGLGETQUERYOBJECTI64V_PROC *)get_proc("glGetQueryObjecti64v");
  glpfGetQueryObjectui64v =
      (PFNGLGETQUERYOBJECTUI64V_PROC *)get_proc("glGetQueryObjectui64v");
  glpfVertexAttribDivisor =
      (PFNGLVERTEXATTRIBDIVISOR_PROC *)get_proc("glVertexAttribDivisor");
  glpfVertexAttribP1ui =
      (PFNGLVERTEXATTRIBP1UI_PROC *)get_proc("glVertexAttribP1ui");
  glpfVertexAttribP1uiv =
      (PFNGLVERTEXATTRIBP1UIV_PROC *)get_proc("glVertexAttribP1uiv");
  glpfVertexAttribP2ui =
      (PFNGLVERTEXATTRIBP2UI_PROC *)get_proc("glVertexAttribP2ui");
  glpfVertexAttribP2uiv =
      (PFNGLVERTEXATTRIBP2UIV_PROC *)get_proc("glVertexAttribP2uiv");
  glpfVertexAttribP3ui =
      (PFNGLVERTEXATTRIBP3UI_PROC *)get_proc("glVertexAttribP3ui");
  glpfVertexAttribP3uiv =
      (PFNGLVERTEXATTRIBP3UIV_PROC *)get_proc("glVertexAttribP3uiv");
  glpfVertexAttribP4ui =
      (PFNGLVERTEXATTRIBP4UI_PROC *)get_proc("glVertexAttribP4ui");
  glpfVertexAttribP4uiv =
      (PFNGLVERTEXATTRIBP4UIV_PROC *)get_proc("glVertexAttribP4uiv");

  /* GL_VERSION_4_0 */

  glpfMinSampleShading =
      (PFNGLMINSAMPLESHADING_PROC *)get_proc("glMinSampleShading");
  glpfBlendEquationi = (PFNGLBLENDEQUATIONI_PROC *)get_proc("glBlendEquationi");
  glpfBlendEquationSeparatei =
      (PFNGLBLENDEQUATIONSEPARATEI_PROC *)get_proc("glBlendEquationSeparatei");
  glpfBlendFunci = (PFNGLBLENDFUNCI_PROC *)get_proc("glBlendFunci");
  glpfBlendFuncSeparatei =
      (PFNGLBLENDFUNCSEPARATEI_PROC *)get_proc("glBlendFuncSeparatei");
  glpfDrawArraysIndirect =
      (PFNGLDRAWARRAYSINDIRECT_PROC *)get_proc("glDrawArraysIndirect");
  glpfDrawElementsIndirect =
      (PFNGLDRAWELEMENTSINDIRECT_PROC *)get_proc("glDrawElementsIndirect");
  glpfUniform1d = (PFNGLUNIFORM1D_PROC *)get_proc("glUniform1d");
  glpfUniform2d = (PFNGLUNIFORM2D_PROC *)get_proc("glUniform2d");
  glpfUniform3d = (PFNGLUNIFORM3D_PROC *)get_proc("glUniform3d");
  glpfUniform4d = (PFNGLUNIFORM4D_PROC *)get_proc("glUniform4d");
  glpfUniform1dv = (PFNGLUNIFORM1DV_PROC *)get_proc("glUniform1dv");
  glpfUniform2dv = (PFNGLUNIFORM2DV_PROC *)get_proc("glUniform2dv");
  glpfUniform3dv = (PFNGLUNIFORM3DV_PROC *)get_proc("glUniform3dv");
  glpfUniform4dv = (PFNGLUNIFORM4DV_PROC *)get_proc("glUniform4dv");
  glpfUniformMatrix2dv =
      (PFNGLUNIFORMMATRIX2DV_PROC *)get_proc("glUniformMatrix2dv");
  glpfUniformMatrix3dv =
      (PFNGLUNIFORMMATRIX3DV_PROC *)get_proc("glUniformMatrix3dv");
  glpfUniformMatrix4dv =
      (PFNGLUNIFORMMATRIX4DV_PROC *)get_proc("glUniformMatrix4dv");
  glpfUniformMatrix2x3dv =
      (PFNGLUNIFORMMATRIX2X3DV_PROC *)get_proc("glUniformMatrix2x3dv");
  glpfUniformMatrix2x4dv =
      (PFNGLUNIFORMMATRIX2X4DV_PROC *)get_proc("glUniformMatrix2x4dv");
  glpfUniformMatrix3x2dv =
      (PFNGLUNIFORMMATRIX3X2DV_PROC *)get_proc("glUniformMatrix3x2dv");
  glpfUniformMatrix3x4dv =
      (PFNGLUNIFORMMATRIX3X4DV_PROC *)get_proc("glUniformMatrix3x4dv");
  glpfUniformMatrix4x2dv =
      (PFNGLUNIFORMMATRIX4X2DV_PROC *)get_proc("glUniformMatrix4x2dv");
  glpfUniformMatrix4x3dv =
      (PFNGLUNIFORMMATRIX4X3DV_PROC *)get_proc("glUniformMatrix4x3dv");
  glpfGetUniformdv = (PFNGLGETUNIFORMDV_PROC *)get_proc("glGetUniformdv");
  glpfGetSubroutineUniformLocation =
      (PFNGLGETSUBROUTINEUNIFORMLOCATION_PROC *)get_proc(
          "glGetSubroutineUniformLocation");
  glpfGetSubroutineIndex =
      (PFNGLGETSUBROUTINEINDEX_PROC *)get_proc("glGetSubroutineIndex");
  glpfGetActiveSubroutineUniformiv =
      (PFNGLGETACTIVESUBROUTINEUNIFORMIV_PROC *)get_proc(
          "glGetActiveSubroutineUniformiv");
  glpfGetActiveSubroutineUniformName =
      (PFNGLGETACTIVESUBROUTINEUNIFORMNAME_PROC *)get_proc(
          "glGetActiveSubroutineUniformName");
  glpfGetActiveSubroutineName = (PFNGLGETACTIVESUBROUTINENAME_PROC *)get_proc(
      "glGetActiveSubroutineName");
  glpfUniformSubroutinesuiv =
      (PFNGLUNIFORMSUBROUTINESUIV_PROC *)get_proc("glUniformSubroutinesuiv");
  glpfGetUniformSubroutineuiv = (PFNGLGETUNIFORMSUBROUTINEUIV_PROC *)get_proc(
      "glGetUniformSubroutineuiv");
  glpfGetProgramStageiv =
      (PFNGLGETPROGRAMSTAGEIV_PROC *)get_proc("glGetProgramStageiv");
  glpfPatchParameteri =
      (PFNGLPATCHPARAMETERI_PROC *)get_proc("glPatchParameteri");
  glpfPatchParameterfv =
      (PFNGLPATCHPARAMETERFV_PROC *)get_proc("glPatchParameterfv");
  glpfBindTransformFeedback =
      (PFNGLBINDTRANSFORMFEEDBACK_PROC *)get_proc("glBindTransformFeedback");
  glpfDeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKS_PROC *)get_proc(
      "glDeleteTransformFeedbacks");
  glpfGenTransformFeedbacks =
      (PFNGLGENTRANSFORMFEEDBACKS_PROC *)get_proc("glGenTransformFeedbacks");
  glpfIsTransformFeedback =
      (PFNGLISTRANSFORMFEEDBACK_PROC *)get_proc("glIsTransformFeedback");
  glpfPauseTransformFeedback =
      (PFNGLPAUSETRANSFORMFEEDBACK_PROC *)get_proc("glPauseTransformFeedback");
  glpfResumeTransformFeedback = (PFNGLRESUMETRANSFORMFEEDBACK_PROC *)get_proc(
      "glResumeTransformFeedback");
  glpfDrawTransformFeedback =
      (PFNGLDRAWTRANSFORMFEEDBACK_PROC *)get_proc("glDrawTransformFeedback");
  glpfDrawTransformFeedbackStream =
      (PFNGLDRAWTRANSFORMFEEDBACKSTREAM_PROC *)get_proc(
          "glDrawTransformFeedbackStream");
  glpfBeginQueryIndexed =
      (PFNGLBEGINQUERYINDEXED_PROC *)get_proc("glBeginQueryIndexed");
  glpfEndQueryIndexed =
      (PFNGLENDQUERYINDEXED_PROC *)get_proc("glEndQueryIndexed");
  glpfGetQueryIndexediv =
      (PFNGLGETQUERYINDEXEDIV_PROC *)get_proc("glGetQueryIndexediv");

  /* GL_VERSION_4_1 */

  glpfReleaseShaderCompiler =
      (PFNGLRELEASESHADERCOMPILER_PROC *)get_proc("glReleaseShaderCompiler");
  glpfShaderBinary = (PFNGLSHADERBINARY_PROC *)get_proc("glShaderBinary");
  glpfGetShaderPrecisionFormat = (PFNGLGETSHADERPRECISIONFORMAT_PROC *)get_proc(
      "glGetShaderPrecisionFormat");
  glpfDepthRangef = (PFNGLDEPTHRANGEF_PROC *)get_proc("glDepthRangef");
  glpfClearDepthf = (PFNGLCLEARDEPTHF_PROC *)get_proc("glClearDepthf");
  glpfGetProgramBinary =
      (PFNGLGETPROGRAMBINARY_PROC *)get_proc("glGetProgramBinary");
  glpfProgramBinary = (PFNGLPROGRAMBINARY_PROC *)get_proc("glProgramBinary");
  glpfProgramParameteri =
      (PFNGLPROGRAMPARAMETERI_PROC *)get_proc("glProgramParameteri");
  glpfUseProgramStages =
      (PFNGLUSEPROGRAMSTAGES_PROC *)get_proc("glUseProgramStages");
  glpfActiveShaderProgram =
      (PFNGLACTIVESHADERPROGRAM_PROC *)get_proc("glActiveShaderProgram");
  glpfCreateShaderProgramv =
      (PFNGLCREATESHADERPROGRAMV_PROC *)get_proc("glCreateShaderProgramv");
  glpfBindProgramPipeline =
      (PFNGLBINDPROGRAMPIPELINE_PROC *)get_proc("glBindProgramPipeline");
  glpfDeleteProgramPipelines =
      (PFNGLDELETEPROGRAMPIPELINES_PROC *)get_proc("glDeleteProgramPipelines");
  glpfGenProgramPipelines =
      (PFNGLGENPROGRAMPIPELINES_PROC *)get_proc("glGenProgramPipelines");
  glpfIsProgramPipeline =
      (PFNGLISPROGRAMPIPELINE_PROC *)get_proc("glIsProgramPipeline");
  glpfGetProgramPipelineiv =
      (PFNGLGETPROGRAMPIPELINEIV_PROC *)get_proc("glGetProgramPipelineiv");
  glpfProgramUniform1i =
      (PFNGLPROGRAMUNIFORM1I_PROC *)get_proc("glProgramUniform1i");
  glpfProgramUniform1iv =
      (PFNGLPROGRAMUNIFORM1IV_PROC *)get_proc("glProgramUniform1iv");
  glpfProgramUniform1f =
      (PFNGLPROGRAMUNIFORM1F_PROC *)get_proc("glProgramUniform1f");
  glpfProgramUniform1fv =
      (PFNGLPROGRAMUNIFORM1FV_PROC *)get_proc("glProgramUniform1fv");
  glpfProgramUniform1d =
      (PFNGLPROGRAMUNIFORM1D_PROC *)get_proc("glProgramUniform1d");
  glpfProgramUniform1dv =
      (PFNGLPROGRAMUNIFORM1DV_PROC *)get_proc("glProgramUniform1dv");
  glpfProgramUniform1ui =
      (PFNGLPROGRAMUNIFORM1UI_PROC *)get_proc("glProgramUniform1ui");
  glpfProgramUniform1uiv =
      (PFNGLPROGRAMUNIFORM1UIV_PROC *)get_proc("glProgramUniform1uiv");
  glpfProgramUniform2i =
      (PFNGLPROGRAMUNIFORM2I_PROC *)get_proc("glProgramUniform2i");
  glpfProgramUniform2iv =
      (PFNGLPROGRAMUNIFORM2IV_PROC *)get_proc("glProgramUniform2iv");
  glpfProgramUniform2f =
      (PFNGLPROGRAMUNIFORM2F_PROC *)get_proc("glProgramUniform2f");
  glpfProgramUniform2fv =
      (PFNGLPROGRAMUNIFORM2FV_PROC *)get_proc("glProgramUniform2fv");
  glpfProgramUniform2d =
      (PFNGLPROGRAMUNIFORM2D_PROC *)get_proc("glProgramUniform2d");
  glpfProgramUniform2dv =
      (PFNGLPROGRAMUNIFORM2DV_PROC *)get_proc("glProgramUniform2dv");
  glpfProgramUniform2ui =
      (PFNGLPROGRAMUNIFORM2UI_PROC *)get_proc("glProgramUniform2ui");
  glpfProgramUniform2uiv =
      (PFNGLPROGRAMUNIFORM2UIV_PROC *)get_proc("glProgramUniform2uiv");
  glpfProgramUniform3i =
      (PFNGLPROGRAMUNIFORM3I_PROC *)get_proc("glProgramUniform3i");
  glpfProgramUniform3iv =
      (PFNGLPROGRAMUNIFORM3IV_PROC *)get_proc("glProgramUniform3iv");
  glpfProgramUniform3f =
      (PFNGLPROGRAMUNIFORM3F_PROC *)get_proc("glProgramUniform3f");
  glpfProgramUniform3fv =
      (PFNGLPROGRAMUNIFORM3FV_PROC *)get_proc("glProgramUniform3fv");
  glpfProgramUniform3d =
      (PFNGLPROGRAMUNIFORM3D_PROC *)get_proc("glProgramUniform3d");
  glpfProgramUniform3dv =
      (PFNGLPROGRAMUNIFORM3DV_PROC *)get_proc("glProgramUniform3dv");
  glpfProgramUniform3ui =
      (PFNGLPROGRAMUNIFORM3UI_PROC *)get_proc("glProgramUniform3ui");
  glpfProgramUniform3uiv =
      (PFNGLPROGRAMUNIFORM3UIV_PROC *)get_proc("glProgramUniform3uiv");
  glpfProgramUniform4i =
      (PFNGLPROGRAMUNIFORM4I_PROC *)get_proc("glProgramUniform4i");
  glpfProgramUniform4iv =
      (PFNGLPROGRAMUNIFORM4IV_PROC *)get_proc("glProgramUniform4iv");
  glpfProgramUniform4f =
      (PFNGLPROGRAMUNIFORM4F_PROC *)get_proc("glProgramUniform4f");
  glpfProgramUniform4fv =
      (PFNGLPROGRAMUNIFORM4FV_PROC *)get_proc("glProgramUniform4fv");
  glpfProgramUniform4d =
      (PFNGLPROGRAMUNIFORM4D_PROC *)get_proc("glProgramUniform4d");
  glpfProgramUniform4dv =
      (PFNGLPROGRAMUNIFORM4DV_PROC *)get_proc("glProgramUniform4dv");
  glpfProgramUniform4ui =
      (PFNGLPROGRAMUNIFORM4UI_PROC *)get_proc("glProgramUniform4ui");
  glpfProgramUniform4uiv =
      (PFNGLPROGRAMUNIFORM4UIV_PROC *)get_proc("glProgramUniform4uiv");
  glpfProgramUniformMatrix2fv = (PFNGLPROGRAMUNIFORMMATRIX2FV_PROC *)get_proc(
      "glProgramUniformMatrix2fv");
  glpfProgramUniformMatrix3fv = (PFNGLPROGRAMUNIFORMMATRIX3FV_PROC *)get_proc(
      "glProgramUniformMatrix3fv");
  glpfProgramUniformMatrix4fv = (PFNGLPROGRAMUNIFORMMATRIX4FV_PROC *)get_proc(
      "glProgramUniformMatrix4fv");
  glpfProgramUniformMatrix2dv = (PFNGLPROGRAMUNIFORMMATRIX2DV_PROC *)get_proc(
      "glProgramUniformMatrix2dv");
  glpfProgramUniformMatrix3dv = (PFNGLPROGRAMUNIFORMMATRIX3DV_PROC *)get_proc(
      "glProgramUniformMatrix3dv");
  glpfProgramUniformMatrix4dv = (PFNGLPROGRAMUNIFORMMATRIX4DV_PROC *)get_proc(
      "glProgramUniformMatrix4dv");
  glpfProgramUniformMatrix2x3fv =
      (PFNGLPROGRAMUNIFORMMATRIX2X3FV_PROC *)get_proc(
          "glProgramUniformMatrix2x3fv");
  glpfProgramUniformMatrix3x2fv =
      (PFNGLPROGRAMUNIFORMMATRIX3X2FV_PROC *)get_proc(
          "glProgramUniformMatrix3x2fv");
  glpfProgramUniformMatrix2x4fv =
      (PFNGLPROGRAMUNIFORMMATRIX2X4FV_PROC *)get_proc(
          "glProgramUniformMatrix2x4fv");
  glpfProgramUniformMatrix4x2fv =
      (PFNGLPROGRAMUNIFORMMATRIX4X2FV_PROC *)get_proc(
          "glProgramUniformMatrix4x2fv");
  glpfProgramUniformMatrix3x4fv =
      (PFNGLPROGRAMUNIFORMMATRIX3X4FV_PROC *)get_proc(
          "glProgramUniformMatrix3x4fv");
  glpfProgramUniformMatrix4x3fv =
      (PFNGLPROGRAMUNIFORMMATRIX4X3FV_PROC *)get_proc(
          "glProgramUniformMatrix4x3fv");
  glpfProgramUniformMatrix2x3dv =
      (PFNGLPROGRAMUNIFORMMATRIX2X3DV_PROC *)get_proc(
          "glProgramUniformMatrix2x3dv");
  glpfProgramUniformMatrix3x2dv =
      (PFNGLPROGRAMUNIFORMMATRIX3X2DV_PROC *)get_proc(
          "glProgramUniformMatrix3x2dv");
  glpfProgramUniformMatrix2x4dv =
      (PFNGLPROGRAMUNIFORMMATRIX2X4DV_PROC *)get_proc(
          "glProgramUniformMatrix2x4dv");
  glpfProgramUniformMatrix4x2dv =
      (PFNGLPROGRAMUNIFORMMATRIX4X2DV_PROC *)get_proc(
          "glProgramUniformMatrix4x2dv");
  glpfProgramUniformMatrix3x4dv =
      (PFNGLPROGRAMUNIFORMMATRIX3X4DV_PROC *)get_proc(
          "glProgramUniformMatrix3x4dv");
  glpfProgramUniformMatrix4x3dv =
      (PFNGLPROGRAMUNIFORMMATRIX4X3DV_PROC *)get_proc(
          "glProgramUniformMatrix4x3dv");
  glpfValidateProgramPipeline = (PFNGLVALIDATEPROGRAMPIPELINE_PROC *)get_proc(
      "glValidateProgramPipeline");
  glpfGetProgramPipelineInfoLog =
      (PFNGLGETPROGRAMPIPELINEINFOLOG_PROC *)get_proc(
          "glGetProgramPipelineInfoLog");
  glpfVertexAttribL1d =
      (PFNGLVERTEXATTRIBL1D_PROC *)get_proc("glVertexAttribL1d");
  glpfVertexAttribL2d =
      (PFNGLVERTEXATTRIBL2D_PROC *)get_proc("glVertexAttribL2d");
  glpfVertexAttribL3d =
      (PFNGLVERTEXATTRIBL3D_PROC *)get_proc("glVertexAttribL3d");
  glpfVertexAttribL4d =
      (PFNGLVERTEXATTRIBL4D_PROC *)get_proc("glVertexAttribL4d");
  glpfVertexAttribL1dv =
      (PFNGLVERTEXATTRIBL1DV_PROC *)get_proc("glVertexAttribL1dv");
  glpfVertexAttribL2dv =
      (PFNGLVERTEXATTRIBL2DV_PROC *)get_proc("glVertexAttribL2dv");
  glpfVertexAttribL3dv =
      (PFNGLVERTEXATTRIBL3DV_PROC *)get_proc("glVertexAttribL3dv");
  glpfVertexAttribL4dv =
      (PFNGLVERTEXATTRIBL4DV_PROC *)get_proc("glVertexAttribL4dv");
  glpfVertexAttribLPointer =
      (PFNGLVERTEXATTRIBLPOINTER_PROC *)get_proc("glVertexAttribLPointer");
  glpfGetVertexAttribLdv =
      (PFNGLGETVERTEXATTRIBLDV_PROC *)get_proc("glGetVertexAttribLdv");
  glpfViewportArrayv = (PFNGLVIEWPORTARRAYV_PROC *)get_proc("glViewportArrayv");
  glpfViewportIndexedf =
      (PFNGLVIEWPORTINDEXEDF_PROC *)get_proc("glViewportIndexedf");
  glpfViewportIndexedfv =
      (PFNGLVIEWPORTINDEXEDFV_PROC *)get_proc("glViewportIndexedfv");
  glpfScissorArrayv = (PFNGLSCISSORARRAYV_PROC *)get_proc("glScissorArrayv");
  glpfScissorIndexed = (PFNGLSCISSORINDEXED_PROC *)get_proc("glScissorIndexed");
  glpfScissorIndexedv =
      (PFNGLSCISSORINDEXEDV_PROC *)get_proc("glScissorIndexedv");
  glpfDepthRangeArrayv =
      (PFNGLDEPTHRANGEARRAYV_PROC *)get_proc("glDepthRangeArrayv");
  glpfDepthRangeIndexed =
      (PFNGLDEPTHRANGEINDEXED_PROC *)get_proc("glDepthRangeIndexed");
  glpfGetFloati_v = (PFNGLGETFLOATI_V_PROC *)get_proc("glGetFloati_v");
  glpfGetDoublei_v = (PFNGLGETDOUBLEI_V_PROC *)get_proc("glGetDoublei_v");

  /* GL_VERSION_4_2 */

  glpfDrawArraysInstancedBaseInstance =
      (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCE_PROC *)get_proc(
          "glDrawArraysInstancedBaseInstance");
  glpfDrawElementsInstancedBaseInstance =
      (PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCE_PROC *)get_proc(
          "glDrawElementsInstancedBaseInstance");
  glpfDrawElementsInstancedBaseVertexBaseInstance =
      (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCE_PROC *)get_proc(
          "glDrawElementsInstancedBaseVertexBaseInstance");
  glpfGetInternalformativ =
      (PFNGLGETINTERNALFORMATIV_PROC *)get_proc("glGetInternalformativ");
  glpfGetActiveAtomicCounterBufferiv =
      (PFNGLGETACTIVEATOMICCOUNTERBUFFERIV_PROC *)get_proc(
          "glGetActiveAtomicCounterBufferiv");
  glpfBindImageTexture =
      (PFNGLBINDIMAGETEXTURE_PROC *)get_proc("glBindImageTexture");
  glpfMemoryBarrier = (PFNGLMEMORYBARRIER_PROC *)get_proc("glMemoryBarrier");
  glpfTexStorage1D = (PFNGLTEXSTORAGE1D_PROC *)get_proc("glTexStorage1D");
  glpfTexStorage2D = (PFNGLTEXSTORAGE2D_PROC *)get_proc("glTexStorage2D");
  glpfTexStorage3D = (PFNGLTEXSTORAGE3D_PROC *)get_proc("glTexStorage3D");
  glpfDrawTransformFeedbackInstanced =
      (PFNGLDRAWTRANSFORMFEEDBACKINSTANCED_PROC *)get_proc(
          "glDrawTransformFeedbackInstanced");
  glpfDrawTransformFeedbackStreamInstanced =
      (PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCED_PROC *)get_proc(
          "glDrawTransformFeedbackStreamInstanced");

  /* GL_VERSION_4_3 */

  glpfClearBufferData =
      (PFNGLCLEARBUFFERDATA_PROC *)get_proc("glClearBufferData");
  glpfClearBufferSubData =
      (PFNGLCLEARBUFFERSUBDATA_PROC *)get_proc("glClearBufferSubData");
  glpfDispatchCompute =
      (PFNGLDISPATCHCOMPUTE_PROC *)get_proc("glDispatchCompute");
  glpfDispatchComputeIndirect = (PFNGLDISPATCHCOMPUTEINDIRECT_PROC *)get_proc(
      "glDispatchComputeIndirect");
  glpfCopyImageSubData =
      (PFNGLCOPYIMAGESUBDATA_PROC *)get_proc("glCopyImageSubData");
  glpfFramebufferParameteri =
      (PFNGLFRAMEBUFFERPARAMETERI_PROC *)get_proc("glFramebufferParameteri");
  glpfGetFramebufferParameteriv =
      (PFNGLGETFRAMEBUFFERPARAMETERIV_PROC *)get_proc(
          "glGetFramebufferParameteriv");
  glpfGetInternalformati64v =
      (PFNGLGETINTERNALFORMATI64V_PROC *)get_proc("glGetInternalformati64v");
  glpfInvalidateTexSubImage =
      (PFNGLINVALIDATETEXSUBIMAGE_PROC *)get_proc("glInvalidateTexSubImage");
  glpfInvalidateTexImage =
      (PFNGLINVALIDATETEXIMAGE_PROC *)get_proc("glInvalidateTexImage");
  glpfInvalidateBufferSubData = (PFNGLINVALIDATEBUFFERSUBDATA_PROC *)get_proc(
      "glInvalidateBufferSubData");
  glpfInvalidateBufferData =
      (PFNGLINVALIDATEBUFFERDATA_PROC *)get_proc("glInvalidateBufferData");
  glpfInvalidateFramebuffer =
      (PFNGLINVALIDATEFRAMEBUFFER_PROC *)get_proc("glInvalidateFramebuffer");
  glpfInvalidateSubFramebuffer = (PFNGLINVALIDATESUBFRAMEBUFFER_PROC *)get_proc(
      "glInvalidateSubFramebuffer");
  glpfMultiDrawArraysIndirect = (PFNGLMULTIDRAWARRAYSINDIRECT_PROC *)get_proc(
      "glMultiDrawArraysIndirect");
  glpfMultiDrawElementsIndirect =
      (PFNGLMULTIDRAWELEMENTSINDIRECT_PROC *)get_proc(
          "glMultiDrawElementsIndirect");
  glpfGetProgramInterfaceiv =
      (PFNGLGETPROGRAMINTERFACEIV_PROC *)get_proc("glGetProgramInterfaceiv");
  glpfGetProgramResourceIndex = (PFNGLGETPROGRAMRESOURCEINDEX_PROC *)get_proc(
      "glGetProgramResourceIndex");
  glpfGetProgramResourceName =
      (PFNGLGETPROGRAMRESOURCENAME_PROC *)get_proc("glGetProgramResourceName");
  glpfGetProgramResourceiv =
      (PFNGLGETPROGRAMRESOURCEIV_PROC *)get_proc("glGetProgramResourceiv");
  glpfGetProgramResourceLocation =
      (PFNGLGETPROGRAMRESOURCELOCATION_PROC *)get_proc(
          "glGetProgramResourceLocation");
  glpfGetProgramResourceLocationIndex =
      (PFNGLGETPROGRAMRESOURCELOCATIONINDEX_PROC *)get_proc(
          "glGetProgramResourceLocationIndex");
  glpfShaderStorageBlockBinding =
      (PFNGLSHADERSTORAGEBLOCKBINDING_PROC *)get_proc(
          "glShaderStorageBlockBinding");
  glpfTexBufferRange = (PFNGLTEXBUFFERRANGE_PROC *)get_proc("glTexBufferRange");
  glpfTexStorage2DMultisample = (PFNGLTEXSTORAGE2DMULTISAMPLE_PROC *)get_proc(
      "glTexStorage2DMultisample");
  glpfTexStorage3DMultisample = (PFNGLTEXSTORAGE3DMULTISAMPLE_PROC *)get_proc(
      "glTexStorage3DMultisample");
  glpfTextureView = (PFNGLTEXTUREVIEW_PROC *)get_proc("glTextureView");
  glpfBindVertexBuffer =
      (PFNGLBINDVERTEXBUFFER_PROC *)get_proc("glBindVertexBuffer");
  glpfVertexAttribFormat =
      (PFNGLVERTEXATTRIBFORMAT_PROC *)get_proc("glVertexAttribFormat");
  glpfVertexAttribIFormat =
      (PFNGLVERTEXATTRIBIFORMAT_PROC *)get_proc("glVertexAttribIFormat");
  glpfVertexAttribLFormat =
      (PFNGLVERTEXATTRIBLFORMAT_PROC *)get_proc("glVertexAttribLFormat");
  glpfVertexAttribBinding =
      (PFNGLVERTEXATTRIBBINDING_PROC *)get_proc("glVertexAttribBinding");
  glpfVertexBindingDivisor =
      (PFNGLVERTEXBINDINGDIVISOR_PROC *)get_proc("glVertexBindingDivisor");
  glpfDebugMessageControl =
      (PFNGLDEBUGMESSAGECONTROL_PROC *)get_proc("glDebugMessageControl");
  glpfDebugMessageInsert =
      (PFNGLDEBUGMESSAGEINSERT_PROC *)get_proc("glDebugMessageInsert");
  glpfDebugMessageCallback =
      (PFNGLDEBUGMESSAGECALLBACK_PROC *)get_proc("glDebugMessageCallback");
  glpfGetDebugMessageLog =
      (PFNGLGETDEBUGMESSAGELOG_PROC *)get_proc("glGetDebugMessageLog");
  glpfPushDebugGroup = (PFNGLPUSHDEBUGGROUP_PROC *)get_proc("glPushDebugGroup");
  glpfPopDebugGroup = (PFNGLPOPDEBUGGROUP_PROC *)get_proc("glPopDebugGroup");
  glpfObjectLabel = (PFNGLOBJECTLABEL_PROC *)get_proc("glObjectLabel");
  glpfGetObjectLabel = (PFNGLGETOBJECTLABEL_PROC *)get_proc("glGetObjectLabel");
  glpfObjectPtrLabel = (PFNGLOBJECTPTRLABEL_PROC *)get_proc("glObjectPtrLabel");
  glpfGetObjectPtrLabel =
      (PFNGLGETOBJECTPTRLABEL_PROC *)get_proc("glGetObjectPtrLabel");
  glpfGetPointerv = (PFNGLGETPOINTERV_PROC *)get_proc("glGetPointerv");
}

/* ----------------------- Extension flag definitions ---------------------- */

/* ---------------------- Function pointer definitions --------------------- */

/* GL_VERSION_1_2 */

PFNGLDRAWRANGEELEMENTS_PROC *glpfDrawRangeElements = NULL;
PFNGLTEXIMAGE3D_PROC *glpfTexImage3D = NULL;
PFNGLTEXSUBIMAGE3D_PROC *glpfTexSubImage3D = NULL;
PFNGLCOPYTEXSUBIMAGE3D_PROC *glpfCopyTexSubImage3D = NULL;

/* GL_VERSION_1_3 */

PFNGLACTIVETEXTURE_PROC *glpfActiveTexture = NULL;
PFNGLSAMPLECOVERAGE_PROC *glpfSampleCoverage = NULL;
PFNGLCOMPRESSEDTEXIMAGE3D_PROC *glpfCompressedTexImage3D = NULL;
PFNGLCOMPRESSEDTEXIMAGE2D_PROC *glpfCompressedTexImage2D = NULL;
PFNGLCOMPRESSEDTEXIMAGE1D_PROC *glpfCompressedTexImage1D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3D_PROC *glpfCompressedTexSubImage3D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2D_PROC *glpfCompressedTexSubImage2D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1D_PROC *glpfCompressedTexSubImage1D = NULL;
PFNGLGETCOMPRESSEDTEXIMAGE_PROC *glpfGetCompressedTexImage = NULL;

/* GL_VERSION_1_4 */

PFNGLBLENDFUNCSEPARATE_PROC *glpfBlendFuncSeparate = NULL;
PFNGLMULTIDRAWARRAYS_PROC *glpfMultiDrawArrays = NULL;
PFNGLMULTIDRAWELEMENTS_PROC *glpfMultiDrawElements = NULL;
PFNGLPOINTPARAMETERF_PROC *glpfPointParameterf = NULL;
PFNGLPOINTPARAMETERFV_PROC *glpfPointParameterfv = NULL;
PFNGLPOINTPARAMETERI_PROC *glpfPointParameteri = NULL;
PFNGLPOINTPARAMETERIV_PROC *glpfPointParameteriv = NULL;
PFNGLBLENDCOLOR_PROC *glpfBlendColor = NULL;
PFNGLBLENDEQUATION_PROC *glpfBlendEquation = NULL;

/* GL_VERSION_1_5 */

PFNGLGENQUERIES_PROC *glpfGenQueries = NULL;
PFNGLDELETEQUERIES_PROC *glpfDeleteQueries = NULL;
PFNGLISQUERY_PROC *glpfIsQuery = NULL;
PFNGLBEGINQUERY_PROC *glpfBeginQuery = NULL;
PFNGLENDQUERY_PROC *glpfEndQuery = NULL;
PFNGLGETQUERYIV_PROC *glpfGetQueryiv = NULL;
PFNGLGETQUERYOBJECTIV_PROC *glpfGetQueryObjectiv = NULL;
PFNGLGETQUERYOBJECTUIV_PROC *glpfGetQueryObjectuiv = NULL;
PFNGLBINDBUFFER_PROC *glpfBindBuffer = NULL;
PFNGLDELETEBUFFERS_PROC *glpfDeleteBuffers = NULL;
PFNGLGENBUFFERS_PROC *glpfGenBuffers = NULL;
PFNGLISBUFFER_PROC *glpfIsBuffer = NULL;
PFNGLBUFFERDATA_PROC *glpfBufferData = NULL;
PFNGLBUFFERSUBDATA_PROC *glpfBufferSubData = NULL;
PFNGLGETBUFFERSUBDATA_PROC *glpfGetBufferSubData = NULL;
PFNGLMAPBUFFER_PROC *glpfMapBuffer = NULL;
PFNGLUNMAPBUFFER_PROC *glpfUnmapBuffer = NULL;
PFNGLGETBUFFERPARAMETERIV_PROC *glpfGetBufferParameteriv = NULL;
PFNGLGETBUFFERPOINTERV_PROC *glpfGetBufferPointerv = NULL;

/* GL_VERSION_2_0 */

PFNGLBLENDEQUATIONSEPARATE_PROC *glpfBlendEquationSeparate = NULL;
PFNGLDRAWBUFFERS_PROC *glpfDrawBuffers = NULL;
PFNGLSTENCILOPSEPARATE_PROC *glpfStencilOpSeparate = NULL;
PFNGLSTENCILFUNCSEPARATE_PROC *glpfStencilFuncSeparate = NULL;
PFNGLSTENCILMASKSEPARATE_PROC *glpfStencilMaskSeparate = NULL;
PFNGLATTACHSHADER_PROC *glpfAttachShader = NULL;
PFNGLBINDATTRIBLOCATION_PROC *glpfBindAttribLocation = NULL;
PFNGLCOMPILESHADER_PROC *glpfCompileShader = NULL;
PFNGLCREATEPROGRAM_PROC *glpfCreateProgram = NULL;
PFNGLCREATESHADER_PROC *glpfCreateShader = NULL;
PFNGLDELETEPROGRAM_PROC *glpfDeleteProgram = NULL;
PFNGLDELETESHADER_PROC *glpfDeleteShader = NULL;
PFNGLDETACHSHADER_PROC *glpfDetachShader = NULL;
PFNGLDISABLEVERTEXATTRIBARRAY_PROC *glpfDisableVertexAttribArray = NULL;
PFNGLENABLEVERTEXATTRIBARRAY_PROC *glpfEnableVertexAttribArray = NULL;
PFNGLGETACTIVEATTRIB_PROC *glpfGetActiveAttrib = NULL;
PFNGLGETACTIVEUNIFORM_PROC *glpfGetActiveUniform = NULL;
PFNGLGETATTACHEDSHADERS_PROC *glpfGetAttachedShaders = NULL;
PFNGLGETATTRIBLOCATION_PROC *glpfGetAttribLocation = NULL;
PFNGLGETPROGRAMIV_PROC *glpfGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOG_PROC *glpfGetProgramInfoLog = NULL;
PFNGLGETSHADERIV_PROC *glpfGetShaderiv = NULL;
PFNGLGETSHADERINFOLOG_PROC *glpfGetShaderInfoLog = NULL;
PFNGLGETSHADERSOURCE_PROC *glpfGetShaderSource = NULL;
PFNGLGETUNIFORMLOCATION_PROC *glpfGetUniformLocation = NULL;
PFNGLGETUNIFORMFV_PROC *glpfGetUniformfv = NULL;
PFNGLGETUNIFORMIV_PROC *glpfGetUniformiv = NULL;
PFNGLGETVERTEXATTRIBDV_PROC *glpfGetVertexAttribdv = NULL;
PFNGLGETVERTEXATTRIBFV_PROC *glpfGetVertexAttribfv = NULL;
PFNGLGETVERTEXATTRIBIV_PROC *glpfGetVertexAttribiv = NULL;
PFNGLGETVERTEXATTRIBPOINTERV_PROC *glpfGetVertexAttribPointerv = NULL;
PFNGLISPROGRAM_PROC *glpfIsProgram = NULL;
PFNGLISSHADER_PROC *glpfIsShader = NULL;
PFNGLLINKPROGRAM_PROC *glpfLinkProgram = NULL;
PFNGLSHADERSOURCE_PROC *glpfShaderSource = NULL;
PFNGLUSEPROGRAM_PROC *glpfUseProgram = NULL;
PFNGLUNIFORM1F_PROC *glpfUniform1f = NULL;
PFNGLUNIFORM2F_PROC *glpfUniform2f = NULL;
PFNGLUNIFORM3F_PROC *glpfUniform3f = NULL;
PFNGLUNIFORM4F_PROC *glpfUniform4f = NULL;
PFNGLUNIFORM1I_PROC *glpfUniform1i = NULL;
PFNGLUNIFORM2I_PROC *glpfUniform2i = NULL;
PFNGLUNIFORM3I_PROC *glpfUniform3i = NULL;
PFNGLUNIFORM4I_PROC *glpfUniform4i = NULL;
PFNGLUNIFORM1FV_PROC *glpfUniform1fv = NULL;
PFNGLUNIFORM2FV_PROC *glpfUniform2fv = NULL;
PFNGLUNIFORM3FV_PROC *glpfUniform3fv = NULL;
PFNGLUNIFORM4FV_PROC *glpfUniform4fv = NULL;
PFNGLUNIFORM1IV_PROC *glpfUniform1iv = NULL;
PFNGLUNIFORM2IV_PROC *glpfUniform2iv = NULL;
PFNGLUNIFORM3IV_PROC *glpfUniform3iv = NULL;
PFNGLUNIFORM4IV_PROC *glpfUniform4iv = NULL;
PFNGLUNIFORMMATRIX2FV_PROC *glpfUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX3FV_PROC *glpfUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX4FV_PROC *glpfUniformMatrix4fv = NULL;
PFNGLVALIDATEPROGRAM_PROC *glpfValidateProgram = NULL;
PFNGLVERTEXATTRIB1D_PROC *glpfVertexAttrib1d = NULL;
PFNGLVERTEXATTRIB1DV_PROC *glpfVertexAttrib1dv = NULL;
PFNGLVERTEXATTRIB1F_PROC *glpfVertexAttrib1f = NULL;
PFNGLVERTEXATTRIB1FV_PROC *glpfVertexAttrib1fv = NULL;
PFNGLVERTEXATTRIB1S_PROC *glpfVertexAttrib1s = NULL;
PFNGLVERTEXATTRIB1SV_PROC *glpfVertexAttrib1sv = NULL;
PFNGLVERTEXATTRIB2D_PROC *glpfVertexAttrib2d = NULL;
PFNGLVERTEXATTRIB2DV_PROC *glpfVertexAttrib2dv = NULL;
PFNGLVERTEXATTRIB2F_PROC *glpfVertexAttrib2f = NULL;
PFNGLVERTEXATTRIB2FV_PROC *glpfVertexAttrib2fv = NULL;
PFNGLVERTEXATTRIB2S_PROC *glpfVertexAttrib2s = NULL;
PFNGLVERTEXATTRIB2SV_PROC *glpfVertexAttrib2sv = NULL;
PFNGLVERTEXATTRIB3D_PROC *glpfVertexAttrib3d = NULL;
PFNGLVERTEXATTRIB3DV_PROC *glpfVertexAttrib3dv = NULL;
PFNGLVERTEXATTRIB3F_PROC *glpfVertexAttrib3f = NULL;
PFNGLVERTEXATTRIB3FV_PROC *glpfVertexAttrib3fv = NULL;
PFNGLVERTEXATTRIB3S_PROC *glpfVertexAttrib3s = NULL;
PFNGLVERTEXATTRIB3SV_PROC *glpfVertexAttrib3sv = NULL;
PFNGLVERTEXATTRIB4NBV_PROC *glpfVertexAttrib4Nbv = NULL;
PFNGLVERTEXATTRIB4NIV_PROC *glpfVertexAttrib4Niv = NULL;
PFNGLVERTEXATTRIB4NSV_PROC *glpfVertexAttrib4Nsv = NULL;
PFNGLVERTEXATTRIB4NUB_PROC *glpfVertexAttrib4Nub = NULL;
PFNGLVERTEXATTRIB4NUBV_PROC *glpfVertexAttrib4Nubv = NULL;
PFNGLVERTEXATTRIB4NUIV_PROC *glpfVertexAttrib4Nuiv = NULL;
PFNGLVERTEXATTRIB4NUSV_PROC *glpfVertexAttrib4Nusv = NULL;
PFNGLVERTEXATTRIB4BV_PROC *glpfVertexAttrib4bv = NULL;
PFNGLVERTEXATTRIB4D_PROC *glpfVertexAttrib4d = NULL;
PFNGLVERTEXATTRIB4DV_PROC *glpfVertexAttrib4dv = NULL;
PFNGLVERTEXATTRIB4F_PROC *glpfVertexAttrib4f = NULL;
PFNGLVERTEXATTRIB4FV_PROC *glpfVertexAttrib4fv = NULL;
PFNGLVERTEXATTRIB4IV_PROC *glpfVertexAttrib4iv = NULL;
PFNGLVERTEXATTRIB4S_PROC *glpfVertexAttrib4s = NULL;
PFNGLVERTEXATTRIB4SV_PROC *glpfVertexAttrib4sv = NULL;
PFNGLVERTEXATTRIB4UBV_PROC *glpfVertexAttrib4ubv = NULL;
PFNGLVERTEXATTRIB4UIV_PROC *glpfVertexAttrib4uiv = NULL;
PFNGLVERTEXATTRIB4USV_PROC *glpfVertexAttrib4usv = NULL;
PFNGLVERTEXATTRIBPOINTER_PROC *glpfVertexAttribPointer = NULL;

/* GL_VERSION_2_1 */

PFNGLUNIFORMMATRIX2X3FV_PROC *glpfUniformMatrix2x3fv = NULL;
PFNGLUNIFORMMATRIX3X2FV_PROC *glpfUniformMatrix3x2fv = NULL;
PFNGLUNIFORMMATRIX2X4FV_PROC *glpfUniformMatrix2x4fv = NULL;
PFNGLUNIFORMMATRIX4X2FV_PROC *glpfUniformMatrix4x2fv = NULL;
PFNGLUNIFORMMATRIX3X4FV_PROC *glpfUniformMatrix3x4fv = NULL;
PFNGLUNIFORMMATRIX4X3FV_PROC *glpfUniformMatrix4x3fv = NULL;

/* GL_VERSION_3_0 */

PFNGLCOLORMASKI_PROC *glpfColorMaski = NULL;
PFNGLGETBOOLEANI_V_PROC *glpfGetBooleani_v = NULL;
PFNGLGETINTEGERI_V_PROC *glpfGetIntegeri_v = NULL;
PFNGLENABLEI_PROC *glpfEnablei = NULL;
PFNGLDISABLEI_PROC *glpfDisablei = NULL;
PFNGLISENABLEDI_PROC *glpfIsEnabledi = NULL;
PFNGLBEGINTRANSFORMFEEDBACK_PROC *glpfBeginTransformFeedback = NULL;
PFNGLENDTRANSFORMFEEDBACK_PROC *glpfEndTransformFeedback = NULL;
PFNGLBINDBUFFERRANGE_PROC *glpfBindBufferRange = NULL;
PFNGLBINDBUFFERBASE_PROC *glpfBindBufferBase = NULL;
PFNGLTRANSFORMFEEDBACKVARYINGS_PROC *glpfTransformFeedbackVaryings = NULL;
PFNGLGETTRANSFORMFEEDBACKVARYING_PROC *glpfGetTransformFeedbackVarying = NULL;
PFNGLCLAMPCOLOR_PROC *glpfClampColor = NULL;
PFNGLBEGINCONDITIONALRENDER_PROC *glpfBeginConditionalRender = NULL;
PFNGLENDCONDITIONALRENDER_PROC *glpfEndConditionalRender = NULL;
PFNGLVERTEXATTRIBIPOINTER_PROC *glpfVertexAttribIPointer = NULL;
PFNGLGETVERTEXATTRIBIIV_PROC *glpfGetVertexAttribIiv = NULL;
PFNGLGETVERTEXATTRIBIUIV_PROC *glpfGetVertexAttribIuiv = NULL;
PFNGLVERTEXATTRIBI1I_PROC *glpfVertexAttribI1i = NULL;
PFNGLVERTEXATTRIBI2I_PROC *glpfVertexAttribI2i = NULL;
PFNGLVERTEXATTRIBI3I_PROC *glpfVertexAttribI3i = NULL;
PFNGLVERTEXATTRIBI4I_PROC *glpfVertexAttribI4i = NULL;
PFNGLVERTEXATTRIBI1UI_PROC *glpfVertexAttribI1ui = NULL;
PFNGLVERTEXATTRIBI2UI_PROC *glpfVertexAttribI2ui = NULL;
PFNGLVERTEXATTRIBI3UI_PROC *glpfVertexAttribI3ui = NULL;
PFNGLVERTEXATTRIBI4UI_PROC *glpfVertexAttribI4ui = NULL;
PFNGLVERTEXATTRIBI1IV_PROC *glpfVertexAttribI1iv = NULL;
PFNGLVERTEXATTRIBI2IV_PROC *glpfVertexAttribI2iv = NULL;
PFNGLVERTEXATTRIBI3IV_PROC *glpfVertexAttribI3iv = NULL;
PFNGLVERTEXATTRIBI4IV_PROC *glpfVertexAttribI4iv = NULL;
PFNGLVERTEXATTRIBI1UIV_PROC *glpfVertexAttribI1uiv = NULL;
PFNGLVERTEXATTRIBI2UIV_PROC *glpfVertexAttribI2uiv = NULL;
PFNGLVERTEXATTRIBI3UIV_PROC *glpfVertexAttribI3uiv = NULL;
PFNGLVERTEXATTRIBI4UIV_PROC *glpfVertexAttribI4uiv = NULL;
PFNGLVERTEXATTRIBI4BV_PROC *glpfVertexAttribI4bv = NULL;
PFNGLVERTEXATTRIBI4SV_PROC *glpfVertexAttribI4sv = NULL;
PFNGLVERTEXATTRIBI4UBV_PROC *glpfVertexAttribI4ubv = NULL;
PFNGLVERTEXATTRIBI4USV_PROC *glpfVertexAttribI4usv = NULL;
PFNGLGETUNIFORMUIV_PROC *glpfGetUniformuiv = NULL;
PFNGLBINDFRAGDATALOCATION_PROC *glpfBindFragDataLocation = NULL;
PFNGLGETFRAGDATALOCATION_PROC *glpfGetFragDataLocation = NULL;
PFNGLUNIFORM1UI_PROC *glpfUniform1ui = NULL;
PFNGLUNIFORM2UI_PROC *glpfUniform2ui = NULL;
PFNGLUNIFORM3UI_PROC *glpfUniform3ui = NULL;
PFNGLUNIFORM4UI_PROC *glpfUniform4ui = NULL;
PFNGLUNIFORM1UIV_PROC *glpfUniform1uiv = NULL;
PFNGLUNIFORM2UIV_PROC *glpfUniform2uiv = NULL;
PFNGLUNIFORM3UIV_PROC *glpfUniform3uiv = NULL;
PFNGLUNIFORM4UIV_PROC *glpfUniform4uiv = NULL;
PFNGLTEXPARAMETERIIV_PROC *glpfTexParameterIiv = NULL;
PFNGLTEXPARAMETERIUIV_PROC *glpfTexParameterIuiv = NULL;
PFNGLGETTEXPARAMETERIIV_PROC *glpfGetTexParameterIiv = NULL;
PFNGLGETTEXPARAMETERIUIV_PROC *glpfGetTexParameterIuiv = NULL;
PFNGLCLEARBUFFERIV_PROC *glpfClearBufferiv = NULL;
PFNGLCLEARBUFFERUIV_PROC *glpfClearBufferuiv = NULL;
PFNGLCLEARBUFFERFV_PROC *glpfClearBufferfv = NULL;
PFNGLCLEARBUFFERFI_PROC *glpfClearBufferfi = NULL;
PFNGLGETSTRINGI_PROC *glpfGetStringi = NULL;
PFNGLISRENDERBUFFER_PROC *glpfIsRenderbuffer = NULL;
PFNGLBINDRENDERBUFFER_PROC *glpfBindRenderbuffer = NULL;
PFNGLDELETERENDERBUFFERS_PROC *glpfDeleteRenderbuffers = NULL;
PFNGLGENRENDERBUFFERS_PROC *glpfGenRenderbuffers = NULL;
PFNGLRENDERBUFFERSTORAGE_PROC *glpfRenderbufferStorage = NULL;
PFNGLGETRENDERBUFFERPARAMETERIV_PROC *glpfGetRenderbufferParameteriv = NULL;
PFNGLISFRAMEBUFFER_PROC *glpfIsFramebuffer = NULL;
PFNGLBINDFRAMEBUFFER_PROC *glpfBindFramebuffer = NULL;
PFNGLDELETEFRAMEBUFFERS_PROC *glpfDeleteFramebuffers = NULL;
PFNGLGENFRAMEBUFFERS_PROC *glpfGenFramebuffers = NULL;
PFNGLCHECKFRAMEBUFFERSTATUS_PROC *glpfCheckFramebufferStatus = NULL;
PFNGLFRAMEBUFFERTEXTURE1D_PROC *glpfFramebufferTexture1D = NULL;
PFNGLFRAMEBUFFERTEXTURE2D_PROC *glpfFramebufferTexture2D = NULL;
PFNGLFRAMEBUFFERTEXTURE3D_PROC *glpfFramebufferTexture3D = NULL;
PFNGLFRAMEBUFFERRENDERBUFFER_PROC *glpfFramebufferRenderbuffer = NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIV_PROC *
    glpfGetFramebufferAttachmentParameteriv = NULL;
PFNGLGENERATEMIPMAP_PROC *glpfGenerateMipmap = NULL;
PFNGLBLITFRAMEBUFFER_PROC *glpfBlitFramebuffer = NULL;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLE_PROC *glpfRenderbufferStorageMultisample =
    NULL;
PFNGLFRAMEBUFFERTEXTURELAYER_PROC *glpfFramebufferTextureLayer = NULL;
PFNGLMAPBUFFERRANGE_PROC *glpfMapBufferRange = NULL;
PFNGLFLUSHMAPPEDBUFFERRANGE_PROC *glpfFlushMappedBufferRange = NULL;
PFNGLBINDVERTEXARRAY_PROC *glpfBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYS_PROC *glpfDeleteVertexArrays = NULL;
PFNGLGENVERTEXARRAYS_PROC *glpfGenVertexArrays = NULL;
PFNGLISVERTEXARRAY_PROC *glpfIsVertexArray = NULL;

/* GL_VERSION_3_1 */

PFNGLDRAWARRAYSINSTANCED_PROC *glpfDrawArraysInstanced = NULL;
PFNGLDRAWELEMENTSINSTANCED_PROC *glpfDrawElementsInstanced = NULL;
PFNGLTEXBUFFER_PROC *glpfTexBuffer = NULL;
PFNGLPRIMITIVERESTARTINDEX_PROC *glpfPrimitiveRestartIndex = NULL;
PFNGLCOPYBUFFERSUBDATA_PROC *glpfCopyBufferSubData = NULL;
PFNGLGETUNIFORMINDICES_PROC *glpfGetUniformIndices = NULL;
PFNGLGETACTIVEUNIFORMSIV_PROC *glpfGetActiveUniformsiv = NULL;
PFNGLGETACTIVEUNIFORMNAME_PROC *glpfGetActiveUniformName = NULL;
PFNGLGETUNIFORMBLOCKINDEX_PROC *glpfGetUniformBlockIndex = NULL;
PFNGLGETACTIVEUNIFORMBLOCKIV_PROC *glpfGetActiveUniformBlockiv = NULL;
PFNGLGETACTIVEUNIFORMBLOCKNAME_PROC *glpfGetActiveUniformBlockName = NULL;
PFNGLUNIFORMBLOCKBINDING_PROC *glpfUniformBlockBinding = NULL;

/* GL_VERSION_3_2 */

PFNGLDRAWELEMENTSBASEVERTEX_PROC *glpfDrawElementsBaseVertex = NULL;
PFNGLDRAWRANGEELEMENTSBASEVERTEX_PROC *glpfDrawRangeElementsBaseVertex = NULL;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEX_PROC *glpfDrawElementsInstancedBaseVertex =
    NULL;
PFNGLMULTIDRAWELEMENTSBASEVERTEX_PROC *glpfMultiDrawElementsBaseVertex = NULL;
PFNGLPROVOKINGVERTEX_PROC *glpfProvokingVertex = NULL;
PFNGLFENCESYNC_PROC *glpfFenceSync = NULL;
PFNGLISSYNC_PROC *glpfIsSync = NULL;
PFNGLDELETESYNC_PROC *glpfDeleteSync = NULL;
PFNGLCLIENTWAITSYNC_PROC *glpfClientWaitSync = NULL;
PFNGLWAITSYNC_PROC *glpfWaitSync = NULL;
PFNGLGETINTEGER64V_PROC *glpfGetInteger64v = NULL;
PFNGLGETSYNCIV_PROC *glpfGetSynciv = NULL;
PFNGLGETINTEGER64I_V_PROC *glpfGetInteger64i_v = NULL;
PFNGLGETBUFFERPARAMETERI64V_PROC *glpfGetBufferParameteri64v = NULL;
PFNGLFRAMEBUFFERTEXTURE_PROC *glpfFramebufferTexture = NULL;
PFNGLTEXIMAGE2DMULTISAMPLE_PROC *glpfTexImage2DMultisample = NULL;
PFNGLTEXIMAGE3DMULTISAMPLE_PROC *glpfTexImage3DMultisample = NULL;
PFNGLGETMULTISAMPLEFV_PROC *glpfGetMultisamplefv = NULL;
PFNGLSAMPLEMASKI_PROC *glpfSampleMaski = NULL;

/* GL_VERSION_3_3 */

PFNGLBINDFRAGDATALOCATIONINDEXED_PROC *glpfBindFragDataLocationIndexed = NULL;
PFNGLGETFRAGDATAINDEX_PROC *glpfGetFragDataIndex = NULL;
PFNGLGENSAMPLERS_PROC *glpfGenSamplers = NULL;
PFNGLDELETESAMPLERS_PROC *glpfDeleteSamplers = NULL;
PFNGLISSAMPLER_PROC *glpfIsSampler = NULL;
PFNGLBINDSAMPLER_PROC *glpfBindSampler = NULL;
PFNGLSAMPLERPARAMETERI_PROC *glpfSamplerParameteri = NULL;
PFNGLSAMPLERPARAMETERIV_PROC *glpfSamplerParameteriv = NULL;
PFNGLSAMPLERPARAMETERF_PROC *glpfSamplerParameterf = NULL;
PFNGLSAMPLERPARAMETERFV_PROC *glpfSamplerParameterfv = NULL;
PFNGLSAMPLERPARAMETERIIV_PROC *glpfSamplerParameterIiv = NULL;
PFNGLSAMPLERPARAMETERIUIV_PROC *glpfSamplerParameterIuiv = NULL;
PFNGLGETSAMPLERPARAMETERIV_PROC *glpfGetSamplerParameteriv = NULL;
PFNGLGETSAMPLERPARAMETERIIV_PROC *glpfGetSamplerParameterIiv = NULL;
PFNGLGETSAMPLERPARAMETERFV_PROC *glpfGetSamplerParameterfv = NULL;
PFNGLGETSAMPLERPARAMETERIUIV_PROC *glpfGetSamplerParameterIuiv = NULL;
PFNGLQUERYCOUNTER_PROC *glpfQueryCounter = NULL;
PFNGLGETQUERYOBJECTI64V_PROC *glpfGetQueryObjecti64v = NULL;
PFNGLGETQUERYOBJECTUI64V_PROC *glpfGetQueryObjectui64v = NULL;
PFNGLVERTEXATTRIBDIVISOR_PROC *glpfVertexAttribDivisor = NULL;
PFNGLVERTEXATTRIBP1UI_PROC *glpfVertexAttribP1ui = NULL;
PFNGLVERTEXATTRIBP1UIV_PROC *glpfVertexAttribP1uiv = NULL;
PFNGLVERTEXATTRIBP2UI_PROC *glpfVertexAttribP2ui = NULL;
PFNGLVERTEXATTRIBP2UIV_PROC *glpfVertexAttribP2uiv = NULL;
PFNGLVERTEXATTRIBP3UI_PROC *glpfVertexAttribP3ui = NULL;
PFNGLVERTEXATTRIBP3UIV_PROC *glpfVertexAttribP3uiv = NULL;
PFNGLVERTEXATTRIBP4UI_PROC *glpfVertexAttribP4ui = NULL;
PFNGLVERTEXATTRIBP4UIV_PROC *glpfVertexAttribP4uiv = NULL;

/* GL_VERSION_4_0 */

PFNGLMINSAMPLESHADING_PROC *glpfMinSampleShading = NULL;
PFNGLBLENDEQUATIONI_PROC *glpfBlendEquationi = NULL;
PFNGLBLENDEQUATIONSEPARATEI_PROC *glpfBlendEquationSeparatei = NULL;
PFNGLBLENDFUNCI_PROC *glpfBlendFunci = NULL;
PFNGLBLENDFUNCSEPARATEI_PROC *glpfBlendFuncSeparatei = NULL;
PFNGLDRAWARRAYSINDIRECT_PROC *glpfDrawArraysIndirect = NULL;
PFNGLDRAWELEMENTSINDIRECT_PROC *glpfDrawElementsIndirect = NULL;
PFNGLUNIFORM1D_PROC *glpfUniform1d = NULL;
PFNGLUNIFORM2D_PROC *glpfUniform2d = NULL;
PFNGLUNIFORM3D_PROC *glpfUniform3d = NULL;
PFNGLUNIFORM4D_PROC *glpfUniform4d = NULL;
PFNGLUNIFORM1DV_PROC *glpfUniform1dv = NULL;
PFNGLUNIFORM2DV_PROC *glpfUniform2dv = NULL;
PFNGLUNIFORM3DV_PROC *glpfUniform3dv = NULL;
PFNGLUNIFORM4DV_PROC *glpfUniform4dv = NULL;
PFNGLUNIFORMMATRIX2DV_PROC *glpfUniformMatrix2dv = NULL;
PFNGLUNIFORMMATRIX3DV_PROC *glpfUniformMatrix3dv = NULL;
PFNGLUNIFORMMATRIX4DV_PROC *glpfUniformMatrix4dv = NULL;
PFNGLUNIFORMMATRIX2X3DV_PROC *glpfUniformMatrix2x3dv = NULL;
PFNGLUNIFORMMATRIX2X4DV_PROC *glpfUniformMatrix2x4dv = NULL;
PFNGLUNIFORMMATRIX3X2DV_PROC *glpfUniformMatrix3x2dv = NULL;
PFNGLUNIFORMMATRIX3X4DV_PROC *glpfUniformMatrix3x4dv = NULL;
PFNGLUNIFORMMATRIX4X2DV_PROC *glpfUniformMatrix4x2dv = NULL;
PFNGLUNIFORMMATRIX4X3DV_PROC *glpfUniformMatrix4x3dv = NULL;
PFNGLGETUNIFORMDV_PROC *glpfGetUniformdv = NULL;
PFNGLGETSUBROUTINEUNIFORMLOCATION_PROC *glpfGetSubroutineUniformLocation = NULL;
PFNGLGETSUBROUTINEINDEX_PROC *glpfGetSubroutineIndex = NULL;
PFNGLGETACTIVESUBROUTINEUNIFORMIV_PROC *glpfGetActiveSubroutineUniformiv = NULL;
PFNGLGETACTIVESUBROUTINEUNIFORMNAME_PROC *glpfGetActiveSubroutineUniformName =
    NULL;
PFNGLGETACTIVESUBROUTINENAME_PROC *glpfGetActiveSubroutineName = NULL;
PFNGLUNIFORMSUBROUTINESUIV_PROC *glpfUniformSubroutinesuiv = NULL;
PFNGLGETUNIFORMSUBROUTINEUIV_PROC *glpfGetUniformSubroutineuiv = NULL;
PFNGLGETPROGRAMSTAGEIV_PROC *glpfGetProgramStageiv = NULL;
PFNGLPATCHPARAMETERI_PROC *glpfPatchParameteri = NULL;
PFNGLPATCHPARAMETERFV_PROC *glpfPatchParameterfv = NULL;
PFNGLBINDTRANSFORMFEEDBACK_PROC *glpfBindTransformFeedback = NULL;
PFNGLDELETETRANSFORMFEEDBACKS_PROC *glpfDeleteTransformFeedbacks = NULL;
PFNGLGENTRANSFORMFEEDBACKS_PROC *glpfGenTransformFeedbacks = NULL;
PFNGLISTRANSFORMFEEDBACK_PROC *glpfIsTransformFeedback = NULL;
PFNGLPAUSETRANSFORMFEEDBACK_PROC *glpfPauseTransformFeedback = NULL;
PFNGLRESUMETRANSFORMFEEDBACK_PROC *glpfResumeTransformFeedback = NULL;
PFNGLDRAWTRANSFORMFEEDBACK_PROC *glpfDrawTransformFeedback = NULL;
PFNGLDRAWTRANSFORMFEEDBACKSTREAM_PROC *glpfDrawTransformFeedbackStream = NULL;
PFNGLBEGINQUERYINDEXED_PROC *glpfBeginQueryIndexed = NULL;
PFNGLENDQUERYINDEXED_PROC *glpfEndQueryIndexed = NULL;
PFNGLGETQUERYINDEXEDIV_PROC *glpfGetQueryIndexediv = NULL;

/* GL_VERSION_4_1 */

PFNGLRELEASESHADERCOMPILER_PROC *glpfReleaseShaderCompiler = NULL;
PFNGLSHADERBINARY_PROC *glpfShaderBinary = NULL;
PFNGLGETSHADERPRECISIONFORMAT_PROC *glpfGetShaderPrecisionFormat = NULL;
PFNGLDEPTHRANGEF_PROC *glpfDepthRangef = NULL;
PFNGLCLEARDEPTHF_PROC *glpfClearDepthf = NULL;
PFNGLGETPROGRAMBINARY_PROC *glpfGetProgramBinary = NULL;
PFNGLPROGRAMBINARY_PROC *glpfProgramBinary = NULL;
PFNGLPROGRAMPARAMETERI_PROC *glpfProgramParameteri = NULL;
PFNGLUSEPROGRAMSTAGES_PROC *glpfUseProgramStages = NULL;
PFNGLACTIVESHADERPROGRAM_PROC *glpfActiveShaderProgram = NULL;
PFNGLCREATESHADERPROGRAMV_PROC *glpfCreateShaderProgramv = NULL;
PFNGLBINDPROGRAMPIPELINE_PROC *glpfBindProgramPipeline = NULL;
PFNGLDELETEPROGRAMPIPELINES_PROC *glpfDeleteProgramPipelines = NULL;
PFNGLGENPROGRAMPIPELINES_PROC *glpfGenProgramPipelines = NULL;
PFNGLISPROGRAMPIPELINE_PROC *glpfIsProgramPipeline = NULL;
PFNGLGETPROGRAMPIPELINEIV_PROC *glpfGetProgramPipelineiv = NULL;
PFNGLPROGRAMUNIFORM1I_PROC *glpfProgramUniform1i = NULL;
PFNGLPROGRAMUNIFORM1IV_PROC *glpfProgramUniform1iv = NULL;
PFNGLPROGRAMUNIFORM1F_PROC *glpfProgramUniform1f = NULL;
PFNGLPROGRAMUNIFORM1FV_PROC *glpfProgramUniform1fv = NULL;
PFNGLPROGRAMUNIFORM1D_PROC *glpfProgramUniform1d = NULL;
PFNGLPROGRAMUNIFORM1DV_PROC *glpfProgramUniform1dv = NULL;
PFNGLPROGRAMUNIFORM1UI_PROC *glpfProgramUniform1ui = NULL;
PFNGLPROGRAMUNIFORM1UIV_PROC *glpfProgramUniform1uiv = NULL;
PFNGLPROGRAMUNIFORM2I_PROC *glpfProgramUniform2i = NULL;
PFNGLPROGRAMUNIFORM2IV_PROC *glpfProgramUniform2iv = NULL;
PFNGLPROGRAMUNIFORM2F_PROC *glpfProgramUniform2f = NULL;
PFNGLPROGRAMUNIFORM2FV_PROC *glpfProgramUniform2fv = NULL;
PFNGLPROGRAMUNIFORM2D_PROC *glpfProgramUniform2d = NULL;
PFNGLPROGRAMUNIFORM2DV_PROC *glpfProgramUniform2dv = NULL;
PFNGLPROGRAMUNIFORM2UI_PROC *glpfProgramUniform2ui = NULL;
PFNGLPROGRAMUNIFORM2UIV_PROC *glpfProgramUniform2uiv = NULL;
PFNGLPROGRAMUNIFORM3I_PROC *glpfProgramUniform3i = NULL;
PFNGLPROGRAMUNIFORM3IV_PROC *glpfProgramUniform3iv = NULL;
PFNGLPROGRAMUNIFORM3F_PROC *glpfProgramUniform3f = NULL;
PFNGLPROGRAMUNIFORM3FV_PROC *glpfProgramUniform3fv = NULL;
PFNGLPROGRAMUNIFORM3D_PROC *glpfProgramUniform3d = NULL;
PFNGLPROGRAMUNIFORM3DV_PROC *glpfProgramUniform3dv = NULL;
PFNGLPROGRAMUNIFORM3UI_PROC *glpfProgramUniform3ui = NULL;
PFNGLPROGRAMUNIFORM3UIV_PROC *glpfProgramUniform3uiv = NULL;
PFNGLPROGRAMUNIFORM4I_PROC *glpfProgramUniform4i = NULL;
PFNGLPROGRAMUNIFORM4IV_PROC *glpfProgramUniform4iv = NULL;
PFNGLPROGRAMUNIFORM4F_PROC *glpfProgramUniform4f = NULL;
PFNGLPROGRAMUNIFORM4FV_PROC *glpfProgramUniform4fv = NULL;
PFNGLPROGRAMUNIFORM4D_PROC *glpfProgramUniform4d = NULL;
PFNGLPROGRAMUNIFORM4DV_PROC *glpfProgramUniform4dv = NULL;
PFNGLPROGRAMUNIFORM4UI_PROC *glpfProgramUniform4ui = NULL;
PFNGLPROGRAMUNIFORM4UIV_PROC *glpfProgramUniform4uiv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2FV_PROC *glpfProgramUniformMatrix2fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3FV_PROC *glpfProgramUniformMatrix3fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4FV_PROC *glpfProgramUniformMatrix4fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2DV_PROC *glpfProgramUniformMatrix2dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3DV_PROC *glpfProgramUniformMatrix3dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4DV_PROC *glpfProgramUniformMatrix4dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X3FV_PROC *glpfProgramUniformMatrix2x3fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X2FV_PROC *glpfProgramUniformMatrix3x2fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X4FV_PROC *glpfProgramUniformMatrix2x4fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X2FV_PROC *glpfProgramUniformMatrix4x2fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X4FV_PROC *glpfProgramUniformMatrix3x4fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X3FV_PROC *glpfProgramUniformMatrix4x3fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X3DV_PROC *glpfProgramUniformMatrix2x3dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X2DV_PROC *glpfProgramUniformMatrix3x2dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X4DV_PROC *glpfProgramUniformMatrix2x4dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X2DV_PROC *glpfProgramUniformMatrix4x2dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X4DV_PROC *glpfProgramUniformMatrix3x4dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X3DV_PROC *glpfProgramUniformMatrix4x3dv = NULL;
PFNGLVALIDATEPROGRAMPIPELINE_PROC *glpfValidateProgramPipeline = NULL;
PFNGLGETPROGRAMPIPELINEINFOLOG_PROC *glpfGetProgramPipelineInfoLog = NULL;
PFNGLVERTEXATTRIBL1D_PROC *glpfVertexAttribL1d = NULL;
PFNGLVERTEXATTRIBL2D_PROC *glpfVertexAttribL2d = NULL;
PFNGLVERTEXATTRIBL3D_PROC *glpfVertexAttribL3d = NULL;
PFNGLVERTEXATTRIBL4D_PROC *glpfVertexAttribL4d = NULL;
PFNGLVERTEXATTRIBL1DV_PROC *glpfVertexAttribL1dv = NULL;
PFNGLVERTEXATTRIBL2DV_PROC *glpfVertexAttribL2dv = NULL;
PFNGLVERTEXATTRIBL3DV_PROC *glpfVertexAttribL3dv = NULL;
PFNGLVERTEXATTRIBL4DV_PROC *glpfVertexAttribL4dv = NULL;
PFNGLVERTEXATTRIBLPOINTER_PROC *glpfVertexAttribLPointer = NULL;
PFNGLGETVERTEXATTRIBLDV_PROC *glpfGetVertexAttribLdv = NULL;
PFNGLVIEWPORTARRAYV_PROC *glpfViewportArrayv = NULL;
PFNGLVIEWPORTINDEXEDF_PROC *glpfViewportIndexedf = NULL;
PFNGLVIEWPORTINDEXEDFV_PROC *glpfViewportIndexedfv = NULL;
PFNGLSCISSORARRAYV_PROC *glpfScissorArrayv = NULL;
PFNGLSCISSORINDEXED_PROC *glpfScissorIndexed = NULL;
PFNGLSCISSORINDEXEDV_PROC *glpfScissorIndexedv = NULL;
PFNGLDEPTHRANGEARRAYV_PROC *glpfDepthRangeArrayv = NULL;
PFNGLDEPTHRANGEINDEXED_PROC *glpfDepthRangeIndexed = NULL;
PFNGLGETFLOATI_V_PROC *glpfGetFloati_v = NULL;
PFNGLGETDOUBLEI_V_PROC *glpfGetDoublei_v = NULL;

/* GL_VERSION_4_2 */

PFNGLDRAWARRAYSINSTANCEDBASEINSTANCE_PROC *glpfDrawArraysInstancedBaseInstance =
    NULL;
PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCE_PROC *
    glpfDrawElementsInstancedBaseInstance = NULL;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCE_PROC *
    glpfDrawElementsInstancedBaseVertexBaseInstance = NULL;
PFNGLGETINTERNALFORMATIV_PROC *glpfGetInternalformativ = NULL;
PFNGLGETACTIVEATOMICCOUNTERBUFFERIV_PROC *glpfGetActiveAtomicCounterBufferiv =
    NULL;
PFNGLBINDIMAGETEXTURE_PROC *glpfBindImageTexture = NULL;
PFNGLMEMORYBARRIER_PROC *glpfMemoryBarrier = NULL;
PFNGLTEXSTORAGE1D_PROC *glpfTexStorage1D = NULL;
PFNGLTEXSTORAGE2D_PROC *glpfTexStorage2D = NULL;
PFNGLTEXSTORAGE3D_PROC *glpfTexStorage3D = NULL;
PFNGLDRAWTRANSFORMFEEDBACKINSTANCED_PROC *glpfDrawTransformFeedbackInstanced =
    NULL;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCED_PROC *
    glpfDrawTransformFeedbackStreamInstanced = NULL;

/* GL_VERSION_4_3 */

PFNGLCLEARBUFFERDATA_PROC *glpfClearBufferData = NULL;
PFNGLCLEARBUFFERSUBDATA_PROC *glpfClearBufferSubData = NULL;
PFNGLDISPATCHCOMPUTE_PROC *glpfDispatchCompute = NULL;
PFNGLDISPATCHCOMPUTEINDIRECT_PROC *glpfDispatchComputeIndirect = NULL;
PFNGLCOPYIMAGESUBDATA_PROC *glpfCopyImageSubData = NULL;
PFNGLFRAMEBUFFERPARAMETERI_PROC *glpfFramebufferParameteri = NULL;
PFNGLGETFRAMEBUFFERPARAMETERIV_PROC *glpfGetFramebufferParameteriv = NULL;
PFNGLGETINTERNALFORMATI64V_PROC *glpfGetInternalformati64v = NULL;
PFNGLINVALIDATETEXSUBIMAGE_PROC *glpfInvalidateTexSubImage = NULL;
PFNGLINVALIDATETEXIMAGE_PROC *glpfInvalidateTexImage = NULL;
PFNGLINVALIDATEBUFFERSUBDATA_PROC *glpfInvalidateBufferSubData = NULL;
PFNGLINVALIDATEBUFFERDATA_PROC *glpfInvalidateBufferData = NULL;
PFNGLINVALIDATEFRAMEBUFFER_PROC *glpfInvalidateFramebuffer = NULL;
PFNGLINVALIDATESUBFRAMEBUFFER_PROC *glpfInvalidateSubFramebuffer = NULL;
PFNGLMULTIDRAWARRAYSINDIRECT_PROC *glpfMultiDrawArraysIndirect = NULL;
PFNGLMULTIDRAWELEMENTSINDIRECT_PROC *glpfMultiDrawElementsIndirect = NULL;
PFNGLGETPROGRAMINTERFACEIV_PROC *glpfGetProgramInterfaceiv = NULL;
PFNGLGETPROGRAMRESOURCEINDEX_PROC *glpfGetProgramResourceIndex = NULL;
PFNGLGETPROGRAMRESOURCENAME_PROC *glpfGetProgramResourceName = NULL;
PFNGLGETPROGRAMRESOURCEIV_PROC *glpfGetProgramResourceiv = NULL;
PFNGLGETPROGRAMRESOURCELOCATION_PROC *glpfGetProgramResourceLocation = NULL;
PFNGLGETPROGRAMRESOURCELOCATIONINDEX_PROC *glpfGetProgramResourceLocationIndex =
    NULL;
PFNGLSHADERSTORAGEBLOCKBINDING_PROC *glpfShaderStorageBlockBinding = NULL;
PFNGLTEXBUFFERRANGE_PROC *glpfTexBufferRange = NULL;
PFNGLTEXSTORAGE2DMULTISAMPLE_PROC *glpfTexStorage2DMultisample = NULL;
PFNGLTEXSTORAGE3DMULTISAMPLE_PROC *glpfTexStorage3DMultisample = NULL;
PFNGLTEXTUREVIEW_PROC *glpfTextureView = NULL;
PFNGLBINDVERTEXBUFFER_PROC *glpfBindVertexBuffer = NULL;
PFNGLVERTEXATTRIBFORMAT_PROC *glpfVertexAttribFormat = NULL;
PFNGLVERTEXATTRIBIFORMAT_PROC *glpfVertexAttribIFormat = NULL;
PFNGLVERTEXATTRIBLFORMAT_PROC *glpfVertexAttribLFormat = NULL;
PFNGLVERTEXATTRIBBINDING_PROC *glpfVertexAttribBinding = NULL;
PFNGLVERTEXBINDINGDIVISOR_PROC *glpfVertexBindingDivisor = NULL;
PFNGLDEBUGMESSAGECONTROL_PROC *glpfDebugMessageControl = NULL;
PFNGLDEBUGMESSAGEINSERT_PROC *glpfDebugMessageInsert = NULL;
PFNGLDEBUGMESSAGECALLBACK_PROC *glpfDebugMessageCallback = NULL;
PFNGLGETDEBUGMESSAGELOG_PROC *glpfGetDebugMessageLog = NULL;
PFNGLPUSHDEBUGGROUP_PROC *glpfPushDebugGroup = NULL;
PFNGLPOPDEBUGGROUP_PROC *glpfPopDebugGroup = NULL;
PFNGLOBJECTLABEL_PROC *glpfObjectLabel = NULL;
PFNGLGETOBJECTLABEL_PROC *glpfGetObjectLabel = NULL;
PFNGLOBJECTPTRLABEL_PROC *glpfObjectPtrLabel = NULL;
PFNGLGETOBJECTPTRLABEL_PROC *glpfGetObjectPtrLabel = NULL;
PFNGLGETPOINTERV_PROC *glpfGetPointerv = NULL;

static void add_extension(const char *extension) {}

/* ------------------ get_proc from Slavomir Kaslev's gl3w ----------------- */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

static HMODULE libgl;

static void open_libgl(void) { libgl = LoadLibraryA("opengl32.dll"); }

static void close_libgl(void) { FreeLibrary(libgl); }

static GLPROC get_proc(const char *proc) {
  GLPROC res;

  res = (GLPROC)wglGetProcAddress(proc);
  if (!res)
    res = (GLPROC)GetProcAddress(libgl, proc);
  return res;
}
#elif defined(__APPLE__) || defined(__APPLE_CC__)
#include <Carbon/Carbon.h>

CFBundleRef bundle;
CFURLRef bundleURL;

static void open_libgl(void) {
  bundleURL = CFURLCreateWithFileSystemPath(
      kCFAllocatorDefault, CFSTR("/System/Library/Frameworks/OpenGL.framework"),
      kCFURLPOSIXPathStyle, true);

  bundle = CFBundleCreate(kCFAllocatorDefault, bundleURL);
  assert(bundle != NULL);
}

static void close_libgl(void) {
  CFRelease(bundle);
  CFRelease(bundleURL);
}

static GLPROC get_proc(const char *proc) {
  GLPROC res;

  CFStringRef procname = CFStringCreateWithCString(kCFAllocatorDefault, proc,
                                                   kCFStringEncodingASCII);
  res = (GLPROC)CFBundleGetFunctionPointerForName(bundle, procname);
  CFRelease(procname);
  return res;
}
#else
#include <dlfcn.h>
#include <GL/glx.h>

static void *libgl;

static void open_libgl(void) {
  libgl = dlopen("libGL.so.1", RTLD_LAZY | RTLD_GLOBAL);
}

static void close_libgl(void) { dlclose(libgl); }

static GLPROC get_proc(const char *proc) {
  GLPROC res;

  res = (GLPROC)glXGetProcAddress((const GLubyte *)proc);
  if (!res)
    res = (GLPROC)dlsym(libgl, proc);
  return res;
}
#endif

#ifdef __cplusplus
}
#endif
