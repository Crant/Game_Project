//Made by Ed�nge Simon 
//for project Not Dead Yet at Blekinge tekniska h�gskola.

#pragma once

#include "Client.h"


enum NETWORK_ERROR_CODE
{
	CONNECTION_SUCCESS, 
	CONNECTION_FAILED,
	CONNECTION_WINSOCK_FAIL,
	CONNECTION_INVALID_SOCKET
};

class Game
{
	public:
		Game();
		/*! Creates a Client that connects to the IP and Port in the Parameters.
			The function will start the thread if succeeded.*/
		bool InitGameClient(const std::string &IP, const unsigned int &port, const std::string &playerModel);
		/*! Initialize the sounds.
			It will read and load all sounds.
		*/

		virtual ~Game();

		void Run();

	private:
		Client* zClient;
};