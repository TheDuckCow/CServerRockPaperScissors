After compiling e.g. "gcc CServerMain.c server.o", run "./server.o 1025" or some other higher port number to avoid need of sudo access.

Use telnet as the client, via "telnet 127.0.0.1 1025" for example if the server/client are on the same local machine, otherwise add in the appropriate IP/port. Once connected, the client can provide messages as instructed to the server.
