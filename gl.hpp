/**
 * @file gl.hpp
 * @brief Resolve the inclusion of OpenGL headers.
 *
 * @author Phillip Kilgore
 */

#ifndef GL_HPP

/* OSX has a wart where OpenGL is located in a nonstandard directory,
 * so we need to use an alternate path. We'll also load 
 */
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <OpenGL/agl.h>
#define SYS_GL_AGL 1

/* According to https://www.opengl.org/registry/, the OpenGL Headers
 * should be located in a directory named "GL". 
 */
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/* Load platform-specific includes and determine flavor */
extern "C" {
#ifdef __unix__
#define SYS_GL_GLX 1
#elif defined(_WIN32) || defined(_WIN64)
#define SYS_GL_WGL 1 
#endif
#endif
}
#endif
