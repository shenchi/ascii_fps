#include <Windows.h>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "ConsoleWindow.h"
#include "Rasterizer.h"
#include "Pipeline.h"
#include "Shader.h"
#include "AsciiGrayScale.h"
#include "MapMesh.h"
#include "ObjMeshDumpper.h"

typedef std::chrono::high_resolution_clock timer;

static ConsoleWindow window;
static short bufferWidth = 0, bufferHeight = 0;

class Adaptor : public IColorBufferAdaptor
{
public:
	Adaptor(ConsoleWindow* window) : window(window) {}

	virtual void WriteRenderTarget(int x, int y, const float* color)
	{
		unsigned short col;
		char ascii;
		AsciiGrayScale::ConvertRGBToAscii(color, ascii, col);
		window->SetColor(static_cast<short>(x), static_cast<short>(y), col, ascii);
	}

	virtual int GetBufferWidth() const
	{
		return window->GetBufferWidth();
	}

	virtual int GetBufferHeight() const
	{
		return window->GetBufferHeight();
	}

	virtual void ClearColorBuffer(const float* color)
	{
		unsigned short col = 0;
		if (color[0] > 0.5f) col |= 4;
		if (color[1] > 0.5f) col |= 2;
		if (color[2] > 0.5f) col |= 1;
		window->Clear(col);
	}

private:
	ConsoleWindow* window;
};

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	int ret = window.Create(L"Test Console API", 800 / 4, 600 / 8, 4, 8);		// low res
	//int ret = window.Create(L"Test Console API", 1200 / 3, 900 / 6, 3, 6);	// high res

	bufferWidth = window.GetBufferWidth();
	bufferHeight = window.GetBufferHeight();

	Adaptor* adaptor = new Adaptor(&window);
	Rasterizer raster(bufferWidth, bufferHeight);
	Pipeline pipeline(&raster, adaptor);

#pragma region data
	const char gridMap[] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};

	const char tileTypes[] = { 1, 0 };

	float red_color[] = { 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };

	MapMesh mesh;
	mesh.CreateFromGridMap(gridMap, 8, 8, tileTypes, 2, 1);
	mesh.GenerateMesh(1.0f, 2.0f, red_color);

	auto vert = mesh.GetVertices();
	auto idx = mesh.GetIndices();

	const float* vertices = reinterpret_cast<const float*>(&(vert[0]));
	size_t num_vertices = vert.size();

	const int* indices = &(idx[0]);
	size_t num_indices = idx.size();

	{
		float* p = const_cast<float*>(vertices);

		float colors[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f
		};

		for (size_t f = 0; f < 4; ++f)
		{
			float* pf = p + f * 4 * 9;
			float* color = &(colors[f * 3]);

			for (size_t v = 0; v < 4; ++v)
			{
				float* pv = pf + v * 9;

				pv[6] = color[0];
				pv[7] = color[1];
				pv[8] = color[2];
			}
		}
	}

	{
		Debugger::ObjMeshDumpper obj("original.obj");
		
		for (size_t i = 0; i < idx.size(); i += 3)
		{
			obj.AddTriangle(
				&(vert[idx[i]].x),
				&(vert[idx[i + 1]].x),
				&(vert[idx[i + 2]].x));
		}
	}

	glm::mat4 matRotate(1.0f);
	glm::mat4 matTranslate = glm::translate(glm::vec3(-4.0f, -1.0f, -4.0f));
	glm::mat4 matProj = glm::perspective(3.14159265f / 2, 0.5f * bufferWidth / bufferHeight, 1.0f, 20.0f);

	float t = 0.0f;
	float backgroundColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
#pragma endregion

	float interval = 0.016f;
	float elapsed = 0.0f;
	auto lastTime = timer::now();
	unsigned pos = 0;
	char title[256] = {};
	while (1)
	{
		auto now = timer::now();
		float deltaTime = std::chrono::duration<float>(now - lastTime).count();

		if (elapsed + deltaTime < interval)
		{
			continue;
		}

		lastTime = now;
		elapsed = deltaTime + elapsed - interval;

		//t += 0.016f;
		float angle = 3.1415f * 0.2f * t;

		sprintf_s(title, "%.4f (%7d, %7d) angle = %10d", deltaTime, window.GetMousePositionX(), window.GetMousePositionY(), int(angle / 3.1415926 * 180) % 360);
		window.SetTitleA(title);

		window.Update();

		pipeline.Clear(backgroundColor, 1.0f);
		pipeline.SetVertexShader(BuiltInShaders::GetVertexShader(1));

		matRotate = glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 matMVP = matProj * matRotate * matTranslate;
		pipeline.SetConstantBuffer(reinterpret_cast<float*>(&matMVP));

		pipeline.Draw(vertices, num_vertices, indices + 6, num_indices / 4);

		window.Flush();
		window.SwapBuffers();

		while (1)
		{
			if (window.IsKeyDown('A')) {
				t += 0.002f;
				break;
			}
			else if (window.IsKeyDown('D')) {
				t -= 0.002f;
				break;
			}
		}
	}

	window.Destroy();
	return 0;
}
