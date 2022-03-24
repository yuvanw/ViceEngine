#pragma once

class IModule
{
public:
	IModule() = default;
	virtual ~IModule() = default;
	
	virtual void OnStartup() {}
	virtual void OnShutdown() {}
};
