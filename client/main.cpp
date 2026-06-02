#include "Client.h"
#include "ClientData.h"
#include "MainWindow.h"
#include <QApplication>
#include <exception>
#include <iostream>

#define EXPECTED_ARGS 2

int main(int argc, char *argv[]) {

  if (argc != EXPECTED_ARGS + 1) {
    std::cerr << "Bad program call. The following call is expected:"
              << "./client <hostname> <servname>" << std::endl;
    return 1;
  }

  const char *hostname = argv[1];
  const char *port = argv[2];

  ClientData clientData = NullClientData{};
  {
    QApplication app(argc, argv);

    MainWindow window(hostname, port);
    QObject::connect(&window, &MainWindow::gameStartRequested,
                     [&](const ClientData &d) { clientData = d; });
    window.show();
    app.exec();
  }

  if (!std::holds_alternative<NullClientData>(clientData)) {
    try {
      Client client(hostname, port, clientData);
      client.run();

    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
      return 1;
    }
  }

  return 0;
}
