#include "channels.hh"

#include <iostream>
#include <sstream>
#include <string>
#include <list>

Channels::Channels() {
  std::cout << "Enter irc-channels (separate with spaces):" << std::endl;
  std::string tmp;
  do {
    std::getline(std::cin, tmp);
  } while (tmp == "");
  
  std::istringstream iss(tmp);

  while (iss >> tmp)
    channels.push_back(tmp);
  
}



//näistä pitäisi sitten kutsua handlerista aina kun liitytään tai poistutaan
void Channels::addChannel(std::string str)
{
	channels.push_back(str);
}

void Channels::removeChannel(std::string str)
{
	channels.remove(str);
}

std::string Channels::joinAllChannels()
{
	std::string joinStr;
	for(std::list<std::string>::iterator iter = channels.begin(); iter != channels.end(); iter++)
	{
	  joinStr += "JOIN " + *iter + "\r\n ";

	}
	return joinStr;
}

