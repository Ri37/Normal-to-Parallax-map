#include <iostream>
#include "gCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

/// <summary>
/// Initializes a new instance of the <see cref="gCamera"/> class.
/// </summary>
gCamera::gCamera(void) : m_eye(0.0f, 20.0f, 20.0f), m_at(0.0f), m_worldUp(0.0f, 1.0f, 0.0f), m_speed(16.0f), m_goForward(0), m_goRight(0), m_goUp(0.0), m_slow(false)
{
	SetView(glm::vec3(0, 20, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	m_distance = glm::length(m_at - m_eye);

	SetProj(glm::radians(60.0f), 640 / 480.0f, 0.01f, 1000.0f);
}

gCamera::gCamera(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _worldUp) : m_speed(16.0f), m_goForward(0), m_goRight(0), m_distance(10), m_slow(false)
{
	SetView(_eye, _at, _worldUp);
}

gCamera::~gCamera(void)
{
}

void gCamera::SetView(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _worldUp)
{
	m_eye = _eye;
	m_at = _at;
	m_worldUp = _worldUp;

	m_forward = glm::normalize(m_at - m_eye);
	m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
	m_up = glm::cross(m_right, m_forward);

	m_distance = glm::length(m_at - m_eye);

	m_u = atan2f(m_forward.z, m_forward.x);
	m_v = acosf(m_forward.y);
}

void gCamera::SetProj(float _angle, float _aspect, float _zn, float _zf)
{
	m_matProj = glm::perspective(_angle, _aspect, _zn, _zf);
	m_matViewProj = m_matProj * m_viewMatrix;
}

glm::mat4 gCamera::GetViewMatrix()
{
	return m_viewMatrix;
}

void gCamera::Update(float _deltaTime)
{
	glm::vec3 deltaPosition = (m_goForward * m_forward + m_goRight * m_right + m_goUp * m_up) * m_speed * _deltaTime;
	m_eye += deltaPosition;
	m_at += deltaPosition;

	m_viewMatrix = glm::lookAt(m_eye, m_at, m_worldUp);
	m_matViewProj = m_matProj * m_viewMatrix;
}

void gCamera::UpdateUV(float du, float dv)
{
	m_u += du;
	m_v = glm::clamp<float>(m_v + dv, 0.1f, 3.1f);

	m_at = m_eye + m_distance * glm::vec3(cosf(m_u) * sinf(m_v),
		cosf(m_v),
		sinf(m_u) * sinf(m_v));

	m_forward = glm::normalize(m_at - m_eye);
	m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
	m_up = glm::cross(m_right, m_forward);
}

void gCamera::SetSpeed(float _val)
{
	m_speed = _val;
}

void gCamera::Resize(int _w, int _h)
{
	SetProj(glm::radians(60.0f), _w / (float)_h, 0.01f, 1000.0f);
}

void gCamera::KeyboardDown(SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym)
	{
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		if (!m_slow)
		{
			m_slow = true;
			m_speed /= 4.0f;
		}
		break;
	case SDLK_w:
		m_goForward = 1;
		break;
	case SDLK_s:
		m_goForward = -1;
		break;
	case SDLK_a:
		m_goRight = -1;
		break;
	case SDLK_d:
		m_goRight = 1;
		break;
	case SDLK_e:
		m_goUp = 1;
		break;
	case SDLK_q:
		m_goUp = -1;
		break;
	}
}

void gCamera::KeyboardUp(SDL_KeyboardEvent& key)
{
	float current_speed = m_speed;
	switch (key.keysym.sym)
	{
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		if (m_slow)
		{
			m_slow = false;
			m_speed *= 4.0f;
		}
		break;
	case SDLK_w:
	case SDLK_s:
		m_goForward = 0;
		break;
	case SDLK_a:
	case SDLK_d:
		m_goRight = 0;
		break;
	case SDLK_q:
	case SDLK_e:
		m_goUp = 0;
		break;
	}
}

void gCamera::MouseMove(SDL_MouseMotionEvent& mouse)
{
	if (mouse.state & SDL_BUTTON_LMASK)
	{
		UpdateUV(mouse.xrel / 100.0f, mouse.yrel / 100.0f);
	}
}

void gCamera::LookAt(glm::vec3 _at)
{
	SetView(m_eye, _at, m_worldUp);
}

