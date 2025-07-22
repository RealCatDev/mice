#include "server.h"

int main(void) {
  // TODO: Add argument parsing
  // TODO: Add configuration

  _Mice_Server server = {0};
  _mice_server_run(&server);

  return 0;
}