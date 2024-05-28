#pragma once

#include <SDL.h>
#include <glm/glm.hpp>

class gCamera
{
public:
	gCamera(void);
	gCamera(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up);
	~gCamera(void);

	/// <summary>
	/// Gets the view matrix.
	/// </summary>
	/// <returns>The 4x4 view matrix</returns>
	glm::mat4 GetViewMatrix();

	void Update(float _deltaTime);

	void SetView(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up);
	void SetProj(float _angle, float _aspect, float _zn, float _zf);
	void LookAt(glm::vec3 _at);
	void moveBack();

	glm::vec3 getForward() { return m_forward; }
	glm::vec3 getUp() { return m_up; }
	glm::vec3 getRight() { return m_right; }

	void SetSpeed(float _val);
	glm::vec3 GetEye()
	{
		return m_eye;
	}

	glm::vec3 GetAt()
	{
		return m_at;
	}

	glm::vec3 GetUp()
	{
		return m_worldUp;
	}

	glm::mat4 GetProj()
	{
		return m_matProj;
	}

	glm::mat4 GetViewProj()
	{
		return m_matViewProj;
	}

	void Resize(int _w, int _h);

	void KeyboardDown(SDL_KeyboardEvent& key);
	void KeyboardUp(SDL_KeyboardEvent& key);
	void MouseMove(SDL_MouseMotionEvent& mouse);

private:
	/// <summary>
	/// Updates the UV.
	/// </summary>
	/// <param name="du">The du, i.e. the change of spherical coordinate u.</param>
	/// <param name="dv">The dv, i.e. the change of spherical coordinate v.</param>
	void UpdateUV(float du, float dv);

	/// <summary>
	///  The traversal speed of the camera
	/// </summary>
	float		m_speed;
	/// <summary>
	/// The view matrix of the camera
	/// </summary>
	glm::mat4	m_viewMatrix;

	glm::mat4	m_matViewProj;

	bool	m_slow;

	/// <summary>
	/// The camera position.
	/// </summary>
	glm::vec3	m_eye;
	glm::vec3	m_oldPos;
	glm::vec3   m_oldDeltaPos;

	/// <summary>
	/// The vector pointing upwards
	/// </summary>
	glm::vec3	m_worldUp;

	/// <summary>
	/// The camera look at point.
	/// </summary>
	glm::vec3	m_at;

	/// <summary>
	/// The u spherical coordinate of the spherical coordinate pair (u,v) denoting the
	/// current viewing direction from the view position m_eye. 
	/// </summary>
	float	m_u;

	/// <summary>
	/// The v spherical coordinate of the spherical coordinate pair (u,v) denoting the
	/// current viewing direction from the view position m_eye. 
	/// </summary>
	float	m_v;

	/// <summary>
	/// The distance of the look at point from the camera. 
	/// </summary>
	float	m_distance;

	/// <summary>
	/// The unit vector pointing towards the viewing direction.
	/// </summary>
	glm::vec3	m_forward;
	/// <summary>
	/// The unit vector pointing to the 'right'
	/// </summary>
	glm::vec3	m_right;
	/// <summary>
	/// The vector pointing upwards
	/// </summary>
	glm::vec3   m_up;

	glm::vec3   m_moveDirUp;
	glm::vec3   m_moveDirRight;
	glm::vec3   m_moveDirForward;


	glm::mat4	m_matProj;

	float	m_goForward;
	float	m_goRight;
	float   m_goUp;
};

