#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#define INVALID_CLIENT_ID 0xFFFFFFFF

void* client_func(void* arg);
void client_exec(TDATA* arg);
void invalidate_data(int idx);
void client_init(int client_id);

#endif // CLIENT_H_INCLUDED
