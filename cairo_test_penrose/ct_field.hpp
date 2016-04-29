#if ! defined (__cplusplus)
#error C++ compiler required!
#elif ! defined (CT_FIELD_HPP)
#define CT_FIELD_HPP 0x1
#include "ct_cairo.hpp"
#include <vector>



namespace ct
{
	struct field
	{
		struct point
		{
			float m_mass;
			unsigned int m_hits;
			glm::vec3 m_pos;
			glm::vec4 m_color;
			unsigned int m_flags;
		};

		#define CT_FIELD_PT_RAW(mp_mass, mp_hits, mp_pos, mp_color, m_flags) \
			{ (mp_mass), (mp_hits), (mp_pos), (mp_color), (m_flags) }

		#define CT_FIELD_PT(mp_mass, mp_pos, mp_color, m_flags) \
			ct::field::point CT_FIELD_PT_RAW(mp_mass, 0, mp_pos, mp_color, m_flags)

		typedef std::vector<point> point_vec_t;


		field()
		{

		}


		~field() throw()
		{

		}


		point*
		search_hit(point_vec_t& pvec, glm::vec3 const& pt, float esp)
		{
			unsigned int imax = pvec.size();

			for (unsigned int i = 0; i < imax; ++i)
			{
				point& p = pvec[i];
				if (p.m_mass > 0.0f) continue;
				float d = glm::distance(p.m_pos, pt);
				if (d < esp) return &p;
			}

			return nullptr;
		}


		glm::vec3 
		calc_point(
			point_vec_t const& pvec, 
			glm::vec3 const& op, 
			float npow
		){
			glm::vec3 g = { 0.0, 0.0, 0.0 };
			unsigned int imax = pvec.size();

			for (unsigned int i = 0; i < imax; ++i)
			{
				glm::vec3 dif = {
					pvec[i].m_pos.x - op.x,
					pvec[i].m_pos.y - op.y,
					pvec[i].m_pos.z - op.z
				};

				float disraw = SQ(dif.x) + SQ(dif.y) + SQ(dif.z);

				float mass = std::pow(disraw, npow);

				//float d = glm::length(dif) / mass;

				if (mass > ESP && ! isnan(mass) && isfinite(mass))
				{
					g.x = g.x + pvec[i].m_mass * dif.x / mass;
					g.y = g.y + pvec[i].m_mass * dif.y / mass;
					g.z = g.z + pvec[i].m_mass * dif.z / mass;
				}
			}

			float dis = std::sqrt(SQ(g.x) + SQ(g.y) + SQ(g.z));

			g.x = g.x / dis;
			g.y = g.y / dis;
			g.z = g.z / dis;

			return g;
		}


		glm::vec3 
		calc_npoint(
			point_vec_t const& pvec, 
			glm::vec3 const& p, 
			float npow,
			float scale
		){
			glm::vec3 g = calc_point(pvec, p, npow);

			g.x = -g.x * scale;
			g.y = -g.y * scale;
			g.z = -g.z * scale;

			glm::vec3 np = {
				p.x + g.x,
				p.y + g.y,
				p.z + g.z
			};

			return np;
		}



		// big, ugly function; needs to be quantified into several parts...
		point_vec_t render(ct::canvas& gc, point_vec_t& pvec, unsigned int imax, bool lines = true)
		{
			assert(imax > 0);

			point_vec_t hits;
			unsigned int ilmax = pvec.size();

			for (unsigned int il = 0; il < ilmax; ++il)
			{
				ct::field::point const& pvpt = pvec[il];
				if (pvpt.m_mass < 0.0f) continue;

				float astep = PI2 / imax;
				float radius = 5.0f;

				for (unsigned int i = 0; i < imax; ++i)
				{
					float angle = astep * i;
					float rangle = PI2 * RAND_FLOAT();

					glm::vec3 p = {
						cos(rangle) * radius + pvpt.m_pos.x,
						sin(rangle) * radius + pvpt.m_pos.y,
						0.0f
					};

					if (lines) gc.line(pvpt.m_pos, p, pvpt.m_color);

					
					{
						unsigned int ixmax = 200;

						for (unsigned int ix = 0; ix < ixmax; ++ix)
						{
							float rradius = 5.0f + 10.0f * RAND_FLOAT();
							glm::vec3 np = calc_npoint(pvec, p, 2.0f, rradius);

							if (lines) gc.line(p, np, pvpt.m_color);

							{
								ct::field::point* hit = search_hit(pvec, np, rradius);

								if (hit)
								{
									assert(hit->m_mass < 0.0f);
									hit->m_hits += 1;
									hit->m_mass += -0.25f;

									{
										std::cout << "hit->m_hits:" << hit->m_hits << "\r";
										ct::field::point nhit = *hit;

										nhit.m_pos = np;
										nhit.m_mass += -0.25f;
										hits.push_back(nhit);
										gc.circle(nhit.m_pos, 1.3f, RGBA(1.0f, 1.0f, 0.0f, 0.5f));
										glm::vec4 color = RGBA(1.0f, 1.0f, 1.0f, 0.8f);
										gc.line(hit->m_pos, nhit.m_pos,
											color, 2.0f);
									}

									break;
								}
							}

							p = np;
						}
					}
				}
			}
			return hits;
		}
	};
}



#endif
