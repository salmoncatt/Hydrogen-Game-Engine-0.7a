
#ifndef HFR_NATIVE_SCRIPT_HEADER_INCLUDE
#define HFR_NATIVE_SCRIPT_HEADER_INCLUDE

#include HFR_API
#include HFR_ECS
#include HFR_GAMEOBJECT
#include HFR_SCRIPT_MANAGER

#define HFR_MAX_SCRIPTS 100

using Script = HFR::GameObject;

namespace HFR {

	struct DLL_API NativeScript {
		std::unordered_map<unsigned int, std::string> nameToScriptIndex{};
		unsigned int scriptAmount = 0;

		std::array<GameObject*, HFR_MAX_SCRIPTS> scripts{nullptr};

		void (*instantiateScript)(NativeScript*, const unsigned int&) = {};

		//name is the name of the dll file in which the script is in
		void addScript(const std::string& name) {
			if (ScriptManager::checkScript(name)) {
				nameToScriptIndex[scriptAmount] = name;
				scriptAmount += 1;
			}
			else
				Debug::systemErr("Couldn't add script with name: " + name + ", because it wasn't registered");
			
			instantiateScript = [](NativeScript* nativeScript, const unsigned int& index) {
				GameObject* script = ScriptManager::instantiateScript(nativeScript->nameToScriptIndex[index]);
				if (script != nullptr)
					nativeScript->scripts[index] = script;
				else
					Debug::systemErr("Couldn't find script: " + nativeScript->nameToScriptIndex[index]);
			};
		}

		void destroyScripts() {

		}

	};

}


#endif