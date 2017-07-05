#include "ResourceManager.h"

ResourcesManager* ResourcesManager::instance = nullptr;

ResourcesManager::ResourcesManager() 
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	
	window.create(sf::VideoMode(1366, 768, 32), "Astrum", sf::Style::Titlebar | sf::Style::Close, settings);

	window.setVerticalSyncEnabled(true);
}

ResourcesManager::~ResourcesManager()
{
}

ResourcesManager* ResourcesManager::getInstance()
{
	if (!instance) instance = new ResourcesManager();

	return instance;
}

ResourcesManager& ResourcesManager::getInstanceRef()
{
	return *getInstance();
}

void ResourcesManager::deleteInstance()
{
	delete instance;
}
