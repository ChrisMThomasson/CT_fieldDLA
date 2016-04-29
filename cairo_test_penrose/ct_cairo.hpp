#if ! defined (__cplusplus)
#error C++ compiler required!
#elif ! defined (CT_CAIRO_HPP)
#define CT_CAIRO_HPP 0x1
#include "ct_master.hpp"
#include <cairo.h>


namespace ct
{
	struct canvas
	{
		cairo_t* m_cairo;
		cairo_surface_t* m_surface;
		unsigned int m_width;
		unsigned int m_height;
		glm::vec3 m_center;
		float m_radius;

		canvas(unsigned int width, unsigned int height)
		:	m_cairo(nullptr),
			m_surface(nullptr),
			m_width(width),
			m_height(height),
			m_center(m_width / 2.0f, m_height / 2.0f, 0.0f),
			m_radius(height / 2.0f)
		{
			m_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
			if (! m_surface) throw;

			m_cairo = cairo_create(m_surface);
			if (! m_cairo)
			{
				cairo_surface_destroy(m_surface);
				throw;
			}
        }

		~canvas() throw()
		{
			cairo_destroy(m_cairo);
			cairo_surface_destroy(m_surface);
		}

		void fill_background(glm::vec4 const& color)
		{
			cairo_rectangle(m_cairo, 0.0, 0.0, m_width, m_height);
			cairo_set_source_rgba(m_cairo, color.r, color.g, color.b, color.a);
			cairo_fill(m_cairo);
		}

		void save_png(const char* fname)
		{
			cairo_surface_write_to_png(m_surface, fname);
		}


		void set_color(glm::vec4 const& color)
		{
			cairo_set_source_rgba(m_cairo, color.r, color.g, color.b, color.a);
		}


		void circle(
			glm::vec3 const& center,
			float radius,
			glm::vec4 const& color
		){
			cairo_new_path(m_cairo);
			cairo_arc(m_cairo, center.x, center.y, radius, 0, PI2);
			set_color(color);
			cairo_set_line_width(m_cairo, 2.0);
			cairo_stroke(m_cairo);
		}


		void line(
			glm::vec3 const& p0,
			glm::vec3 const& p1,
			glm::vec4 const& color,
			float lwidth = 2.0f
		){
			cairo_new_path(m_cairo);
			cairo_move_to(m_cairo, p0.x, p0.y);
			cairo_line_to(m_cairo, p1.x, p1.y);
			set_color(color);
			cairo_set_line_width(m_cairo, lwidth);
			cairo_stroke(m_cairo);
		}
	};

}


#endif
