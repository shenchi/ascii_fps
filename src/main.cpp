#include "ConsoleWindow.h"
#include "Rasterizer.h"
#include "Pipeline.h"
#include <Windows.h>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

typedef std::chrono::high_resolution_clock timer;

static ConsoleWindow window;
static short bufferWidth = 0, bufferHeight = 0;

class Adaptor : public IColorBufferAdaptor
{
public:
	Adaptor(ConsoleWindow* window) : window(window) {}

	virtual void WriteRenderTarget(int x, int y, float* color)
	{
		unsigned int col = 8;
		if (color[0] > 0.5f) col |= 4;
		if (color[1] > 0.5f) col |= 2;
		if (color[2] > 0.5f) col |= 1;
		window->SetColor(static_cast<short>(x), static_cast<short>(y), col);
	}

	virtual int GetBufferWidth() const
	{
		return window->GetBufferWidth();
	}

	virtual int GetBufferHeight() const
	{
		return window->GetBufferHeight();
	}

private:
	ConsoleWindow* window;
};

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	window.Create(L"Test Console API", 800 / 4, 600 / 8, 4, 8);	// low res
	//window.Create(L"Test Console API", 1200 / 3, 900 / 6, 3, 6);	// high res

	bufferWidth = window.GetBufferWidth();
	bufferHeight = window.GetBufferHeight();

	Adaptor adaptor(&window);
	Rasterizer raster(bufferWidth, bufferHeight);
	Pipeline pipeline(&raster, &adaptor);

#pragma region data
	float triangle_vertices[] =
	{
		-0.5f, -0.5f, -0.5f, 1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f,
		 -0.5f, -0.5f, 0.5f, 1.0f,
		 -0.5f,  0.5f, 0.5f, 1.0f,
		 0.5f, -0.5f, 0.5f, 1.0f,
		 0.5f,  0.5f, 0.5f, 1.0f,
	};
	size_t num_vertices = sizeof(triangle_vertices) / sizeof(float) / 4;

	size_t triangle_indices[] =
	{
		0, 1, 2,
		2, 1, 3,

		2, 3, 6,
		6, 3, 7,

		6, 7, 4,
		4, 7, 5,

		4, 5, 0,
		0, 5, 1,

		1, 5, 3,
		3, 5, 7,

		4, 0, 6,
		6, 0, 2
	};
	size_t num_indices = sizeof(triangle_indices) / sizeof(size_t);

	glm::mat4 matRotate(1.0f);
	glm::mat4 matTranslate = glm::translate(glm::vec3(0.0f, 0.0f, 5.0f));
	glm::mat4 matProj = glm::perspective(45.0f, 0.5f * bufferWidth / bufferHeight, 0.01f, 10.0f);

	float t = 0.0f;
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

		sprintf_s(title, "%.4f", deltaTime);
		window.SetTitleA(title);

		window.Update();

		window.Clear(0);

		t += deltaTime;
		matRotate = glm::rotate(3.1415f * 0.5f * t, glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 matMVP = matProj * matTranslate * matRotate;
		pipeline.SetConstantBuffer(reinterpret_cast<float*>(&matMVP));

		pipeline.Draw(triangle_vertices, num_vertices, triangle_indices, num_indices);

		window.Flush();
		window.SwapBuffers();
	}

	window.Destroy();
	return 0;
}
