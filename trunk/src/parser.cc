#include "parser.hh"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;

Parser::Parser() :  server("irc.cs.hut.fi"), botName("JTTBot") {}
Parser::Parser(string server, string botName) : server(server), botName(botName) {}


string Parser::parse(string str) {
  
  std::istringstream iss(str);
  
  // Tehd��n sy�tteen� saadusta tekstijonosta vektori, jolloin sen k�sittely on helppoa
  std::vector<string> strVec;
  string tmp;
  while (iss >> tmp) {
    strVec.push_back(tmp);
  }
    
  // Liian lyhyit� sy�tteit� ei k�sitell�
  if (strVec.size() < 1)
    return "";


  /** serverin PING-komentoon vastaaminen on niin triviaalia, ettei sit� kannata kierr�tt�� commandHandlerin kautta
      serverilt� tulee siis sy�te "PING :<server>" johon vastataan "PONG <server>". **/
  if (strVec[0] == "PING") {
    return "PONG " + server + "\r\n";
  } 
  
  /** Erotellaan sy�tteest� ilmoitukset, kun henkil� liittyy kanavalle, ja kutsutaan handlerin joinEvent-funktiota.
      T�ss� sy�tteest� parsitun kanavan nimen edess� on kaksoispiste, mik� pit�� saada pois ts. sy�te on muotoa
      ":<nick>!<host> JOIN :<kanava>" **/
  if (strVec[1] == "JOIN") {
    channel = simplify(strVec[2]);
    host = parseHost(strVec[0]);
    nick = parseNick(strVec[0]);
    return handler.joinEvent(channel, nick, host);
  }

  /** Mik�li botille annetaan jollain kanavalla komento, on tuleva sy�te muotoa 
      ":<nick>!<host> PRIVMSG <kanava> :<botin nick> <komento>". Sy�te parsitaan ja kutsutaan asianmukaisia commandHandlerin
      funktioita. **/
  if (strVec.size() > 4 && strVec[3] == ":" + botName) {
    // Parsitaan sy�tteest� kanava, nick, host ja komento
    channel = strVec[2];
    host = parseHost(strVec[0]);
    nick = parseNick(strVec[0]);
    command = strVec[4];
    
    // tarkistetaan aluksi onko kyseess� quote-toimintoihin liittyv� komento      
    // addQuote-komennon j�lkeen tuleva tektijono kasataan taas string-muuttujaksi ennen commandHandlerin kutsumista
    if (command == "addQuote") {
      string quote;
      for (unsigned int i = 5; i < strVec.size(); i++)
	quote += strVec.at(i) + " ";
      return handler.addQuote(channel, quote);
    }
      
    if (command == "getQuote")
      return handler.getQuote(channel);
    
    if (command == "removeLastQuote")
      return handler.removeLastQuote(channel, host);
    
    // Parametrit�n loadQuotes-komento k�sitell��n t�ss�
    if (command == "loadQuotes" && strVec.size() == 5)
      return handler.setUMode(channel, command, nick, host, "");

    // ja loadAuth-komento t�ss�
    if (command == "loadAuth" && strVec.size() == 5)
      return handler.loadAuth(channel, host);

    // Muissa komennoissa on my�s kohde, ja niit� varten on setUMode-funktio commandHandlerissa.
    if (strVec.size() != 6) {
      return "PRIVMSG " + channel + " :Give commands in the following form:  <bot's nick> <command> <target>\r\n";      
    }
    
    target = strVec[5];

    return handler.setUMode(channel, command, nick, host, target);
  }
  
  /** Jos sy�tteen� tulee pelkk� botin nick, osoitetaan elonmerkkej� **/
  if (strVec.size() == 4 && strVec[3] == ":" + botName) 
    return "PRIVMSG " + strVec[2] + " :Hi " + parseNick(strVec[0]) + "\r\n";

  // Muuten palautetaan tyhj� string.
  return "";
}



// main kutsuu t�t� funktiota, mik�li botti yhdist�� uudestaan serverille.
string Parser::joinAllChannels() {
  return handler.joinAllChannels();
}



// poistaa ik�v�t kaksoispisteet string-muuttujien edestä
string Parser::simplify(string str) {
  return str.substr(1, str.size());
}



// funktio parsii nickin nick-hostin ketjusta
string Parser::parseNick(string str) {
  // etsitään :Esim!eaesim@palvelin.fi -muotoisesta hostista nick
  size_t end = str.find_first_of('!') - 1;
  return str.substr(1, end);
}



// toimii samoin kuin parseNick, mutta parsii hostin
string Parser::parseHost(string str) {
  size_t begin = str.find_first_of('!') + 1;
  return str.substr(begin, str.size());
}
