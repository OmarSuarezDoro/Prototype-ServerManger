
#include "ServerTCPManager.h"

std::string PrintMessage(int m_option, std::string string_to_print, std::string additional_options = "") {
  std::string result;
  switch (m_option) {
    case MESSAGE:
      result = NC + std::string("[i] ") + GRAY + string_to_print + NC;
      break;
    case QUESTION:
      result = BLUE + std::string("[?] ") + GRAY + string_to_print + NC + additional_options + GRAY + ": ";
      break;
    case WARNING:
      result = YELLOW + std::string("[!] ") + GRAY +  string_to_print + NC;
      break;
    case ERROR:
      result = RED + std::string("[-] ") + string_to_print + NC;
      break;
    case PERFECT:
      result = GREEN + std::string("[+] ") + GRAY + string_to_print + NC;
      break;
  }
  return result;
}

ServerTCPManager::ServerTCPManager(int port, int connections_to_handle) {
  connections_to_handle_ = connections_to_handle;
  if (port == 7982) {
    std::cout << PrintMessage(WARNING, "Careful you haven't selected a port. A random port is going to be selected if you start the server.") << std::endl;
  }
  port_ = port;
  server_socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  server_socket_configuration_.sin_family = AF_INET;
  server_socket_configuration_.sin_port = htons(port);
  server_socket_configuration_.sin_addr.s_addr = INADDR_ANY;
  BindServer();
}

void ServerTCPManager::StartServer() {
  try {
    if (listen(server_socket_fd_, connections_to_handle_) != 0) {
      throw errno;
    }
    std::cout << PrintMessage(MESSAGE, "Socket Listening on " + std::to_string(port_) + " Succesful.") << std::endl;
    // Bucle infinito recibiendo peticiones
    while (true) {
      struct sockaddr_in* information_struct;
      int client_socket = accept(server_socket_fd_, (struct sockaddr*)& information_struct, (socklen_t* )& information_struct);
      if (client_socket != 0) {
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(information_struct->sin_addr), client_ip, INET_ADDRSTRLEN);
        std::cout << PrintMessage(PERFECT, "Connection incoming from " + std::string(client_ip)) << std::endl;
        HandleConnection(client_socket, information_struct);
      } else {
        // Agregar mensaje, si se ve conveniente
        close(client_socket);
      }
    }
  } catch (...) {
    std::cerr << PrintMessage(ERROR, "ERROR Starting the Server: ") << std::endl;
  }
}



void ServerTCPManager::StopServer() const {
  for (const auto& connection : connections_vec_) {
    std::cout << PrintMessage(MESSAGE, "Connection socket from " + connection.second + " have been closed successfuly.") << std::endl;
    if (close(connection.first) != 0) {
      std::cout << PrintMessage(ERROR, "ERROR: Connection from " + connection.second + " couldn't be closed") << std::endl;   
    }
  }
  if (close(server_socket_fd_) != 0) {
    std::cout << PrintMessage(ERROR, "ERROR: Server Socket couldn't be closed") << std::endl;   
  }
  std::cout << PrintMessage(MESSAGE, "Server socket closed successfully") << std::endl;
  std::cout << PrintMessage(PERFECT, "Server has been stopped") << std::endl;
  exit(EXIT_SUCCESS);
}



void ServerTCPManager::BindServer() {
  try {
    if ( bind(
      server_socket_fd_, 
      (struct sockaddr*)& server_socket_configuration_,
      sizeof(server_socket_configuration_)
    ) != 0) {
      throw errno;
    }
    char option = 'n';
    std::cout << PrintMessage(QUESTION, "Would you like to Start the server?", "(y/n)");
    std::cin >> option;
    std::cout << std::endl;
    if (option == 'y') { 
      StartServer();
    } 
  } catch (...) {
    std::cerr << PrintMessage(ERROR, "ERROR in socket Binding :" + errno) << std::endl;
  }
}


void ServerTCPManager::HandleConnection(int client_socket_fd, struct sockaddr_in* information_of_connection) {
  std::cout << "A!" << std::endl;
  char client_ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(information_of_connection->sin_addr), client_ip, INET_ADDRSTRLEN);
  connections_vec_.push_back({client_socket_fd, client_ip});
  StopServer();
  long connections_size = connections_vec_.size();
  char buffer[1024] = "This is a default Message.";
  send(client_socket_fd, &buffer, sizeof(buffer), 0);
  close(connections_vec_[connections_size].first);
  connections_vec_.erase(connections_vec_.begin() + connections_size - 1);
}