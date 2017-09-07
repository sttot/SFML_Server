////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
	
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <cstdlib>
#include <string>

#include "SFML\System\Time.hpp"
#include "LobbySystem.h"
#include "Player.h"

using namespace std;

// Create a socket for communicating with the server
sf::TcpSocket socket;

Player* players[3];						// List of all players on the server with a valid ID
LobbySystem* lobbies[3];				// List of all current lobbies on the server

unsigned short int activePlayerList[3];	// List all current players on server by ID
string activePlayerNames[3];			// Lists all current players on server by name

// Server player globals - For this game, these will always be these numbers
// Change according to game requirements
unsigned short int playerLimit = 2;
unsigned short int req = 2;

// For traversal through lobbies array 
unsigned short int lobbyLength = sizeof(lobbies) / sizeof(lobbies[0]);

// Used to define new IDs for new users
unsigned short int latestID = 100;

sf::Packet dataPacket;

////////////////////////////////////////////////////////////
///
/// Lists all currently online players on the server
///
////////////////////////////////////////////////////////////

void playerList()
{
	string playerListGraphics, playerGraphic1, playerGraphic2;
	dataPacket.clear();
	socket.receive(dataPacket);

	if (dataPacket >> playerListGraphics >> playerGraphic1 >> playerGraphic2)
	{
		cout << playerListGraphics << playerGraphic1 << playerGraphic2;
	}

	for (int x(0); x < 10; x++)
	{
		if (activePlayerNames[x].empty())
			cout << "";
		else
			cout << "Name: " << activePlayerNames[x] << " ID: " << activePlayerList[x] << "\n";
	}
	cout << "\n";
}

////////////////////////////////////////////////////////////
///
/// Where all lobbies will be set up.
/// Acts as top of lobby hierarchy.
///
////////////////////////////////////////////////////////////

void lobbyList()
{
	// Iterate through legnth of lobby list for all sizes
	for (int x(0); x < lobbyLength; x++)
	{
		if (lobbies[x]->lobbyName.empty())
			cout << "";
		else
		{
			cout << "Lobby Name: " << lobbies[x]->lobbyName << "'s Lobby \n";
			cout << "Players: " << lobbies[x]->currentPlayers << "/" << lobbies[x]->maxPlayers << "\n";
			cout << "Spectators: " << lobbies[x]->currentSpectators << "/" << lobbies[x]->maxSpectators << "\n";
			cout << "\n";
		}
	}
}

void gameState(unsigned short int x)
{
	bool gameRunning = false;
	cout << "Lobby Name: " << lobbies[x]->lobbyName << "\n";
	cout << "Players: " << lobbies[x]->currentPlayers << "/" << lobbies[x]->maxPlayers << "\n";
	cout << "Spectators: " << lobbies[x]->currentSpectators << "/" << lobbies[x]->maxSpectators << "\n";
	cout << "Game Status: ";
	if (lobbies[x]->gameState == false)
		cout << "Offline \n";
	else
		cout << "Online \n";
	cout << "Players : ";
}

void lobbyCreation()
{
	string creation1, creation2, name;
	unsigned short int spectatorLimit;

	dataPacket.clear();
	socket.receive(dataPacket);

	if (dataPacket >> creation1)
	{
		cout << creation1;
	}

	cin >> name;
	dataPacket.clear();
	dataPacket << name;
	socket.send(dataPacket);
	dataPacket.clear();
	socket.receive(dataPacket);

	cin >> spectatorLimit;


	for (int x(0); x < lobbyLength; x++)
	{
		if (lobbies[x] == NULL)
		{
			lobbies[x] = new LobbySystem(name, playerLimit, 0, req, 0, spectatorLimit, false);
			gameState(x);
		}
	}
}

void logInCreate()
{
	// Enters but goes wrong
	socket.receive(dataPacket);
	
	string logIn1;

	dataPacket >> logIn1;
	cout << logIn1;
	system("pause");
	string username, pass;
	cin >> username;
	dataPacket.clear();
	dataPacket << username;
	socket.send(dataPacket);
	for (int x(0); x < (sizeof(players) / sizeof(players[0])); x++)
	{
		// If the element in the player list has a name identical to input
		if (username == players[x]->name)
		{
			cout << "This name has already been taken. please enter a different name \n";
			logInCreate();
		}
		// If the element has a name but is not the same as input
		if (players[x]->name.length() == 0)
		{
			cout << "Element " << x << "\n";
			return;
		}
		// If element has no name to it, enter input here
		if (players[x]->name.empty())
		{
			cout << "Please input a password \n";
			cin >> pass;
			players[x] = new Player(username, latestID, pass);
		}

	}
}

void logIn()
{
	unsigned short int logInInput;
	string logInMenu1;
	//dataPacket.clear();
	socket.receive(dataPacket);
	
	dataPacket >> logInMenu1;

	dataPacket.clear();

	cin >> logInInput;

	//dataPacket >> logInInput;
	//socket.send(dataPacket);

	//cout << logInInput;
	//if (dataPacket >> logInInput)
	//{
	//	cout << "Input: " << logInInput;
	//}

	//dataPacket.clear();
	//cin >> logInInput;
	//dataPacket << logInInput;
	//socket.send(dataPacket);

	if (logInInput == 1)
	{
		cout << "working?\n";
		logInCreate();
	}
}

void profile()
{

}

void gameServer()
{
	unsigned short int userID = 100;
	unsigned short int input;

	socket.receive(dataPacket);

	string gameServerLine1, gameServerLine2, gameServerLine3, gameServerLine4;
	sf::Uint16 newID;

	if (dataPacket >> gameServerLine1 >> newID >> gameServerLine2 >> gameServerLine3 >> gameServerLine4)
	{
		cout << gameServerLine1 << newID << gameServerLine2 << gameServerLine3 << gameServerLine4;
	}

	cin >> input;
	cout << "\n";

	dataPacket.clear();
	dataPacket << input;
	socket.send(dataPacket);

	dataPacket.clear();
	socket.receive(dataPacket);

	if (dataPacket >> gameServerLine1)
	{
		cout << gameServerLine1;
	}
	
	cin >> input;
	cout << "\n";

	dataPacket.clear();
	dataPacket << input;
	socket.send(dataPacket);

	cin >> input;
	cout << "\n";

	// Working
	if (input == 1)
	{
		logIn();
		gameServer();
	}
	if (input == 2)
	{
		playerList();
		gameServer();
	}
	if (input == 3)
	{
		lobbyList();
		gameServer();
	}
	if (input == 4)
	{
		lobbyCreation();
		gameServer();
	}
	if (input == 5)
	{
		profile();
		gameServer();
	}
	if (input == 6)
	{

	}
}

////////////////////////////////////////////////////////////
///
/// Create a client, connect it to a server, display the
/// welcome message and send an answer.
///
////////////////////////////////////////////////////////////

void runTcpClient(unsigned short port)
{
	// Ask for the server address
	sf::IpAddress server;

	// Creates a timeout situation if it is not possible to connect to a server
	// Otherwise, program is stuck forever
	sf::Time timeout = sf::seconds(5.0f);

	do
	{
		// Local address for computer is 192.168.0.5
		std::cout << "Type the address or name of the server to connect to: ";
		std::cin >> server;
	} while (server == sf::IpAddress::None);

	// Connection Error Checking
	sf::Socket::Status status = socket.connect(server, port, timeout);
	if (status != sf::Socket::Done)
	{
		cout << "Error Connecting to Server at address " << server << endl;
		system("pause");
	}

	// Connect to the server
	if (socket.connect(server, port) != sf::Socket::Done)
		return;
	std::cout << "Connected to server " << server << std::endl;

	socket.receive(dataPacket);
	string testList;
	if (dataPacket >> testList)
	{
		cout << testList << "\n";
	}

	dataPacket.clear();
	gameServer();

	// Receive a message from the server
	char in[128];
	std::size_t received;
	if (socket.receive(in, sizeof(in), received) != sf::Socket::Done)
		return;
	std::cout << "Message received from the server: \"" << in << "\"" << std::endl;

	// Send an answer to the server
	const char out[] = "Hi, I'm a client";
	if (socket.send(out, sizeof(out)) != sf::Socket::Done)
		return;
	std::cout << "Message sent to the server: \"" << out << "\"" << std::endl;
}

int main()
{
	const unsigned short port = 50000;
	
	players[0] = new Player("Aaron", 100, "test");
	players[1] = new Player("Chris", 101, "hello");

	activePlayerList[0] = latestID;
	latestID++;
	activePlayerList[1] = latestID;
	latestID++;
	activePlayerNames[0] = "Aaron";
	activePlayerNames[1] = "Chris";

	lobbies[0] = new LobbySystem("LobbyTest", playerLimit, 0, req, 0, 4, false);
	lobbies[1] = new LobbySystem("Placeholder", playerLimit, 0, req, 0, 2, false);

	runTcpClient(port);

	// Makes the server client not close program when a function is complete
	cout << "Press enter to exit..." << endl;
	cin.ignore(10000, '\n');
	cin.ignore(10000, '\n');

	return EXIT_SUCCESS;
};