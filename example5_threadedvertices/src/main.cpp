﻿#include <gl/pogl.h>
#include <thread>
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
	POGL_HANDLE windowHandle = POGLCreateExampleWindow(POGL_SIZEI(1024, 768), POGL_TOSTRING("Example 5: Threaded Vertices"));

	// Create a POGL device based on the supplied information
	POGL_DEVICE_INFO deviceInfo = { 0 };
#ifdef _DEBUG
	deviceInfo.flags = POGLDeviceInfoFlags::DEBUG_MODE;
#else
	deviceInfo.flags = POGLDeviceInfoFlags::DEBUG_MODE;
#endif
	deviceInfo.windowHandle = windowHandle;
	deviceInfo.colorBits = 32;
	deviceInfo.depthBits = 16;
	deviceInfo.pixelFormat = POGLPixelFormat::R8G8B8A8;
	IPOGLDevice* device = POGLCreateDevice(&deviceInfo);

	try {
		// Create a device context for main thread
		IPOGLDeviceContext* context = device->GetDeviceContext();

		// Create an effect based on the supplied vertex- and fragment shader
		IPOGLShaderProgram* vertexShader = context->CreateShaderProgramFromMemory(SIMPLE_EFFECT_VS, sizeof(SIMPLE_EFFECT_VS), POGLShaderProgramType::VERTEX_SHADER);
		IPOGLShaderProgram* fragmentShader = context->CreateShaderProgramFromMemory(SIMPLE_EFFECT_FS, sizeof(SIMPLE_EFFECT_FS), POGLShaderProgramType::FRAGMENT_SHADER);
		IPOGLShaderProgram* programs[] = { vertexShader, fragmentShader };
		IPOGLEffect* simpleEffect = context->CreateEffectFromPrograms(programs, 2);
		vertexShader->Release();
		fragmentShader->Release();

		//
		// Create vertex buffer for circle geometry
		//

		std::vector<POGL_POSITION_VERTEX> vertices;
		vertices.push_back(POGL_POSITION_VERTEX(POGL_VECTOR3F(0, 0, 0)));
		for (POGL_UINT32 i = 0; i < CIRCLE_PTS; ++i) {
			const POGL_FLOAT x = 0.1f * cosf(i * 0.0174532925f);
			const POGL_FLOAT y = 0.1f * sinf(i * 0.0174532925f);
			vertices.push_back(POGL_POSITION_VERTEX(POGL_VECTOR3F(x, y, 0)));
		}
		IPOGLVertexBuffer* vertexBuffer = context->CreateVertexBuffer(&vertices[0], vertices.size() * sizeof(POGL_POSITION_VERTEX), POGLPrimitiveType::TRIANGLE_FAN, POGLBufferUsage::DYNAMIC);

		std::atomic<bool> running(true);
		std::atomic<POGL_FLOAT> totalTime(0.0f);
		
		//
		// Create two threads. 
		//
		// The first thread is responsible for updating the vertex buffer; increasing and decreasing the radius of the top-part of the circle over time.
		// The second thread is responsible for increasing and decreasing the radius of the bottom-part of the circle over time.
		//

		std::thread t1([&device, &vertexBuffer, &running, &totalTime] {
			try {
				IPOGLDeviceContext* context = device->GetDeviceContext();
				while (running.load()) {
					const POGL_FLOAT totalTimeFlt = totalTime.load();

					const POGL_UINT32 offset = 1;
					const POGL_UINT32 length = CIRCLE_PTS / 2;
					
					//
					// Open a stream to the vertex buffer
					//

					IPOGLResourceStream* stream = context->OpenStream(vertexBuffer, POGLResourceStreamType::WRITE);

					//
					// Map the internal memory
					//

					POGL_POSITION_VERTEX* vertices = (POGL_POSITION_VERTEX*)stream->Map(offset * sizeof(POGL_POSITION_VERTEX), length * sizeof(POGL_POSITION_VERTEX));
					POGL_POSITION_VERTEX* ptr = vertices;

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
					// Close the stream and synchronize between contexts
					//

					stream->Close();

				}
				context->Release();
			}
			catch (POGLException e) {
				POGLAlert(e);
			} 
		});

		std::thread t2([&device, &vertexBuffer, &running, &totalTime] {
			try {
				IPOGLDeviceContext* context = device->GetDeviceContext();
				while (running.load()) {
					const POGL_FLOAT totalTimeFlt = totalTime.load();

					const POGL_UINT32 offset = 1 + CIRCLE_PTS / 2;
					const POGL_UINT32 length = (1 + CIRCLE_PTS) - offset;

					//
					// Open a stream to the vertex buffer
					//

					IPOGLResourceStream* stream = context->OpenStream(vertexBuffer, POGLResourceStreamType::WRITE);

					//
					// Map the internal memory
					//

					POGL_POSITION_VERTEX* vertices = (POGL_POSITION_VERTEX*)stream->Map(offset * sizeof(POGL_POSITION_VERTEX), length * sizeof(POGL_POSITION_VERTEX));
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

					stream->Close();

				}
				context->Release();
			}
			catch (POGLException e) {
				POGLAlert(e);
			}
		});

		POGL_FLOAT totalTimeFlt = 0.0f;
		while (POGLProcessEvents()) {
			totalTimeFlt += POGLGetTimeSinceLastTick();
			totalTime.store(totalTimeFlt);

			IPOGLRenderState* state = context->Apply(simpleEffect);
			state->Clear(POGLClearType::COLOR | POGLClearType::DEPTH);
			state->Draw(vertexBuffer);
			state->EndFrame();

			device->SwapBuffers();
		}

		running.store(false);
		t1.join();
		t2.join();

		// Release resources
		vertexBuffer->Release();
		simpleEffect->Release();
		context->Release();
	}
	catch (POGLException e) {
		POGLAlert(e);
	}

	// Release the device
	device->Release();

	// Destroy the example window
	POGLDestroyExampleWindow(windowHandle);

	// Quit application
	return 0;
}
