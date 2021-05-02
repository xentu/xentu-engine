#ifndef XEN_QUAD_HPP
#define XEN_QUAD_HPP

#include "Color.hpp"
#include "Rect.hpp"
#include "Texture.hpp"
#include "Transformable.hpp"
#include "Vertex.hpp"

namespace xen
{
	class Quad
	{
	public:
		Quad();
		Quad(float rectLeft, float rectTop, float rectWidth, float rectHeight);

		void initialize();
		void draw();

	private:
		float left;       //!< Left coordinate of the rectangle.
		float top;        //!< Top coordinate of the rectangle.
		float width;      //!< Width of the rectangle.
		float height;     //!< Height of the rectangle.

		unsigned int vbo; //!< the vertex buffer.
		unsigned int ibo; //!< the index buffer.

		unsigned int indices[6] = {
			0,1,2,
			2,3,0
		};

		float vertices[16] = {
			  0.0f,   0.0f,  0.0f, 1.0f, // top-left
			100.0f,   0.0f,  1.0f, 1.0f, // top-right
			100.0f, 100.0f,  1.0f, 0.0f, // bottom-right
			  0.0f, 100.0f,  0.0f, 0.0f, // bottom-left
		};

		Vertex vertices_new[4] = {
			Vertex(   0.0f,   0.0f, 0.0f, 1.0f),
			Vertex( 100.0f,   0.0f, 1.0f, 1.0f),
			Vertex( 100.0f, 100.0f, 1.0f, 0.0f),
			Vertex(   0.0f, 100.0f, 0.0f, 0.0f)
		};
	};
}

#endif