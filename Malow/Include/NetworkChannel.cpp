#include "NetworkChannel.h"
#include "NetworkException.h"
#include "ClientDisconnectedEvent.h"
#include "Winsock.h"

using MaloW::NetworkChannel;


NetworkChannel::NetworkChannel( SOCKET socket ) : 
	zSocket(socket),
	zPacketNumberIn(0),
	zPacketNumberOut(0),
	zNumBytesIn(0),
	zNumBytesOut(0)
{
	QueryPerformanceCounter((LARGE_INTEGER*)&zChannelCreated);
}

bool NetworkChannel::Receive( std::string& msg, double& timeTaken )
{
	zReadMutex.lock();

	try
	{
		// Error Code
		int errCode;

		// Receive Packet Time
		__int64 packetSentTime;
		if ( (errCode = recv(zSocket, reinterpret_cast<char*>(&packetSentTime), sizeof(__int64), 0)) <= 0 )
		{
			if ( errCode < 0 )
			{
				throw NetworkException("Failed Receiving Packet Number!", WSAGetLastError());
			}
			else
			{
				throw NetworkException("Connection Closed!");
			}
		}

		// Receive Packet Number
		unsigned int packetNumber = 0;
		if ( (errCode = recv(zSocket, reinterpret_cast<char*>(&packetNumber), sizeof(unsigned int), 0)) <= 0 )
		{
			if ( errCode < 0 )
			{
				throw NetworkException("Failed Receiving Packet Number!", WSAGetLastError());
			}
			else
			{
				throw NetworkException("Connection Closed!");
			}
		}

		// Check Packet Number
		if ( packetNumber != zPacketNumberIn++ )
		{
			throw NetworkException("Packet Not Received In Correct Order!");
		}

		// Receive Packet Size
		unsigned int packetSize = 0;
		if ( (errCode = recv(zSocket, reinterpret_cast<char*>(&packetSize), sizeof(unsigned int), 0)) <= 0 )
		{
			if ( errCode < 0 )
			{
				throw NetworkException("Failed Receiving Packet Size!", WSAGetLastError());
			}
			else
			{
				throw NetworkException("Connection Closed!");
			}
		}
		msg.clear();

		// Receive Packet
		msg.resize(packetSize);
		if ( (errCode = recv(zSocket, &msg[0], packetSize, MSG_WAITALL)) <= 0 )
		{
			if ( errCode < 0 )
			{
				throw NetworkException("Failed Receiving Packet Data!", WSAGetLastError());
			}
			else
			{
				throw NetworkException("Connection Closed!");
			}
		}

		if (packetSize != errCode)
		{
			throw NetworkException("Message is not expected size!");
		}

		// Count Bytes Received
		zNumBytesIn += packetSize + sizeof(unsigned int) * 2;

		// Count Time Taken
		__int64 curTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
		__int64 timeItTook = curTime - zChannelCreated - packetSentTime;

		// Count Frequency
		__int64 freqency;
		QueryPerformanceCounter((LARGE_INTEGER*)&freqency);
		
		// Convert To Double
		timeTaken = ( 1.0 / (double)freqency ) * (double)timeItTook;
	}
	catch(...)
	{
		zReadMutex.unlock();
		throw;
	}

	zReadMutex.unlock();
	return true;
}

bool NetworkChannel::Send(const std::string& msg)
{
	zWriteMutex.lock();

	try
	{
		// ErrorCode
		int errCode;

		// Send Packet Time
		__int64 curTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
		__int64 relTime = curTime - zChannelCreated;
		if ( (errCode = send(zSocket, reinterpret_cast<char*>(&relTime), sizeof(__int64), 0)) <= 0 )
		{
			if ( errCode < 0 )
			{
				throw NetworkException("Failed Sending Packet Time!", WSAGetLastError());
			}
			else
			{
				throw NetworkException("Connection Closed!");
			}
		}

		// Send Packet Number
		unsigned int packetNumber = zPacketNumberOut++;
		if ( (errCode = send(zSocket, reinterpret_cast<char*>(&packetNumber), sizeof(unsigned int), 0)) <= 0 )
		{
			if ( errCode < 0 )
			{
				throw NetworkException("Failed Sending Packet Number!", WSAGetLastError());
			}
			else
			{
				throw NetworkException("Connection Closed!");
			}
		}

		// Send Packet Size
		unsigned int size = msg.length();
		if( (errCode = send(zSocket, reinterpret_cast<char*>(&size), sizeof(unsigned int), 0)) <= 0 )
		{
			if ( errCode < 0 )
			{
				throw NetworkException("Failed Sending Packet Size!", WSAGetLastError());
			}
			else
			{
				throw NetworkException("Connection Closed!");
			}
		}

		// Send Packet Data
		if( (errCode = send(zSocket, &msg[0], msg.length(), 0)) <= 0 )
		{
			if ( errCode < 0 )
			{
				throw NetworkException("Failed Sending Packet Data!", WSAGetLastError());
			}
			else
			{
				throw NetworkException("Connection Closed!");
			}
		}

		// Count Bytes Sent
		zNumBytesOut += size + sizeof(unsigned int) * 2;
	}
	catch(...)
	{
		zWriteMutex.unlock();
		throw;
	}

	zWriteMutex.unlock();
	return true;
}

bool NetworkChannel::TrySend(const std::string& msg)
{
	try
	{
		return Send(msg);
	}
	catch(...)
	{
		return false;
	}
}

void NetworkChannel::Disconnect()
{
	shutdown(zSocket, SD_BOTH);
}