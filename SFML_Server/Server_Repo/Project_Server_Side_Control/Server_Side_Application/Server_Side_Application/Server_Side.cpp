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

Player* players[10];						// List of all players on the server with a valid ID
LobbySystem* lobbies[3];					// List of all current lobbies on the server

unsigned short int activePlayerList[10];	// List all current players on server by ID
string activePlayerNames[10];				// Lists all current players on server by name

// Server player globals - For this game, these will always be these numbers
// Change according to game requirements
unsigned short int playerLimit = 2;
unsigned short int req = 2;

// For traversal through lobbies array 
unsigned short int lobbyLength = sizeof(lobbies) / sizeof(lobbies[0]);

// Used to define new IDs for new users
unsigned short int latestID = 100;

// Globally defined socket
sf::TcpSocket socket;

// Used to monitor all sockets of all types
sf::SocketSelector selector;

// Data Packet Test
sf::Packet dataPacket;

////////////////////////////////////////////////////////////
///
/// Lists all currently online players on the server
///
////////////////////////////////////////////////////////////

void playerList()
{
	string playerListGraphic = "Current active players: \n";

	string playerGraphic1 = "Current active players: \n \n------------------------------ \n|                            |\n";
	string playerGraphic2 = "|           Players          |\n|                            |\n------------------------------ \n";
	dataPacket.clear();
	dataPacket << playerListGraphic << playerGraphic1 << playerGraphic2;
	socket.send(dataPacket);

	cout << "Current active players: \n";
	cout << "\n";
	cout << "------------------------------ \n";
	cout << "|                            | \n";
	cout << "|           Players          | \n";
	cout << "|                            | \n";
	cout << "------------------------------ \n";
	for (int x(0); x < 10; x++)
	{
		if (activePlayerNames[x].empty())
			cout << "";
		else
			cout << "Name: " << activePlayerNames[x] << " ID: " << activePlayerList[x] << "\n";
	}
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

////////////////////////////////////////////////////////////
///
/// Call upon the game state.
/// Server will pass game info as the game sends it to the server
/// Will only send packets when the calls are necessary
/// As game component was only implemented less than 24 hours before submission
/// Omissions for the game will be placed in.
/// Where needed, comments will state where the game would be placed in optimal conditions.
///
////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////
///
/// Users can create new servers here
/// Currently using fixed size array to determine if there is space
///
////////////////////////////////////////////////////////////

void lobbyCreation()
{
	string creation1 = "Choose server name: ";
	string creation2 = "\nChoose spectator limit: ";
	string name;
	unsigned short int spectatorLimit;
	cin >> name;
	cin >> spectatorLimit;

	if (dataPacket >> name)
	{
		cin >> name;
	}

	for (int x(0); x < lobbyLength; x++)
	{
		if (lobbies[x] == NULL)
		{
			lobbies[x] = new LobbySystem(name, playerLimit, 0, req, 0, spectatorLimit, false);
			gameState(x);
		}
		else
			return;
	}
	
}

void logInCreate()
{
	cout << "Creation \n";
	string logIn1 = "Please input a username: ";
	dataPacket.clear();
	dataPacket << logIn1;
	socket.send(dataPacket);

	dataPacket.clear();
	socket.receive(dataPacket);

	string username;
	if (dataPacket >> username)
	{
		cin >> username;
		cout << "complete";
	}

	cout << username;
	for (int x(0); x < (sizeof(players) / sizeof(players[0])); x++)
	{
		// If the element in the player list has a name identical to input
		if (username == players[x]->name)
		{
			cout << "This name has already been taken. please enter a different name \n";
			logInCreate();
		}
		// If the element has a name but is not the same as input
		if (players[x]->name.length() > 0)
		{
			return;
		}
		// If element has no name to it, enter input here
		if (players[x]->name.empty())
		{

		}

	}
}

void logIn()
{
	unsigned short int logInInput; 
	string logInMenu1 = "1. Create Profile\n2. Log In To Existing Profile\nOption: ";
	dataPacket.clear();

	dataPacket << logInMenu1;
	socket.send(dataPacket);
	dataPacket.clear();
	socket.receive(dataPacket);

	dataPacket >> logInInput;
	cout << logInInput;

	if (dataPacket >> logInInput)
	{
		cout << "test log input: " << logInInput;
	}

	cout << "\n\n";

	dataPacket.clear();

	if (logInInput == 1)
	{
		cout << "Input taken\n";
		cout << "Working \n";
		logInCreate();
	}
	if (logInInput == 3)
	{
		return;
	}
}

void profile()
{

}

////////////////////////////////////////////////////////////
///
/// Allow client to choose what they want to do on server
///
////////////////////////////////////////////////////////////

void gameServer()
{
	unsigned short int userID = 100;
	unsigned short int input;

	string gameServerMenu1 = "\nWelcome to the Game Server, Player ";
	sf::Uint16 newID = latestID;
	string gameServerMenu2 = "\n \nPlease select which option you would like: \n1. Log in \n";
	string gameServerMenu3 = "2. See Player List\n3. See Lobby List\n4. Create Lobby\n5. View Profile (when logged in)\n";
	string gameServerMenu4 = "6. Disconnect\n\nOption: ";
	dataPacket << gameServerMenu1 << newID << gameServerMenu2 << gameServerMenu3 << gameServerMenu4;
	socket.send(dataPacket);

	dataPacket.clear();
	socket.receive(dataPacket);

	cout << "Server Option: ";

	if (dataPacket >> input)
	{
		cout << input;
	}

	cout << "\n\n";

	// Working
	if (input == 1)
	{
		dataPacket.clear();
		cout << "logInFunction\n";
		logIn();
		cout << "Back to server\n";
		gameServer();
	}
	// Works - doesnt send player list to client
	if (input == 2)
	{
		dataPacket.clear();
		playerList();
		gameServer();
	}
	// Not working
	if (input == 3)
	{
		dataPacket.clear();
		lobbyList();
		gameServer();
	}
	// Working but displays nothing on either side
	if (input == 4)
	{
		dataPacket.clear();
		lobbyCreation();
		gameServer();
	}
	// Working but displays nothing on either side
	if (input == 5)
	{
		dataPacket.clear();
		profile();
		gameServer();
	}
	if (input == 6)
	{

	}
}

////////////////////////////////////////////////////////////
///
/// Launch a server, wait for an incoming connection,
/// send a message and wait for the answer.
///
////////////////////////////////////////////////////////////

void runTcpServer(unsigned short port)
{
	// Create a server socket to accept new connections
	sf::TcpListener listener;

	// Listen to the given port for incoming connections
	if (listener.listen(port) != sf::Socket::Done)
		return;
	std::cout << "Server is listening to port " << port << ", waiting for connections... " << std::endl;

	// Wait for a connection
	if (listener.accept(socket) != sf::Socket::Done)
		return;
	std::cout << "Client connected: " << socket.getRemoteAddress() << std::endl;

	string testList = "Hello User \n";
	dataPacket << testList;
	socket.send(dataPacket);
	dataPacket.clear();
	gameServer();
}

int main()
{
	const unsigned short port = 50000;

	players[0] = new Player("Aaron", 100, "test");
	players[1] = new Player("Chris", 101, "hello");

	activePlayerList[0] = players[0]->playerID;
	latestID++;
	activePlayerList[1] = players[1]->playerID;
	latestID++;
	activePlayerNames[0] = players[0]->name;
	activePlayerNames[1] = players[1]->name;

	lobbies[0] = new LobbySystem("LobbyTest", playerLimit, 0, req, 0, 4, false);
	lobbies[1] = new LobbySystem("Placeholder", playerLimit, 0, req, 0, 2, false);

	runTcpServer(port);

	// Makes the server client not close program when a function is complete
	cout << "Press enter to exit..." << endl;
	cin.ignore(10000, '\n');
	cin.ignore(10000, '\n');

	return EXIT_SUCCESS;
};