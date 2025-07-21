#ifndef   __MICE_SERVER_H_
#define   __MICE_SERVER_H_

#include "mice/client.h"

typedef struct _Mice_Server {
  Mice_Client *clients;

  void *platformData;
} _Mice_Server;

#endif // __MICE_SERVER_H_