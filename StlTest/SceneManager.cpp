#include "SceneManager.h"

SceneManager::SceneManager()
{
}

void SceneManager::setCurrentScene(std::string sceneName)
{
	if (registeredScenes.find(sceneName) != registeredScenes.end()) {
		currentScene = registeredScenes[sceneName];
	}
}

GraphicsScene* SceneManager::getCurrentScene()
{
	return currentScene;
}

void SceneManager::registerScene(const std::string& sceneName, GraphicsScene& scene)
{
	if (registeredScenes.find(sceneName) != registeredScenes.end()) {
		registeredScenes.erase(sceneName);
	}
	registeredScenes.insert(std::make_pair(sceneName, &scene));
}

void SceneManager::unregisterScene(const std::string& sceneName)
{
	if (registeredScenes.find(sceneName) != registeredScenes.end()) {
		registeredScenes.erase(sceneName);
	}
}

SceneManager::~SceneManager()
{
}
