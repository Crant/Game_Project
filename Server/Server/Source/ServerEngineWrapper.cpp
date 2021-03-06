//File: ServerEngineWrapper.cpp

#include "ServerEngineWrapper.h"
#include "ServerEngine.h"

ServerEngineWrapper::ServerEngineWrapper()
{
	zServerEngine = new ServerEngine();
}

ServerEngineWrapper::~ServerEngineWrapper()
{
	SAFE_DELETE(zServerEngine);
}

const char* ServerEngineWrapper::Init(const unsigned int &port, const unsigned int &maxPlayers, char* gameMode, char* mapName) const
{
	return zServerEngine->InitHost(port, maxPlayers, gameMode, mapName);
}

void ServerEngineWrapper::ShutdownHost() const
{
	this->zServerEngine->ShutDownHost();
}

bool ServerEngineWrapper::StartHost() const
{
	return this->zServerEngine->StartHost();
}

unsigned int ServerEngineWrapper::GetNumClients() const
{
	return zServerEngine->GetNumClients();
}

bool ServerEngineWrapper::IsRunning() const
{
	return zServerEngine->IsRunning();
}

void ServerEngineWrapper::SendMessageToClient(char* message)
{
	zServerEngine->SendMessageToClient(message);
}