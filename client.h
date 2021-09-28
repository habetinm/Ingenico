#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#define INVALID_CLIENT_ID 0xFFFFFFFF

void client_exec(TDATA* arg);
void client_invalidate(TDATA* arg);
void client_init(int client_id);

#endif // CLIENT_H_INCLUDED
