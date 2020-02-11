#ifndef PARSER_HH
#define PARSER_HH

#include "commandHandler.hh"

#include <string>

using std::string;

class Parser
{
public:
  Parser();
  Parser(string server, string botName);

  string parse(string);
  
  string joinAllChannels();

private:
  string server, botName;
  string channel, command, host, nick, target;
  
  string simplify(string);
  string parseHost(string);
  string parseNick(string);
  CommandHandler handler;
};

#endif
