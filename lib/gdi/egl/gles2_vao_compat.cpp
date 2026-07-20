/* ZHE Vision — GLES2 VAO link compatibility.
 *
 * The e2egl shaders use Vertex Array Objects, which are GLES3 core but only an
 * (optional) extension on GLES2. Every VAO call in the shader code is guarded at
 * runtime by gles::isGLES3(), so on a GLES2-only driver they are never executed.
 * However the linker still needs the symbols resolved.
 *
 * This box's Broadcom VideoCore V3D libGLESv2 is GLES2-only and exports no VAO
 * entry points, so we provide WEAK no-op fallbacks. On a GLES3-capable driver the
 * real (strong) symbols from libGLESv2 override these. This keeps a single shader
 * codebase that links and runs on both GLES2 and GLES3 targets.
 */
#ifdef HAVE_EGL
#include <GLES2/gl2.h>

extern "C" {

__attribute__((weak)) void glGenVertexArrays(GLsizei /*n*/, GLuint * /*arrays*/) {}
__attribute__((weak)) void glBindVertexArray(GLuint /*array*/) {}
__attribute__((weak)) void glDeleteVertexArrays(GLsizei /*n*/, const GLuint * /*arrays*/) {}

}
#endif
