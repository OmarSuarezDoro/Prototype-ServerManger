
#include "ServerManager.h"

class ServerTCPManager : public ServerManager {
 public:
  ServerTCPManager(int port = 7982, int connections_to_handle = 1);
  virtual void StartServer() override;
  virtual void StopServer() const override;
  void HandleConnection(int client_socket_fd, struct sockaddr_in* information_of_connection);
 private:
  int server_socket_fd_;
  int connections_to_handle_;
  int port_;
  struct sockaddr_in server_socket_configuration_;
  std::vector<std::pair<int, std::string>> connections_vec_;
  // Methods
  void BindServer();
};