#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <time.h>
#include <list>
#include <tuple>
#include <imgui/imgui.h>
#include "GLUtils.hpp"

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(-10, 5, 0), glm::vec3(0, 5, 0), glm::vec3(0, 1, 0));
}

CMyApp::~CMyApp(void)
{
}

void CMyApp::InitGround()
{
	glm::vec3 tangent(1, 0, 0);    // X irányú tangenciális vektor
	glm::vec3 bitangent(0, 1, 0);  // Y irányú bitangenciális vektor

	std::vector<Vertex> vertices =
	{
		// Csúcspontok meghatározása a tangenciális és bitangenciális vektorokkal
		{ glm::vec3(-0.5, -0.5, 0.0), glm::vec3(0, 0, 1), glm::vec2(0, 0), tangent, bitangent },
		{ glm::vec3(+0.5, -0.5, 0.0), glm::vec3(0, 0, 1), glm::vec2(1, 0), tangent, bitangent },
		{ glm::vec3(-0.5, +0.5, 0.0), glm::vec3(0, 0, 1), glm::vec2(0, 1), tangent, bitangent },
		{ glm::vec3(+0.5, +0.5, 0.0), glm::vec3(0, 0, 1), glm::vec2(1, 1), tangent, bitangent }
	};

	std::vector<int> indices =
	{
		0, 1, 2,
		2, 1, 3
	};

	m_GroundVertexBuffer.BufferData(vertices);
	m_GroundIndices.BufferData(indices);

	m_GroundVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0,                                         sizeof(Vertex)>, m_GroundVertexBuffer },
			{ CreateAttribute<1, glm::vec3, offsetof(Vertex, norm),                    sizeof(Vertex)>, m_GroundVertexBuffer },
			{ CreateAttribute<2, glm::vec2, offsetof(Vertex, tex),                     sizeof(Vertex)>, m_GroundVertexBuffer },
			{ CreateAttribute<3, glm::vec3, offsetof(Vertex, tangent),                 sizeof(Vertex)>, m_GroundVertexBuffer },
			{ CreateAttribute<4, glm::vec3, offsetof(Vertex, bitangent),               sizeof(Vertex)>, m_GroundVertexBuffer },
		},
		m_GroundIndices
		);
}

void CMyApp::InitSide()
{
	// A négyzet síkja az XY sík, Z tengely mentén néz.
	glm::vec3 tangent(1, 0, 0);    // X irányú tangenciális vektor
	glm::vec3 bitangent(0, 1, 0);  // Y irányú bitangenciális vektor

	std::vector<Vertex> vertices =
	{
		// Csúcspontok meghatározása a tangenciális és bitangenciális vektorokkal
		{ glm::vec3(-0.5, -0.5, 0.0), glm::vec3(0, 0, 1), glm::vec2(0, 0), tangent, bitangent },
		{ glm::vec3(+0.5, -0.5, 0.0), glm::vec3(0, 0, 1), glm::vec2(1, 0), tangent, bitangent },
		{ glm::vec3(-0.5, +0.5, 0.0), glm::vec3(0, 0, 1), glm::vec2(0, 1), tangent, bitangent },
		{ glm::vec3(+0.5, +0.5, 0.0), glm::vec3(0, 0, 1), glm::vec2(1, 1), tangent, bitangent }
	};

	std::vector<int> indices =
	{
		0, 1, 2,
		2, 1, 3
	};

	// Feltételezzük, hogy a BufferData() és Init() függvények a megfelelő helyen vannak implementálva.
	m_SideVertexBuffer.BufferData(vertices);
	m_SideIndices.BufferData(indices);

	m_SideVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0,                                         sizeof(Vertex)>, m_SideVertexBuffer },
			{ CreateAttribute<1, glm::vec3, offsetof(Vertex, norm),                    sizeof(Vertex)>, m_SideVertexBuffer },
			{ CreateAttribute<2, glm::vec2, offsetof(Vertex, tex),                     sizeof(Vertex)>, m_SideVertexBuffer },
			{ CreateAttribute<3, glm::vec3, offsetof(Vertex, tangent),                 sizeof(Vertex)>, m_SideVertexBuffer },
			{ CreateAttribute<4, glm::vec3, offsetof(Vertex, bitangent),               sizeof(Vertex)>, m_SideVertexBuffer },
		},
		m_SideIndices
		);
}

std::string CMyApp::LoadShaderSource(const std::string& filename)
{
	std::ifstream file(filename);
	std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return source;
}

ProgramObject CMyApp::CreateProgramWithDefine(const std::string& define) {
	ProgramObject program;

	std::string vertexShaderSource = LoadShaderSource("myVert.vert");
	std::string fragmentShaderSource = LoadShaderSource("myFrag.frag");

	size_t lineEnd = fragmentShaderSource.find('\n') + 1;
	fragmentShaderSource.insert(lineEnd, define + "\n");

	// Compile vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderCStr = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderCStr, NULL);
	glCompileShader(vertexShader);

	// Compile fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderCStr = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderCStr, NULL);
	glCompileShader(fragmentShader);

	// Attach shaders to program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	// Bind attribute locations before linking
	glBindAttribLocation(program, 0, "vs_in_pos");
	glBindAttribLocation(program, 1, "vs_in_norm");
	glBindAttribLocation(program, 2, "vs_in_tex");
	glBindAttribLocation(program, 3, "vs_in_tan");
	glBindAttribLocation(program, 4, "vs_in_bitan");

	// Link the program
	glLinkProgram(program);

	// Detach and delete shaders after linking
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

void CMyApp::InitShaders()
{
	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_programNormal = new ProgramObject(CreateProgramWithDefine("#define USE_NORMAL_MAP"));
	m_programBump   = new ProgramObject(CreateProgramWithDefine("#define USE_BUMP_MAP"));
	
	//m_program = CreateProgramWithDefine("#define USE_NORMAL_MAP");

	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_programParallax.AttachShaders({
		{ GL_VERTEX_SHADER, "parallax.vert"},
		{ GL_FRAGMENT_SHADER, "parallax.frag"}
		});

	// attributomok osszerendelese a VAO es shader kozt
	m_programParallax.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
		{ 2, "vs_in_tex" },				// VAO 2-es csatorna menjen a vs_in_tex-be
		{ 3, "vs_in_tan" },				// VAO 3-as csatorna menjen a vs_in_tan-ba
		{ 4, "vs_in_bitan" }			// VAO 4-es csatorna menjen a vs_in_bitan-ba
		});

	m_programParallax.LinkProgram();
}

bool CMyApp::Init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	glLineWidth(4.0f);  // a vonalprimitívek vastagsága: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glLineWidth.xhtml
	glPointSize(15.0f); // a raszterizált pontprimitívek mérete

	InitShaders();
	InitGround();
	InitSide();

	// egyéb textúrák betöltése
	m_groundTexture.FromFile("assets/wood.png");
	m_sideTexture.FromFile("assets/marble.png");
	m_normalTexture.FromFile("assets/normal.png");
	m_heightTexture.FromFile("assets/height.png");
	m_generatedHeightTexture.FromFile("assets/generated_height.png");

	// kamera
	m_camera.SetProj(glm::radians(60.0f), 640.0f / 480.0f, 0.01f, 1000.0f);
	
	m_lightPos = glm::vec4(-5.0, 5.0, 0.0, 0.0);

	return true;
}

void CMyApp::Clean()
{
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();

	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;
	m_ElapsedTime += delta_time;

	last_time = SDL_GetTicks();

	m_camera.Update(delta_time);
}

void CMyApp::Render()
{
	glClearColor(0, 0, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderScene();

	if (ImGui::Begin("World Settings"))
	{
		ImGui::SliderFloat3("Light Position", &m_lightPos[0], -6.0, 6.0);
		ImGui::Separator();
		ImGui::SliderFloat("Bump Map Intensity", &bumpIntensity, 0, 0.8);
		ImGui::SliderFloat("Parallax Map Intensity", &parallaxIntensity, 0, 0.2);
		ImGui::Separator();
		if (isParallaxActive) {
			ImGui::Text(shaderSwitch ? "<- Bump | Parallax ->" : "<- Normal | Parallax ->");
		}
		else {
			ImGui::Text(shaderSwitch ? "<- Bump | Normal ->" : "<- Normal | Bump ->");
		}
		if (ImGui::Button("Switch Normal-Bump")) {
			shaderSwitch = !shaderSwitch;
		}
		if (ImGui::Checkbox("Activate Parallax", &isParallaxActive));
		if (ImGui::Checkbox("Display Generated Map", &displayGenerated));
	}

	ImGui::End();
}

void CMyApp::RenderScene() {
	ProgramObject* first = shaderSwitch ? m_programBump : m_programNormal;
	ProgramObject* second = shaderSwitch ? m_programNormal   : m_programBump;

	glViewport(0, 0, w / 2, h);
	RenderMapping(first);
	glViewport(w / 2, 0, w / 2, h);
	isParallaxActive ? RenderParallaxMap() : RenderMapping(second);
	glViewport(0, 0, w, h);
}

void CMyApp::RenderMapping(ProgramObject* m_program) {
	glm::mat4 viewProj = m_camera.GetViewProj();
	m_program->Use();
	m_program->SetUniform("light_pos", m_lightPos);

	// ground

	m_GroundVao.Bind();

	glm::mat4 groundWorld = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::vec3(80.0f)) * glm::rotate(-(float)M_PI / 2, glm::vec3(1, 0, 0));

	m_program->SetTexture("texImage", 0, m_groundTexture);
	m_program->SetTexture("normalTexImage", 1, m_normalTexture);
	m_program->SetTexture("heightTexImage", 2, displayGenerated ? m_generatedHeightTexture : m_heightTexture);
	m_program->SetUniform("MVP", viewProj * groundWorld);
	m_program->SetUniform("world", groundWorld);
	m_program->SetUniform("worldIT", glm::inverse(glm::transpose(groundWorld)));
	m_program->SetUniform("intensity", bumpIntensity);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	m_GroundVao.Unbind();

	// cube

	m_SideVao.Bind();
	m_program->SetTexture("texImage", 0, m_sideTexture);
	m_program->SetTexture("normalTexImage", 1, m_normalTexture);
	m_program->SetTexture("heightTexImage", 2, displayGenerated ? m_generatedHeightTexture : m_heightTexture);
	m_program->SetUniform("intensity", bumpIntensity);

	glm::vec3 startCoords;
	glm::mat4 sideWorld;

	for (int i = 0; i < 6; i++) {
		startCoords = glm::vec3(0.0);
		if (i == 0) startCoords = glm::vec3(3.0, 5.0, 0.0);
		if (i == 1) startCoords = glm::vec3(0.0, 5.0, -3.0);
		if (i == 2) startCoords = glm::vec3(-3.0, 5.0, 0.0);
		if (i == 3) startCoords = glm::vec3(0.0, 5.0, 3.0);
		if (i == 4) startCoords = glm::vec3(0.0, 8.0, 0.0);
		if (i == 5) startCoords = glm::vec3(0.0, 2.0, 0.0);

		if (i < 4)  sideWorld = glm::translate(startCoords) * glm::scale(glm::vec3(6.0f, 6.0f, 6.0f)) * glm::rotate((float)M_PI * (i + 1) / 2, glm::vec3(0, 1, 0));
		if (i == 4) sideWorld = glm::translate(startCoords) * glm::scale(glm::vec3(6.0f, 6.0f, 6.0f)) * glm::rotate((float)M_PI * 3 / 2, glm::vec3(1, 0, 0));
		if (i == 5) sideWorld = glm::translate(startCoords) * glm::scale(glm::vec3(6.0f, 6.0f, 6.0f)) * glm::rotate((float)M_PI * 1 / 2, glm::vec3(1, 0, 0));

		m_program->SetUniform("MVP", viewProj * sideWorld);
		m_program->SetUniform("world", sideWorld);
		m_program->SetUniform("worldIT", glm::inverse(glm::transpose(sideWorld)));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
	m_SideVao.Unbind();

	m_program->Unuse();
}

void CMyApp::RenderParallaxMap() {
	glm::mat4 viewProj = m_camera.GetViewProj();
	m_programParallax.Use();
	m_programParallax.SetUniform("lightPos", glm::vec3(m_lightPos));
	m_programParallax.SetUniform("viewPos", m_camera.GetEye());
	m_programParallax.SetUniform("heightScale",parallaxIntensity);

	// ground

	m_GroundVao.Bind();

	glm::mat4 groundWorld = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::vec3(80.0f)) * glm::rotate(-(float)M_PI / 2, glm::vec3(1, 0, 0));

	m_programParallax.SetTexture("texImage", 0, m_groundTexture);
	m_programParallax.SetTexture("heightTexImage", 1, displayGenerated ? m_generatedHeightTexture : m_heightTexture);
	m_programParallax.SetUniform("MVP", viewProj * groundWorld);
	m_programParallax.SetUniform("world", groundWorld);
	m_programParallax.SetUniform("worldIT", glm::inverse(glm::transpose(groundWorld)));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	m_GroundVao.Unbind();

	// cube

	m_SideVao.Bind();
	m_programParallax.SetTexture("texImage", 0, m_sideTexture);
	m_programParallax.SetTexture("heightTexImage", 1, displayGenerated ? m_generatedHeightTexture : m_heightTexture);

	glm::vec3 startCoords;
	glm::mat4 sideWorld;

	for (int i = 0; i < 6; i++) {
		startCoords = glm::vec3(0.0);
		if (i == 0) startCoords = glm::vec3(3.0, 5.0, 0.0);
		if (i == 1) startCoords = glm::vec3(0.0, 5.0, -3.0);
		if (i == 2) startCoords = glm::vec3(-3.0, 5.0, 0.0);
		if (i == 3) startCoords = glm::vec3(0.0, 5.0, 3.0);
		if (i == 4) startCoords = glm::vec3(0.0, 8.0, 0.0);
		if (i == 5) startCoords = glm::vec3(0.0, 2.0, 0.0);

		if (i < 4)  sideWorld = glm::translate(startCoords) * glm::scale(glm::vec3(6.0f, 6.0f, 6.0f)) * glm::rotate((float)M_PI * (i + 1) / 2, glm::vec3(0, 1, 0));
		if (i == 4) sideWorld = glm::translate(startCoords) * glm::scale(glm::vec3(6.0f, 6.0f, 6.0f)) * glm::rotate((float)M_PI * 3 / 2, glm::vec3(1, 0, 0));
		if (i == 5) sideWorld = glm::translate(startCoords) * glm::scale(glm::vec3(6.0f, 6.0f, 6.0f)) * glm::rotate((float)M_PI * 1 / 2, glm::vec3(1, 0, 0));

		m_programParallax.SetUniform("MVP", viewProj * sideWorld);
		m_programParallax.SetUniform("world", sideWorld);
		m_programParallax.SetUniform("worldIT", glm::inverse(glm::transpose(sideWorld)));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
	m_SideVao.Unbind();

	m_programParallax.Unuse();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_camera.Resize(_w / 2, _h);
	
	w = _w;
	h = _h;
}
