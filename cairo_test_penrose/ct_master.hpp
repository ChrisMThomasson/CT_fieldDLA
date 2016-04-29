#if ! defined (__cplusplus)
#error C++ compiler required!
#elif ! defined (CT_MASTER_HPP)
#define CT_MASTER_HPP 0x1
#include <cassert>
#include <cstdlib>
#include <glm/glm.hpp>


// globals
#define PI 3.141592654f
#define PI2 (PI * 2.0f)
#define ESP 0.0000001f

#define RAND_FLOAT() (std::rand() / (RAND_MAX - 0.0f))

#define SQ(mp_p) ((mp_p) * (mp_p))

#define RGBA(mp_r, mp_g, mp_b, mp_a)	\
	glm::vec4{ (mp_r), (mp_g), (mp_b), (mp_a) }

#define RGB(mp_r, mp_g, mp_b)	\
	glm::vec4{ (mp_r), (mp_g), (mp_b), 1.0f }


#endif
