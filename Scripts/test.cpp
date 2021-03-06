#define HGE_SCRIPT
#include "hpch.h"
#include "HGE.h"

using namespace HGE;


	//test script
	class TestScript : public Script {
	public:

		GuiFrame frame;

		void start() {
			frame = GuiFrame();
			frame.sizeType = HGE_PIXEL_SIZE;
			frame.size = Vec2f(400, 400);
			frame.position = Vec2f();
			frame.name = "test";
			frame.anchorPoint = Vec2f();
			frame.backgroundColor = Vec4f(0.9f, 0.6f, 0, 0.5f);
			frame.borderColor = Vec4f(0, 0, 0, 0.6f);

			frame.draggable = true;
			frame.selectable = true;
			frame.active = true;

			Engine::registerGuiFrame(&frame);

		}

		void update() {
			frame.rotation += Input::getScrollMovement().y;

			if (Input::getKey(HGE_KEY_B)) {
				Debug::log("test");
			}
		}

		void fixedUpdate() {

		}

		void stop() {
		}

		~TestScript() {
		}

	};	

extern "C" {
	__declspec(dllexport) Script* CreateScript()
	{
		return new TestScript();
	}
}