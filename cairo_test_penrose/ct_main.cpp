#include <iostream>
#include <cmath>
#include <cstdlib>
#include "ct_field.hpp"


ct::field::point_vec_t&
ct_circle_field(
	ct::canvas& gc, 
	ct::field& gf,
	ct::field::point_vec_t& pvec,
	float radius,
	float mass,
	unsigned int imax,
	glm::vec4 const& flcolor
){
	assert(imax > 0);

	float astep = PI2 / imax;

	for (unsigned int i = 0; i < imax; ++i)
	{
		float iscale = i / (imax - 1.0f);
		float xxxangle = astep * i;
		float angle = PI2 * RAND_FLOAT();
		float rradius_min = radius * 0.8f;
		float rradius = rradius_min + RAND_FLOAT() * (radius - rradius_min);

		glm::vec3 p;
		float rn = RAND_FLOAT();
		
		if (rn < 0.25f)
		{
			// figure 8 x-axis
			p = glm::vec3 {
				cos(angle) * fabs(cos(angle)) * radius + gc.m_center.x,
				sin(angle) * fabs(cos(angle)) * radius + gc.m_center.y,
				0.0f
			};
		}

		else if(rn < 0.5f)
		{
			// figure 8 y-axis
			p = glm::vec3 {
				cos(angle) * fabs(sin(angle)) * radius + gc.m_center.x,
				sin(angle) * fabs(sin(angle)) * radius + gc.m_center.y,
				0.0f
			};
		}

		else if (rn < 0.75f)
		{
			// square
			p = glm::vec3{
				cos(angle) * fabs(cos(angle)) * radius + gc.m_center.x,
				sin(angle) * fabs(sin(angle)) * radius + gc.m_center.y,
				0.0f
			};
		}

		else 
		{
			// circle
			p = glm::vec3{
				cos(angle) * radius + gc.m_center.x,
				sin(angle) * radius + gc.m_center.y,
				0.0f
			};
		}
		
		// add the point to the field
		pvec.push_back(CT_FIELD_PT_RAW(mass, 0, p, flcolor, 0));
	}

	return pvec;
}


glm::vec3
line_scale(
	glm::vec3 const& p0,
	glm::vec3 const& p1,
	float scale
){
	glm::vec3 sp = {
		p0.x + (p1.x - p0.x) * scale,
		p0.y + (p1.y - p0.y) * scale,
		p0.z + (p1.z - p0.z) * scale,
	};
	
	return sp;
}


int main()
{
	std::srand(12345);

	{
		ct::canvas gc(1024, 1024);
		ct::field gf;
		ct::field::point_vec_t pvec;
		float radius = gc.m_radius;

		gc.fill_background(glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });

		{
			glm::vec3 p = gc.m_center;
			pvec.push_back(CT_FIELD_PT_RAW(-1.0f, 0, p, 
				RGBA(0.0f, 1.0f, 1.0f, 0.2f), 0));
		}

		{
			unsigned int imax = 100;
			unsigned int rline_max = 30;
			assert(imax > 30);

			for (unsigned int i = 0; i < imax; ++i)
			{
				unsigned ipmax = 17;
				float rn = RAND_FLOAT();
				float alpha = 0.1f;

				ct_circle_field(gc, gf, pvec, radius * 1.0f,
					1.0f, ipmax, RGBA(1.0f, 0.0f, 0.0f, alpha));

				bool lines = true; // false;
				if (i > imax - 30) lines = true;
				ct::field::point_vec_t hvec = gf.render(gc, pvec, 1, lines);

				for (unsigned int ip = 0; ip < ipmax; ++ip)
				{
					pvec.pop_back();
				}

				pvec.insert(pvec.end(), hvec.begin(), hvec.end());
				std::cout << "Iterated: " << i << " of " << imax 
						  << " with N:" << pvec.size() << std::endl;
			}
		}
		
		gc.save_png("output.png");
	}

	std::cout << "Complete" << std::endl;
	std::system("mspaint output.png");

	return 0;
}
