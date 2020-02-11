/*
 * CommandHandler.hh
 *
 *  Created on: 12.11.2008
 *      Author: Patsa
 */

#ifndef COMMANDHANDLER_HH_
#define COMMANDHANDLER_HH_

#include "authority.hh"
#include "channels.hh"
#include "quotes.hh"
#include <string>
#include <vector>

using std::string;

class CommandHandler {
public:
  
  CommandHandler();
  
  string setUMode(string channel, string command, string nick, string host, string target);

  string loadAuth(string channel, string host);
  
  string joinAllChannels();

  string joinEvent(string channel, string nick, string host);
    
  string addQuote(string channel, string quote);
  string getQuote(string channel);
  string removeLastQuote(string channel, string host);
  
private:
  Channels channels;
  Authority auth;
  Quotes quotes;
  
  bool isCommand(string command);
};

#endif /* COMMANDHANDLER_H_ */
