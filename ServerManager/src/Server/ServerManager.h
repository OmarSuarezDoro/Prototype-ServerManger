#include <iostream>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>

#include "../ConfigsFolder/CommonFiles.h"


#ifndef __SERVER_MANAGER__
#define __SERVER_MANAGER__

class ServerManager {
 public:
  virtual void StartServer() = 0;
  virtual void StopServer() const = 0;

 private:
};

#endif