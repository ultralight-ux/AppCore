#include "GPUDriverGL.h"
#include "GPUContextGL.h"
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/FileSystem.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../../../shaders/glsl/shader_fill_frag.h"
#include "../../../shaders/glsl/shader_fill_path_frag.h"
#include "../../../shaders/glsl/shader_v2f_c4f_t2f_t2f_d28f_vert.h"
#include "../../../shaders/glsl/shader_v2f_c4f_t2f_vert.h"

#define SHADER_PATH "glsl/"

#ifdef _DEBUG
#if _WIN32
#define INFO(x) { std::cerr << "[INFO] " << __FUNCSIG__ << " @ Line " << __LINE__ << ":\n\t" << x << std::endl; }
#else
#define INFO(x) { std::cerr << "[INFO] " << __PRETTY_FUNCTION__ << " @ Line " << __LINE__ << ":\n\t" << x << std::endl; }
#endif
#else
#define INFO(x)
#endif

#if _WIN32
#include <Windows.h>
#define FATAL(x) { std::stringstream str; \
  str << "[ERROR] " << __FUNCSIG__ << " @ Line " << __LINE__ << ":\n\t" << x << std::endl; \
  OutputDebugStringA(str.str().c_str()); \
  std::cerr << str.str() << std::endl; \
  __debugbreak(); std::cin.get(); exit(-1); }
#else
#define FATAL(x) { std::cerr << "[ERROR] " << __PRETTY_FUNCTION__ << " @ Line " << __LINE__ << ":\n\t" << x << std::endl; \
  std::cin.get(); exit(-1); }
#endif

static void ReadFile(const char* filepath, std::string& result) {
  // To maintain predictable behavior across platforms we use
  // whatever FileSystem that Ultralight is using:
  ultralight::FileSystem* fs = ultralight::Platform::instance().file_system();
  if (!fs)
    FATAL("No FileSystem defined.");

  auto handle = fs->OpenFile(filepath, false);
  if (handle == ultralight::invalidFileHandle)
    FATAL("Could not open file path: " << filepath);

  int64_t fileSize = 0;
  fs->GetFileSize(handle, fileSize);
  result.resize((size_t)fileSize);
  fs->ReadFromFile(handle, &result[0], fileSize);
  fs->CloseFile(handle);
}

inline char const* glErrorString(GLenum const err) noexcept
{
  switch (err)
  {
  // OpenGL 2.0+ Errors:
  case GL_NO_ERROR: return "GL_NO_ERROR";
  case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
  case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
  case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
  case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
  case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
  case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
  // OpenGL 3.0+ Errors
  case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
  default: return "UNKNOWN ERROR";
  }
}

inline std::string GetShaderLog(GLuint shader_id) {
  GLint length, result;
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
  std::string str(length, ' ');
  glGetShaderInfoLog(shader_id, (GLsizei)str.length(), &result, &str[0]);
  return str;
}

inline std::string GetProgramLog(GLuint program_id) {
  GLint length, result;
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &length);
  std::string str(length, ' ');
  glGetProgramInfoLog(program_id, (GLsizei)str.length(), &result, &str[0]);
  return str;
}

static GLuint LoadShaderFromSource(GLenum shader_type, const char* source, const char* filename) {
  GLint compileStatus;
  GLuint shader_id = glCreateShader(shader_type);
  glShaderSource(shader_id, 1, &source, NULL);
  glCompileShader(shader_id);
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compileStatus);
  if (compileStatus == GL_FALSE)
    FATAL("Unable to compile shader. Filename: " << filename << "\n\tError:"
      << glErrorString(glGetError()) << "\n\tLog: " << GetShaderLog(shader_id))
    return shader_id;
}

static GLuint LoadShaderFromFile(GLenum shader_type, const char* filename) {
  std::string shader_source;
  std::string path = std::string(SHADER_PATH) + filename;
  ReadFile(path.c_str(), shader_source);
  GLint compileStatus;
  const char* shader_source_str = shader_source.c_str();
  GLuint shader_id = glCreateShader(shader_type);
  glShaderSource(shader_id, 1, &shader_source_str, NULL);
  glCompileShader(shader_id);
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compileStatus);
  if (compileStatus == GL_FALSE)
    FATAL("Unable to compile shader. Filename: " << filename << "\n\tError:" 
      << glErrorString(glGetError()) << "\n\tLog: " << GetShaderLog(shader_id))
  return shader_id;
}

#ifdef _DEBUG
#define CHECK_GL()  {if (GLenum err = glGetError()) FATAL(glErrorString(err)) }                                                     
#else
#define CHECK_GL()
#endif   

namespace ultralight {

GPUDriverGL::GPUDriverGL(GPUContextGL* context) : context_(context) {
  // Render Buffer ID 0 is reserved for the screen
  render_buffer_map[0].fbo_id = 0;
}

void GPUDriverGL::CreateTexture(uint32_t texture_id,
  Ref<Bitmap> bitmap) {
  if (bitmap->IsEmpty()) {
    CreateFBOTexture(texture_id, bitmap);
    return;
  }

  CHECK_GL();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  CHECK_GL();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  CHECK_GL();

  TextureEntry& entry = texture_map[texture_id];
  glGenTextures(1, &entry.tex_id);
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, entry.tex_id);
  CHECK_GL();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap->row_bytes() / bitmap->bpp());
  CHECK_GL();
  if (bitmap->format() == kBitmapFormat_A8_UNORM) {
    const void* pixels = bitmap->LockPixels();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, bitmap->width(), bitmap->height(), 0,
      GL_RED, GL_UNSIGNED_BYTE, pixels);
    bitmap->UnlockPixels();
  } else if (bitmap->format() == kBitmapFormat_BGRA8_UNORM_SRGB) {
    const void* pixels = bitmap->LockPixels();
    glTexImage2D(GL_TEXTURE_2D, 0, bitmap->IsEmpty()? GL_RGBA8 : GL_SRGB8_ALPHA8, bitmap->width(), bitmap->height(), 0,
      GL_BGRA, GL_UNSIGNED_BYTE, pixels);
    bitmap->UnlockPixels();
  } else {
    FATAL("Unhandled texture format: " << bitmap->format())
  }

  CHECK_GL();
  glGenerateMipmap(GL_TEXTURE_2D);
  CHECK_GL();
}

void GPUDriverGL::UpdateTexture(uint32_t texture_id,
  Ref<Bitmap> bitmap) {
  glActiveTexture(GL_TEXTURE0 + 0);
  TextureEntry& entry = texture_map[texture_id];
  glBindTexture(GL_TEXTURE_2D, entry.tex_id);
  CHECK_GL();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap->row_bytes() / bitmap->bpp());

  if (!bitmap->IsEmpty()) {
    if (bitmap->format() == kBitmapFormat_A8_UNORM) {
      const void* pixels = bitmap->LockPixels();
      glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, bitmap->width(), bitmap->height(), 0,
        GL_RED, GL_UNSIGNED_BYTE, pixels);
      bitmap->UnlockPixels();
    } else if (bitmap->format() == kBitmapFormat_BGRA8_UNORM_SRGB) {
      const void* pixels = bitmap->LockPixels();
      glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, bitmap->width(), bitmap->height(), 0,
        GL_BGRA, GL_UNSIGNED_BYTE, pixels);
      bitmap->UnlockPixels();
    } else {
      FATAL("Unhandled texture format: " << bitmap->format());
    }

    CHECK_GL();
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  CHECK_GL();
}

void GPUDriverGL::BindTexture(uint8_t texture_unit, uint32_t texture_id) {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  BindUltralightTexture(texture_id);
}

void GPUDriverGL::DestroyTexture(uint32_t texture_id) {
  TextureEntry& entry = texture_map[texture_id];
  glDeleteTextures(1, &entry.tex_id);

  if (entry.msaa_tex_id)
    glDeleteTextures(1, &entry.msaa_tex_id);
}

void GPUDriverGL::CreateRenderBuffer(uint32_t render_buffer_id,
  const RenderBuffer& buffer) {
  if (render_buffer_id == 0) {
    INFO("Should not be reached! Render Buffer ID 0 is reserved for default framebuffer.");
    return;
  }

  RenderBufferEntry& entry = render_buffer_map[render_buffer_id];
  glGenFramebuffers(1, &entry.fbo_id);
  CHECK_GL();
  glBindFramebuffer(GL_FRAMEBUFFER, entry.fbo_id);
  CHECK_GL();

  TextureEntry& textureEntry = texture_map[buffer.texture_id];
  textureEntry.render_buffer_id = render_buffer_id;

  glBindTexture(GL_TEXTURE_2D, textureEntry.tex_id);
  CHECK_GL();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureEntry.tex_id, 0);
  CHECK_GL();

  GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, drawBuffers);
  CHECK_GL();

  GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (result != GL_FRAMEBUFFER_COMPLETE)
    FATAL("Error creating FBO, this usually fails if your DPI scale is invalid or View dimensions are massive: " << result);
  CHECK_GL();

  if (!context_->msaa_enabled())
    return;

  // Create MSAA FBO
  glGenFramebuffers(1, &entry.msaa_fbo_id);
  CHECK_GL();
  glBindFramebuffer(GL_FRAMEBUFFER, entry.msaa_fbo_id);
  CHECK_GL();

  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureEntry.msaa_tex_id);
  CHECK_GL();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureEntry.msaa_tex_id, 0);
  CHECK_GL();

  glDrawBuffers(1, drawBuffers);
  CHECK_GL();

  result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (result != GL_FRAMEBUFFER_COMPLETE)
    FATAL("Error creating MSAA FBO, this usually fails if your DPI scale is invalid or View dimensions are massive: " << result);
  CHECK_GL();
}

void GPUDriverGL::BindRenderBuffer(uint32_t render_buffer_id) {
  // All FBOs are in linear gamma, we need to convert the final framebuffer
  // to sRGB before presenting on screen. Enable sRGB conversion if we are
  // binding the main render buffer (id == 0)
  if (render_buffer_id == 0)
    glEnable(GL_FRAMEBUFFER_SRGB);
  else
    glDisable(GL_FRAMEBUFFER_SRGB);

  RenderBufferEntry& entry = render_buffer_map[render_buffer_id];

  if (context_->msaa_enabled()) {
    // We use the MSAA FBO when doing multisampled rendering.
    // The other FBO (entry.fbo_id) is used for resolving.
    glBindFramebuffer(GL_FRAMEBUFFER, entry.msaa_fbo_id);
    entry.needs_resolve = true;
  } else {
    glBindFramebuffer(GL_FRAMEBUFFER, entry.fbo_id);
  }

  CHECK_GL();
}

void GPUDriverGL::ClearRenderBuffer(uint32_t render_buffer_id) {
  BindRenderBuffer(render_buffer_id);
  glDisable(GL_SCISSOR_TEST);
  CHECK_GL();
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  CHECK_GL();
  glClear(GL_COLOR_BUFFER_BIT);
  CHECK_GL();
}

void GPUDriverGL::DestroyRenderBuffer(uint32_t render_buffer_id) {
  if (render_buffer_id == 0)
    return;
  RenderBufferEntry& entry = render_buffer_map[render_buffer_id];
  glDeleteFramebuffers(1, &entry.fbo_id);
  if (context_->msaa_enabled())
    glDeleteFramebuffers(1, &entry.msaa_fbo_id);
}

void GPUDriverGL::CreateGeometry(uint32_t geometry_id,
  const VertexBuffer& vertices,
  const IndexBuffer& indices) {
  GeometryEntry geometry;
  glGenVertexArrays(1, &geometry.vao);
  glBindVertexArray(geometry.vao);

  glGenBuffers(1, &geometry.vbo_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, geometry.vbo_vertices);
  glBufferData(GL_ARRAY_BUFFER, vertices.size, vertices.data, GL_DYNAMIC_DRAW);

  if (vertices.format == kVertexBufferFormat_2f_4ub_2f_2f_28f) {
    GLsizei stride = 140;

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (GLvoid*)8);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)12);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)20);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)28);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)44);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)60);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)76);
    glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)92);
    glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)108);
    glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)124);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glEnableVertexAttribArray(7);
    glEnableVertexAttribArray(8);
    glEnableVertexAttribArray(9);
    glEnableVertexAttribArray(10);

    CHECK_GL();
  } else if (vertices.format == kVertexBufferFormat_2f_4ub_2f) {
    GLsizei stride = 20;

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (GLvoid*)8);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)12);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    CHECK_GL();
  } else {
    FATAL("Unhandled vertex format: " << vertices.format);
  }

  glGenBuffers(1, &geometry.vbo_indices);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.vbo_indices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size, indices.data,
    GL_STATIC_DRAW);
  CHECK_GL();

  geometry_map[geometry_id] = geometry;

  glBindVertexArray(0);
  CHECK_GL();
}

void GPUDriverGL::UpdateGeometry(uint32_t geometry_id,
  const VertexBuffer& vertices,
  const IndexBuffer& indices) {
  GeometryEntry& geometry = geometry_map[geometry_id];
  glBindVertexArray(geometry.vao);
  CHECK_GL();
  glBindBuffer(GL_ARRAY_BUFFER, geometry.vbo_vertices);
  glBufferData(GL_ARRAY_BUFFER, vertices.size, vertices.data, GL_DYNAMIC_DRAW);
  CHECK_GL();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.vbo_indices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size, indices.data, GL_STATIC_DRAW);
  CHECK_GL();
  glBindVertexArray(0);
  CHECK_GL();
}

void GPUDriverGL::DrawGeometry(uint32_t geometry_id,
  uint32_t indices_count,
  uint32_t indices_offset,
  const GPUState& state) {
  if (programs_.empty())
    LoadPrograms();

  BindRenderBuffer(state.render_buffer_id);

  SetViewport(state.viewport_width, state.viewport_height);

  GeometryEntry& geometry = geometry_map[geometry_id];
  SelectProgram((ProgramType)state.shader_type);
  UpdateUniforms(state);
  
  CHECK_GL();

  glBindVertexArray(geometry.vao);

  CHECK_GL();

  BindTexture(0, state.texture_1_id);
  BindTexture(1, state.texture_2_id);
  BindTexture(2, state.texture_3_id);

  CHECK_GL();

  if (state.enable_scissor) {
    glEnable(GL_SCISSOR_TEST);
    const Rect& r = state.scissor_rect;
    float scale = context_->scale();
    glScissor(r.left * scale, r.top * scale, (r.right - r.left) * scale, (r.bottom - r.top) * scale);
  } else {
    glDisable(GL_SCISSOR_TEST);
  }

  if (state.enable_blend)
    glEnable(GL_BLEND);
  else
    glDisable(GL_BLEND);

  glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT,
    (GLvoid*)(indices_offset * sizeof(unsigned int)));

  glBindVertexArray(0);

  batch_count_++;

  CHECK_GL();
}

void GPUDriverGL::DestroyGeometry(uint32_t geometry_id) {
  GeometryEntry& geometry = geometry_map[geometry_id];

  glDeleteBuffers(1, &geometry.vbo_indices);
  glDeleteBuffers(1, &geometry.vbo_vertices);

  glDeleteVertexArrays(1, &geometry.vao);

  geometry_map.erase(geometry_id);
}

void GPUDriverGL::UpdateCommandList(const CommandList& list) {
  if (list.size) {
    command_list.resize(list.size);
    memcpy(&command_list[0], list.commands, sizeof(Command) * list.size);
  }
}

void GPUDriverGL::DrawCommandList() {
  if (command_list.empty())
    return;

  CHECK_GL();

  batch_count_ = 0;

  glEnable(GL_BLEND);
  glDisable(GL_SCISSOR_TEST);
  glDisable(GL_DEPTH_TEST);
  glDepthFunc(GL_NEVER);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  CHECK_GL();

  for (auto i = command_list.begin(); i != command_list.end(); ++i) {
    switch (i->command_type) {
    case kCommandType_DrawGeometry:
      DrawGeometry(i->geometry_id, i->indices_count, i->indices_offset, i->gpu_state);
      break;
    case kCommandType_ClearRenderBuffer:
      ClearRenderBuffer(i->gpu_state.render_buffer_id);
      break;
    };
  }

  command_list.clear();
  glDisable(GL_SCISSOR_TEST);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GPUDriverGL::BindUltralightTexture(uint32_t ultralight_texture_id) {
  TextureEntry& entry = texture_map[ultralight_texture_id];
  if (context_->msaa_enabled() && entry.render_buffer_id) {
    RenderBufferEntry& renderBufferEntry = render_buffer_map[entry.render_buffer_id];
    if (renderBufferEntry.needs_resolve) {
      GLint drawFboId = 0, readFboId = 0;
      glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
      glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFboId);
      CHECK_GL();
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderBufferEntry.fbo_id);
      glBindFramebuffer(GL_READ_FRAMEBUFFER, renderBufferEntry.msaa_fbo_id);
      CHECK_GL();
      glBlitFramebuffer(0, 0, entry.width, entry.height, 0, 0, entry.width, entry.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
      CHECK_GL();
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFboId);
      glBindFramebuffer(GL_READ_FRAMEBUFFER, readFboId);

      renderBufferEntry.needs_resolve = false;
    }
  }

  glBindTexture(GL_TEXTURE_2D, entry.tex_id);
  CHECK_GL();
}

void GPUDriverGL::LoadPrograms(void) {
  LoadProgram(ultralight::kShaderType_Fill);
  LoadProgram(ultralight::kShaderType_FillPath);
}

void GPUDriverGL::DestroyPrograms(void) {
  GLenum ErrorCheckValue = glGetError();
  glUseProgram(0);
  for (auto i = programs_.begin(); i != programs_.end(); i++) {
    ProgramEntry& prog = i->second;
    glDetachShader(prog.program_id, prog.vert_shader_id);
    glDetachShader(prog.program_id, prog.frag_shader_id);
    glDeleteShader(prog.vert_shader_id);
    glDeleteShader(prog.frag_shader_id);
    glDeleteProgram(prog.program_id);
  }
  programs_.clear();
}

void GPUDriverGL::LoadProgram(ProgramType type) {
  GLenum ErrorCheckValue = glGetError();
  ProgramEntry prog;
  if (type == kShaderType_Fill)
  {
    prog.vert_shader_id = LoadShaderFromSource(GL_VERTEX_SHADER,
      shader_v2f_c4f_t2f_t2f_d28f_vert().c_str(), "shader_v2f_c4f_t2f_t2f_d28f.vert");
    prog.frag_shader_id = LoadShaderFromSource(GL_FRAGMENT_SHADER,
      shader_fill_frag().c_str(), "shader_fill.frag");
  }
  else if (type == kShaderType_FillPath) {
    prog.vert_shader_id = LoadShaderFromSource(GL_VERTEX_SHADER,
      shader_v2f_c4f_t2f_vert().c_str(), "shader_v2f_c4f_t2f.vert");
    prog.frag_shader_id = LoadShaderFromSource(GL_FRAGMENT_SHADER,
      shader_fill_path_frag().c_str(), "shader_fill_path.frag");
  }

  prog.program_id = glCreateProgram();
  glAttachShader(prog.program_id, prog.vert_shader_id);
  glAttachShader(prog.program_id, prog.frag_shader_id);

  glBindAttribLocation(prog.program_id, 0, "in_Position");
  glBindAttribLocation(prog.program_id, 1, "in_Color");
  glBindAttribLocation(prog.program_id, 2, "in_TexCoord");

  if (type == kShaderType_Fill) {
    glBindAttribLocation(prog.program_id, 3, "in_ObjCoord");
    glBindAttribLocation(prog.program_id, 4, "in_Data0");
    glBindAttribLocation(prog.program_id, 5, "in_Data1");
    glBindAttribLocation(prog.program_id, 6, "in_Data2");
    glBindAttribLocation(prog.program_id, 7, "in_Data3");
    glBindAttribLocation(prog.program_id, 8, "in_Data4");
    glBindAttribLocation(prog.program_id, 9, "in_Data5");
    glBindAttribLocation(prog.program_id, 10, "in_Data6");
  }

  glLinkProgram(prog.program_id);
  glUseProgram(prog.program_id);

  if (type == kShaderType_Fill) {
    glUniform1i(glGetUniformLocation(prog.program_id, "Texture1"), 0);
    glUniform1i(glGetUniformLocation(prog.program_id, "Texture2"), 1);
    glUniform1i(glGetUniformLocation(prog.program_id, "Texture3"), 2);
  }

  if (glGetError())
    FATAL("Unable to link shader.\n\tError:" << glErrorString(glGetError()) << "\n\tLog: " << GetProgramLog(prog.program_id))
 
  programs_[type] = prog;
}

void GPUDriverGL::SelectProgram(ProgramType type) {
  auto i = programs_.find(type);
  if (i != programs_.end()) {
    cur_program_id_ = i->second.program_id;
    glUseProgram(i->second.program_id);
  } else {
    FATAL("Missing shader type: " << type);
  }
}

void GPUDriverGL::UpdateUniforms(const GPUState& state) {
  bool flip_y = state.render_buffer_id != 0;
  Matrix model_view_projection = ApplyProjection(state.transform, state.viewport_width, state.viewport_height, flip_y);

  float params[4] = { (float)(glfwGetTime() / 1000.0), state.viewport_width, state.viewport_height, context_->scale() };
  SetUniform4f("State", params);
  CHECK_GL();
  ultralight::Matrix4x4 mat = model_view_projection.GetMatrix4x4();
  SetUniformMatrix4fv("Transform", 1, mat.data);
  CHECK_GL();
  SetUniform4fv("Scalar4", 2, &state.uniform_scalar[0]);
  CHECK_GL();
  SetUniform4fv("Vector", 8, &state.uniform_vector[0].value[0]);
  CHECK_GL();
  SetUniform1ui("ClipSize", state.clip_size);
  CHECK_GL();
  SetUniformMatrix4fv("Clip", 8, &state.clip[0].data[0]);
}

void GPUDriverGL::SetUniform1ui(const char* name, GLuint val) {
  glUniform1ui(glGetUniformLocation(cur_program_id_, name), val);
}

void GPUDriverGL::SetUniform1f(const char* name, float val) {
  glUniform1f(glGetUniformLocation(cur_program_id_, name), (GLfloat)val);
}

void GPUDriverGL::SetUniform1fv(const char* name, size_t count, const float* val) {
  glUniform1fv(glGetUniformLocation(cur_program_id_, name), (GLsizei)count, val);
}

void GPUDriverGL::SetUniform4f(const char* name, const float val[4]) {
  glUniform4f(glGetUniformLocation(cur_program_id_, name),
    (GLfloat)val[0], (GLfloat)val[1], (GLfloat)val[2], (GLfloat)val[3]);
}

void GPUDriverGL::SetUniform4fv(const char* name, size_t count, const float* val) {
  glUniform4fv(glGetUniformLocation(cur_program_id_, name), (GLsizei)count, val);
}

void GPUDriverGL::SetUniformMatrix4fv(const char* name, size_t count, const float* val) {
  glUniformMatrix4fv(glGetUniformLocation(cur_program_id_, name), (GLsizei)count, false, val);
}

void GPUDriverGL::SetViewport(float width, float height) {
  glViewport(0, 0, static_cast<GLsizei>(width * context_->scale()),
                   static_cast<GLsizei>(height * context_->scale()));
}

Matrix GPUDriverGL::ApplyProjection(const Matrix4x4& transform, float screen_width, float screen_height, bool flip_y) {
  Matrix transform_mat;
  transform_mat.Set(transform);

  Matrix result;
  result.SetOrthographicProjection(screen_width, screen_height, flip_y);
  result.Transform(transform_mat);

  return result;
}

void GPUDriverGL::CreateFBOTexture(uint32_t texture_id, Ref<Bitmap> bitmap) {
  CHECK_GL();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  CHECK_GL();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  CHECK_GL();

  TextureEntry& entry = texture_map[texture_id];
  entry.width = bitmap->width();
  entry.height = bitmap->height();

  // Allocate a single-sampled texture
  glGenTextures(1, &entry.tex_id);
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, entry.tex_id);
  
  // Allocate texture in linear space.
  // We will convert back to sRGB for monitor when binding renderbuffer 0
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, entry.width, entry.height, 0,
    GL_BGRA, GL_UNSIGNED_BYTE, nullptr);

  if (context_->msaa_enabled()) {
    // Allocate the multisampled texture
    glGenTextures(1, &entry.msaa_tex_id);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, entry.msaa_tex_id);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, entry.width, entry.height, false);
  }

  CHECK_GL();
  glGenerateMipmap(GL_TEXTURE_2D);
  CHECK_GL();
}


}  // namespace ultralight
