#ifndef PTYB_CLIENT_INCLUDED
#define PTYB_CLIENT_INCLUDED

int init_client(char *sock_domain);

int client_main(int master);
int init_shell(int maid);

int server_connect(char *sock_domain);

int ptyb_message_server(char *sock_domain, char *message);
int ptyb_get_cid(char *sock_domain);

#endif
