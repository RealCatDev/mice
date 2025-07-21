#ifndef   _MICE_SERVER_H_
#define   _MICE_SERVER_H_

typedef struct _Mice_Server *Mice_Server;

Mice_Server mice_server_create();
void mice_server_destroy(Mice_Server server);

void mice_server_run(Mice_Server server);

#endif // _MICE_SERVER_H_