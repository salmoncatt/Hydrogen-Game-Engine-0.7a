
#ifndef HGE_GAME_OBJECT_HEADER_INCLUDE
#define HGE_GAME_OBJECT_HEADER_INCLUDE

#include HGE_API
#include "ECS.h"
#include "Scene.h"

namespace HGE {

	struct DLL_API GameObject {
		Entity entity;
		Scene* scene = nullptr;

		GameObject(Entity entity, Scene* scene);
		GameObject();
		~GameObject();

		template <typename T>
		void addComponent() {
			T component = T();
			scene->getSystem()->getComponentManager()->addComponent<T>(entity, component);
		};

		template <typename T>
		void addComponent(T& component) {
			scene->getSystem()->getComponentManager()->addComponent<T>(entity, component);
		};
			
		template <typename T>
		T& getComponent() {
			return scene->getSystem()->getComponentManager()->getComponent<T>(entity);
		}

		template <typename T>
		void removeComponent() {
			scene->getSystem()->getComponentManager()->removeComponent<T>(entity);
		}

	};

}

#endif