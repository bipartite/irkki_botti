#include "authority.hh"

#include <fstream>
#include <iostream>

using std::string;



Authority::Authority() {
  /** Konstruktori lataa k‰ytt‰j‰tiedot load()-funktiolla. Mik‰li funktion toiminnassa tulee ongelmia,
      k‰ytt‰j‰n pit‰isi saada siit‰ tieto. **/
  load();
}



bool Authority::addUser(string host) {
  if (contains(users, host)) {
    std::cout << host << " is already in users-list" << std::endl;
    return false;
  }
  
  users.push_back(host);
  
  if (!save()) {
    users.remove(host);
    std::cout << "Adding " << host << " to users failed" << std::endl;
    return false;
  }

  std::cout << host << " saved to users-list" << std::endl;

 return true;
}



bool Authority::addAutoOp(string channel, string host) {
  string newAutoOp = channel + " " + host;
  if (contains(autoOp, newAutoOp)) {
    std::cout << host << " is already in autoOp-list in channel " << channel << std::endl;
    return false;
  }
  
  autoOp.push_back(newAutoOp);
  
  if (!save()) {
    autoOp.remove(host);
    std::cout << "Adding " << host << " to AutoOp-list in channel " << channel << " failed" << std::endl;
    return false;
  }

  std::cout << host << " saved to autoOp-list in channel " << channel << std::endl;

  return true;
}



bool Authority::addAutoVoice(string channel, string host) {
  string newAutoVoice = channel + " " + host;
  if (contains(autoVoice, newAutoVoice)) {
    std::cout << host << " is already in autoVoice-list in channel " << channel << std::endl;
    return false;
  }
  
  autoVoice.push_back(newAutoVoice);
  
  if (!save()) {
    autoVoice.remove(host);
    std::cout << "Adding " << host << " to AutoVoice-list in channel " << channel << " failed" << std::endl;
    return false;
  }

  std::cout << host << " saved to autoVoice-list in channel " << channel  << std::endl;
  
  return true;
}



bool Authority::isUser(string host) {
  return contains(users, host);
}



bool Authority::removeUser(string host) {
  if (!contains(users, host)) {
    std::cout << "Users-list does not contain user " << host << std::endl;
    return false;
  }

  users.remove(host);
  if (!save()) {
    users.push_back(host);
    std::cout << "removing user " << host << " failed" << std::endl;
    return false;
  }

  std::cout << host << " removed from users" << std::endl;

  return true;
}



bool Authority::removeAutoOp(string channel, string host) {
  if (!contains(autoOp, channel + " " + host)) {
    std::cout << "autoOp-list doesn't contain " << host << " in channel " << channel << std::endl;
    return false;
  }

  autoOp.remove(channel + " " + host);
  
  if (!save()) {
    autoOp.push_back(channel + " " + host);
    std::cout << "removing autoOp-status of " << host << " in channel " << channel << " failed" << std::endl;
    return false;
  }
  
  std::cout << host << " autoOp-status removed in channel " << channel << std::endl;
  
  return true;
}



bool Authority::removeAutoVoice(string channel, string host) {
  if (!contains(autoVoice, channel + " " + host)) {
    std::cout << "autoVoice-list doesn't contain " << host << " in channel " << channel << std::endl;
    return false;
  }

  autoVoice.remove(channel + " " + host);
  
  if (!save()) {
    autoVoice.push_back(channel + " " + host);
    std::cout << "removing autoVoice-status of " << host << " in channel " << channel << " failed" << std::endl;
    return false;
  }

  std::cout << host << " autoVoice-status removed in channel " << channel << std::endl;

  return true;
}



int Authority::getAutoStatus(string channel, string host) {
  string checkStr = channel + " " + host;
  if (contains(autoOp, checkStr))
    return 2;
  if (contains(autoVoice, checkStr))
    return 1;
  return 0;
}



bool Authority::loadAuth() {
  return load();
}



bool Authority::contains(std::list<string> list, string str) {
  std::list<string>::iterator iter = list.begin();
  while (iter != list.end()) {
    if (*iter == str)
      return true;
    iter++;
  }
  return false;
}



bool Authority::save() {
  users.sort();
  autoOp.sort();
  autoVoice.sort();
  
  std::list<string>::iterator iter;
  
  std::ofstream out("./authorities.txt");

  try {
    out << "users:\n";
    iter = users.begin();
    while (iter != users.end()) {
      out << *iter << "\n";
      iter++;
    }

    out << "autoOp:\n";
    iter = autoOp.begin();
    while (iter != autoOp.end()) {
      out << *iter << "\n";
      iter++;
    }
    
    out << "autoVoice:\n";
    iter = autoVoice.begin();
    while (iter != autoVoice.end()) {
      out << *iter << "\n";
      iter++;
    }
    
    out << "END\n";
  }
  catch (std::ofstream::failure e) {
    out.close();
    std::cout << "Exception in Authority::save() function" << std::endl;
    return false;
  }
  
  out.close();

  std::cout << "Authorities saved" << std::endl;
    
  return true;
}



bool Authority::load() {
  std::ifstream in;
  in.open("authorities.txt");
  
  try {
    
    //Seuraavat rivit luovat authorities.txt-tiedoston, mik√§li sit√§ ei ole olemassa
    if (!in.is_open()) {
      in.close();
      std::cout << "authorities.txt not found. Enter your host (eq. userName@vipunen.hut.fi) " << std::endl;
      string host;
      std::cin >> host;
      users.push_back(host);
      save();
      return true;
    }

    // Tyhjennet‰‰n listat
    users.clear();
    autoOp.clear();
    autoVoice.clear();

    string line;
    
    getline(in, line);
    if ( line != "users:") {
      std::cout << "Invalid first line: " << line << "\"" << std::endl;;
      return false;
    }
    
    while (getline(in, line) && line != "autoOp:")
      users.push_back(line);
    
    while (getline(in, line) && line != "autoVoice:")
      autoOp.push_back(line);
    
    while (getline(in, line) && line != "END")
      autoVoice.push_back(line);
  } catch (std::ifstream::failure e) {
    in.close();
    std::cout << "Exception in Authority::load() function " << std::endl;
    return false;
  }
        
  in.close();
  
  users.unique();
  autoOp.unique();
  autoVoice.unique();

  std::cout << "Authorities loaded" << std::endl;
  
  return true;
}
