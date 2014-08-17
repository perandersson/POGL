﻿#include <gl/pogl.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <cmath>
#include "POGLExampleWindow.h"

static const POGL_CHAR SIMPLE_EFFECT_VS[] = { R"(
	#version 330

	layout(location = 0) in vec3 position;

	void main()
	{
		gl_Position = vec4(position, 1.0);
	}
)"};

static const POGL_CHAR SIMPLE_EFFECT_FS[] = { R"(
	#version 330

	layout(location = 0) out vec4 color;

	void main()
	{
		color = vec4(1, 1, 1, 1);
	}
)" };

static const POGL_UINT32 CIRCLE_PTS = 365;

int main()
{
	// Create a window
	POGL_HANDLE windowHandle = POGLCreateExampleWindow(POGL_SIZE(1024, 768), POGL_TOSTRING("Example: MapRange Vertex Data"));

	// Create a POGL device based on the supplied information
	POGL_DEVICE_INFO deviceInfo = { 0 };
#ifdef _DEBUG
	deviceInfo.flags = POGLDeviceInfoFlags::DEBUG_MODE;
#else
	deviceInfo.flags = 0;
#endif
	deviceInfo.windowHandle = windowHandle;
	deviceInfo.colorBits = 32;
	deviceInfo.depthBits = 16;
	deviceInfo.pixelFormat = POGLPixelFormat::R8G8B8A8;

	try {
		IPOGLDevice* device = POGLCreateDevice(&deviceInfo);
		IPOGLDeviceContext* context = device->GetDeviceContext();

		IPOGLShader* vertexShader = context->CreateShaderFromMemory(SIMPLE_EFFECT_VS, sizeof(SIMPLE_EFFECT_VS), POGLShaderType::VERTEX_SHADER);
		IPOGLShader* fragmentShader = context->CreateShaderFromMemory(SIMPLE_EFFECT_FS, sizeof(SIMPLE_EFFECT_FS), POGLShaderType::FRAGMENT_SHADER);
		IPOGLShader* shaders[] = { vertexShader, fragmentShader, nullptr };
		IPOGLProgram* program = context->CreateProgramFromShaders(shaders);
		vertexShader->Release();
		fragmentShader->Release();

		//
		// Create vertex buffer for circle geometry
		//

		std::vector<POGL_POSITION_VERTEX> vertices;
		vertices.push_back(POGL_POSITION_VERTEX(POGL_VECTOR3(0, 0, 0)));
		for (POGL_UINT32 i = 0; i < CIRCLE_PTS; ++i) {
			const POGL_FLOAT x = 0.1f * cosf(i * 0.0174532925f);
			const POGL_FLOAT y = 0.1f * sinf(i * 0.0174532925f);
			vertices.push_back(POGL_POSITION_VERTEX(POGL_VECTOR3(x, y, 0)));
		}
		IPOGLVertexBuffer* vertexBuffer = context->CreateVertexBuffer(&vertices[0], vertices.size() * sizeof(POGL_POSITION_VERTEX), POGLPrimitiveType::TRIANGLE_FAN, POGLBufferUsage::DYNAMIC);
		
		POGL_FLOAT totalTimeFlt = 0.0f;
		while (POGLProcessEvents()) {
			totalTimeFlt += POGLGetTimeSinceLastTick();

			//
			// Update the top part of the circle
			//

			{
				const POGL_UINT32 offset = 1;
				const POGL_UINT32 length = CIRCLE_PTS / 2;

				//
				// Map a memory location where the new vertex buffer data can be put into
				//

				POGL_POSITION_VERTEX* vertices = (POGL_POSITION_VERTEX*)context->Map(vertexBuffer, offset * sizeof(POGL_POSITION_VERTEX), 
					length * sizeof(POGL_POSITION_VERTEX), POGLResourceMapType::WRITE);

				//
				// Make sure to not update the midpoint
				//

				POGL_POSITION_VERTEX* ptr = vertices + 1;

				//
				// Update the buffer data
				//

				const POGL_FLOAT radius = sinf((totalTimeFlt * 15.0f) * 0.0174532925f) * 0.5f;
				for (POGL_UINT32 i = 0; i < length; ++i) {
					ptr->position.x = radius * cosf(i * 0.0174532925f);
					ptr->position.y = radius * sinf(i * 0.0174532925f);
					ptr->position.z = 0;
					ptr++;
				}

				//
				// Unmap the buffer and submit the data to the graphics card
				//

				context->Unmap(vertexBuffer);
			}

			//
			// Update the bottom part of the circle
			//

			{
				const POGL_UINT32 offset = 1 + CIRCLE_PTS / 2;
				const POGL_UINT32 length = (1 + CIRCLE_PTS) - offset;

				//
				// Map a memory location where the new vertex buffer data can be put into
				//

				POGL_POSITION_VERTEX* vertices = (POGL_POSITION_VERTEX*)context->Map(vertexBuffer, offset * sizeof(POGL_POSITION_VERTEX),
					length * sizeof(POGL_POSITION_VERTEX), POGLResourceMapType::WRITE);
				POGL_POSITION_VERTEX* ptr = vertices;

				//
				// Update the buffer data
				//

				const POGL_FLOAT radius = sinf((totalTimeFlt * 15.0f) * 0.0174532925f) * 0.5f;
				for (POGL_UINT32 i = 0; i < length; ++i) {
					ptr->position.x = radius * cosf((i + length) * 0.0174532925f);
					ptr->position.y = radius * sinf((i + length) * 0.0174532925f);
					ptr->position.z = 0;
					ptr++;
				}

				//
				// Close the stream and synchronize between contexts
				//

				context->Unmap(vertexBuffer);

			}

			//
			// Render the vertexBuffer to the screen
			//

			IPOGLRenderState* state = context->Apply(program);
			state->Clear(POGLClearType::COLOR | POGLClearType::DEPTH);
			state->Draw(vertexBuffer);
			state->Release();

			//
			// End the current frame
			//

			device->EndFrame();
		}
		
		vertexBuffer->Release();
		program->Release();
		context->Release();
		device->Release();
	}
	catch (POGLException e) {
		POGLAlert(e);
	}

	// Destroy the example window
	POGLDestroyExampleWindow(windowHandle);

	// Quit application
	return 0;
}
