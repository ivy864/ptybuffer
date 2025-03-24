#ifndef PTYB_CLIENT_INCLUDED
#define PTYB_CLIENT_INCLUDED

int init_client();

int client_main(int master);
int init_shell(int maid);

int server_connect(char *socket_domain);

#endif
