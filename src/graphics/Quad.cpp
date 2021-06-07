#ifndef XEN_QUAD_CPP
#define XEN_QUAD_CPP

#include <GLEW/GL/glew.h>
#include <iostream>

#include "Quad.h"

namespace xen
{
	Quad::Quad() :
		left(0),
		top(0),
		width(1),
		height(1),
		vbo(-1),
		ibo(-1)
	{ }



	Quad::Quad(float rectLeft, float rectTop, float rectWidth, float rectHeight) :
		left(rectLeft),
		top(rectTop),
		width(rectWidth),
		height(rectHeight),
		vbo(-1),
		ibo(-1)
	{ }



	void GLAPIENTRY
		MessageCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}



	void Quad::initialize()
	{
		// During init, enable debug output
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices_new, GL_DYNAMIC_DRAW);
		std::cout << "VBO Debug: " << glGetError() << std::endl;

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
		std::cout << "IBO Debug: " << glGetError() << std::endl;

		/* Specify the vertex layout (pos + uv + color). */
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(2 * sizeof(GLfloat)));
	}



	void Quad::draw()
	{
		/* vertices_new[0].x++;
		vertices_new[1].x++;
		vertices_new[2].x++;
		vertices_new[3].x++; */

		// prepare vertex buffer.
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices_new, GL_DYNAMIC_DRAW);

		// assign the index buffer.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		// draw stuff.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	} 
}

#endif