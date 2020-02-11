#ifndef CHANNELS_HH
#define CHANNELS_HH

#include <list>
#include <string>

class Channels
{
private:
  std::list<std::string> channels;
  
public:
  Channels();
  
  void addChannel(std::string);
  void removeChannel(std::string);
  std::string joinAllChannels();
};

#endif
