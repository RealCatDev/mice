#include "mice/server.h"

int main(void) {
  // TODO: Add argument parsing
  // TODO: Add configuration

  Mice_Server server = mice_server_create();
  mice_server_run(server);
  mice_server_destroy(server);

  return 0;
}