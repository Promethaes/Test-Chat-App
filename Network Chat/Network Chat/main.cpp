#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

#include "Networking.h"

//dont worry about this stuff, we'll look into it later
//std::map<std::string, std::string> g_pages;
//std::mutex g_pages_mutex;



void runNetwork(Network* network) {
	printf("entering thread 3\n");

	while (true)
		network->listen();

	printf("exiting thread 3\n");
}

void chatFunc(Network* network, std::string ip) {
	while (true) {
		std::string temp = "";
		std::getline(std::cin, temp);
		if (temp == "exit")
			break;
		if (temp == "clear")
			system("cls");
		network->sendMessage(temp, ip);
	}
}

int main()
{
	Network* network = new Network(5400, "192.168.0.102", false);

	std::string temp = "192.168.0.100";
	if (!network->isHosting()) {
		printf("enter ip to connect to\n");
		std::getline(std::cin, temp);


		network->sendMessage("connected", temp);
	}
	//construction "ignites" the thread, calls the callback that you put in
	//you can put in function parameters if the callback requires paramters (eg. chatThread(chatFunc,name,id))
	std::thread chatThread(chatFunc, network, temp);
	std::thread networkThread(runNetwork, network);

	//detach lets THIS thread continue without being blocked by the networkThread
	networkThread.detach();

	//join does the opposite, as THIS thread's execution is blocked until the chatThread has finished executing.
	chatThread.join();

}