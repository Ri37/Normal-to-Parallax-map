#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Utils
#include "gCamera.h"

#include "ProgramObject.h"
#include "BufferObject.h"
#include "VertexArrayObject.h"
#include "TextureObject.h"

// mesh
#include "ObjParser_OGL3.h"

class CMyApp
{
public:
	CMyApp();
	~CMyApp();

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);

protected:
	// shaderekhez szükséges változók
	ProgramObject*		m_programBump   = nullptr;		 // normal   map shader
	ProgramObject*		m_programNormal = nullptr;	     // bump     map shader
	ProgramObject		m_programParallax;				 // parallax map shader

	// ground
	VertexArrayObject	m_GroundVao;			// VAO
	IndexBuffer			m_GroundIndices;		// index buffer
	ArrayBuffer			m_GroundVertexBuffer;	// VBO

	// side
	VertexArrayObject	m_SideVao;			// VAO
	IndexBuffer			m_SideIndices;		// index buffer
	ArrayBuffer			m_SideVertexBuffer;	// VBO

	// Kamera
	gCamera m_camera;

	// Texture
	Texture2D			m_groundTexture;
	Texture2D			m_sideTexture;
	Texture2D			m_normalTexture;
	Texture2D			m_heightTexture;
	Texture2D			m_generatedHeightTexture;

	struct Vertex
	{
		glm::vec3 pos;      // Pozíció
		glm::vec3 norm;     // Normálvektor
		glm::vec2 tex;      // Textúra koordináták
		glm::vec3 tangent;  // Tangenciális vektor
		glm::vec3 bitangent;// Bitangenciális vektor
	};

	// Init
	void InitShaders();
	void InitGround();
	void InitSide();

	// Render
	void RenderScene();
	void RenderMapping(ProgramObject*);
	void RenderParallaxMap();

	std::string LoadShaderSource(const std::string&);
	ProgramObject CreateProgramWithDefine(const std::string&);
	
	
	int w, h;
	
	// ImGUI
	bool shaderSwitch	  = true;
	bool isParallaxActive = false;
	bool displayGenerated = false;

	float bumpIntensity = 0.1;
	float parallaxIntensity = 0.05;

	// kellékek
	float m_ElapsedTime = 0.0;
	float m_currentParam = 0.0;
	
	glm::vec4 m_lightPos = glm::vec4(0.0, 5.0, 0.0, 0.0);
};

