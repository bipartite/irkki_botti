
/*
 * CommandHandler.cpp
 *
 *  Created on: 12.11.2008
 *      Author: Patsa
 */

#include <iostream>
#include <sstream>
#include <vector>
#include "commandHandler.hh"

using std::string;

CommandHandler::CommandHandler() {}



bool CommandHandler::isCommand(string command) {
  if (command == "op" || command == "deop" || command == "voice" || command == "devoice" || command == "kick" ||
      command == "addUser" || command == "addAutoOp" || command == "addAutoVoice" || command == "removeUser" ||
      command == "removeAutoOp" || command == "removeAutoVoice" || command == "join" || command == "leave" || 
      command == "leave" || command == "loadQuotes")
    return true;
  return false;
}



string CommandHandler::setUMode(string channel, string command, string nick, string host, string target) {
	
  std::ostringstream oss;
  
  if (!isCommand(command))
    return "PRIVMSG " + channel + " :" + nick + ": unknown command\r\n";
  
  // Komennot, joiden käyttämiseksi tarvitsee user-valtuudet
  if (!auth.isUser(host))
    return "PRIVMSG " + channel + " :" + nick + ": you don't have the authority to do that\r\n";
  
  if (command == "op") 	  
    return "mode " + channel + " +o " + target + "\r\n";
  
  if (command == "voice")
    return "mode " + channel + " +v " + target + "\r\n";
  
  if (command == "deop")
    return "mode " + channel + " -o " + target + "\r\n";
  
  if (command == "devoice")
    return "mode " + channel + " -v " + target + "\r\n";
  
  if (command == "kick")
    return "kick " + channel + " " + target + "\r\n";
  
  if (command == "addUser") {
    if (!auth.addUser(target))
      return "PRIVMSG " + channel + " :" + nick + ": Adding user " + target + " failed\r\n"; 
    return "PRIVMSG " + channel + " :" + nick + ": user " + target + " added\r\n";
  }
    
  if (command == "addAutoOp") {
    if (!auth.addAutoOp(channel, target))
      return "PRIVMSG " + channel + " :" + nick + ": Adding the autoOp-status for " + target + " failed\r\n"; 
    return "PRIVMSG " + channel + " :" + nick + ": AutoOp-status added for " + target + "\r\n";
  }
    
  if (command == "addAutoVoice") {
    if (!auth.addAutoVoice(channel, target))
      return "PRIVMSG " + channel + " :" + nick + ": Adding the autoVoice-status for " + target + " failed\r\n"; 
    return "PRIVMSG " + channel + " :" + nick + ": AutoVoice-status added for " + target + "\r\n";
  }
    
  if (command == "removeUser") {
    if (!auth.removeUser(target))
      return "PRIVMSG " + channel + " :" + nick + ": Removing user " + target + " failed\r\n"; 
    return "PRIVMSG " + channel + " :" + nick + ": User " + target + " was removed\r\n";
  }
    
  if (command == "removeAutoOp") {
    if (!auth.removeAutoOp(channel, target))
      return "PRIVMSG " + channel + " :" + nick + ": Removing AutoOp-status from " + target + " failed\r\n";
    return "PRIVMSG " + channel + " :" + nick + ": AutoOp-status of " + target + " was removed\r\n";
  }
    
  if (command == "removeAutoVoice") {
    if (!auth.removeAutoVoice(channel, target))
      return "PRIVMSG " + channel + " :" + nick + ": Removing AutoVoice-status from " + target + " failed\r\n";
    return "PRIVMSG " + channel + " :" + nick + ": AutoVoice-status of " + target + " was removed\r\n";
  }	
    
  if (command == "join") {
    // Kutsutaan myös channels-luokan funktiota
    channels.addChannel(target);
    return "JOIN " + target + "\r\n";
  }
  
  if (command == "leave" || command == "part") {
    // kutsutaan myös channels-luokan funktiota
    channels.removeChannel(target);
    return "PART " + target + "\r\n";
  }
    
  if (command == "loadQuotes") {
    if (quotes.loadQuotes(target))
      return "PRIVMSG " + channel + " :Quotes loaded\r\n";
    return "PRIVMSG " + channel + " :Loading quotes failed\r\n";
  }
  
  // Jos jostain syystä vielä ei olla palautettu mitään, palautetaan pelkästään tyhjä arvo
  return "";
}



string CommandHandler::loadAuth(string channel, string host) {
  if (auth.isUser(host)) {
    if (auth.loadAuth())
      return "PRIVMSG " + channel + " :Authorities loaded\r\n";
    return "PRIVMSG " + channel + " :Loading authorities failed\r\n";
  }
  return "PRIVMSG " + channel + " :You don't have the authority to do that\r\n";
}



string CommandHandler::joinAllChannels() {
  return channels.joinAllChannels();
}



string CommandHandler::joinEvent(string channel, string nick, string host) {
  // Authority-luokan getAutoStatus()-funktio palauttaa arvon 2, mikäli käyttäjällä on auto-op status, 1 mikäli autoVoice ja 0 jollei mitään.
  int status = auth.getAutoStatus(channel, host);
  
  switch (status) {
  case 1:
    return "mode " + channel + " +v " + nick + "\r\n";
  case 2:
    return "mode " + channel + " +o " + nick + "\r\n";
  }
  return "";
}



string CommandHandler::addQuote(string channel, string quote) {
  if (quote == "")
    return "PRIVMSG " + channel + " :Enter a proper quote\r\n";
  if (quotes.addQuote(quote))
    return "PRIVMSG " + channel + " :Quote added\r\n";
  return "PRIVMSG " + channel + " :Failed to add quote\r\n";
}



string CommandHandler::getQuote(string channel) {
  return "PRIVMSG " + channel + " :" + quotes.getQuote() + "\r\n";
}



string CommandHandler::removeLastQuote(string channel, string host) {
  if (auth.isUser(host)) {
    if (quotes.removeLastQuote())
      return "PRIVMSG " + channel + " :Quote removed\r\n";
    return "PRIVMSG " + channel + " :Removing quote failed\r\n";
  }
  return "PRIVMSG " + channel + " :You don't have the authority to do that\r\n";
}
