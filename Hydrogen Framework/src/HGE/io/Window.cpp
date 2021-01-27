#include "hpch.h"
#include "Window.h"
#include "HGE/util/Debug.h"
#include "HGE/util/time/Time.h"
#include "HGE/graphics/rendering/Renderer.h"
#include "HGE/util/Util.h"
#include "HGE/math/vectors/Vec4.h"

namespace HGE {

	Window::Window(const Vec2i& _size, const std::string& _Name) {
		size = _size;
		position = Vec2i();
		name = _Name;
	}

	Window::~Window() {
		Debug::systemLog("Attempting to delete window: " + name);
		Debug::systemSuccess("Deleted window: " + name);
	}

	void Window::create() {

		Debug::systemLog("Attempting to create window: " + name + ", With dimensions: " + std::to_string(size.x) + ", " + std::to_string(size.y));

		window = glfwCreateWindow(size.x, size.y, name.c_str(), NULL, NULL);
		
		if (!window) {
			Debug::systemErr("Failed to create window: " + name);
			exit(1);
		}
		else {
			Debug::systemSuccess("Created window: " + name);
			Debug::resetTextColor();
		}

		glfwMakeContextCurrent(window);

		glfwSwapInterval(0);
		glEnable(GL_DEPTH_TEST);

		glfwShowWindow(window);
	}

	void Window::update() {
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		glfwGetWindowSize(window, &size.x, &size.y);
	}

	void Window::setBackGroundColor(const Vec4f& color) {
		glClearColor(color.x, color.y, color.z, color.w);
	}

	bool Window::shouldClose() {
		update();
		return glfwWindowShouldClose(window);
	}

	void Window::setVSync(const bool& sync) {
		glfwSwapInterval(sync ? 1 : 0);
	}
	
	void Window::resize(const Vec2i& _size) {
		glfwSetWindowSize(window, _size.x, _size.y);
		glViewport(0, 0, _size.x, _size.y);
		size = _size;
	}

	void Window::setPosition(const Vec2i& _position) {
		glfwSetWindowPos(window, _position.x, _position.y);
	}

	void Window::limitFps(const int& fps) {
		float waitTime = (float)1000 / fps;
		float sleepTime = waitTime - Time::getDeltaTime();

		if (sleepTime > 0) {
			Sleep((DWORD)sleepTime);
		}
	}

	void Window::setIcon(Image& image) {
		GLFWimage images[1];

		image.generatePixelData();

		std::vector<unsigned char> data = Util::convertPixelsToRawData(image.pixeldata);

		unsigned char* pixelbuffer = new unsigned char[data.size()];
		std::copy(data.begin(), data.end(), pixelbuffer);

		images[0].width = image.width;
		images[0].height = image.width;
		images[0].pixels = pixelbuffer;

		glfwSetWindowIcon(window, 1, images);

		delete[] pixelbuffer;
	}

	bool Window::isFocused() {
		return focused;
	}

}