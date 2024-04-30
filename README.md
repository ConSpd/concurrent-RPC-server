# RPC server

RPC (Remote Procedure Call) in C lets programs execute functions on remote computers as if they were local. It simplifies building distributed systems by handling network communication complexities. The server process can handle multiple client requests and deals with the following services.

A client sends a number $a \in \mathbb{R}$ and a vector $Y = (y_1, y_2, ..., y_n) \in \mathbb{N}$ where $n$ is defined by the user.
Afterwards it offers the following services.

1. Returns the average of $Y$.
2. Returns the max and min element of $Y$ in an array of 2 integers.
3. Returns the product $a * Y$ which is a vector of real numbers of length $n$.

Each socket-Client process will read from the keyboard (iteratively, until the user indicates that it does not wish to continue) 
(a) the choice of the calculation the user wishes to perform (1,2,3) and 
(b) the respective-necessary data $(n, Y, a)$, feed it to the socket-Server process and wait for it to receive the result to print it on the screen.

### Files
---
`client.c`. Contains the source code of the socket client. It is the final recipient of the messages and the one who initiates the communication. 
It sends the information to the socket server which is also an RPC client and receives the results accordingly.

`service_client.c`. It is both the Socket Server and the RPC client. It is the intermediate link between the socket client and the RPC server. 
It presents the socket client with the options and accepts the data.

`service_server.c`. Contains the 3 services. It gets called by the RPC Client.

`service.x`. It creates the template needed for the RPC communication. The `Makefile` will create all the necessary files. 

### Compilation & Run
---
```
gcc client.c -o client
make -f Makefile
./service_server
./service_client localhost
./client
```

### Screenshots of App Running
---
`./make -f Makefile`
<br>
<img src="https://github.com/ConSpd/concurrent-RPC-server/assets/74179715/416f0ace-b263-4e83-a74a-c0839a8cd7dc" width=500>
<br><br>

`./service_server`
<br>
<img src="https://github.com/ConSpd/concurrent-RPC-server/assets/74179715/d9e76984-492f-48c6-9516-061c3fb6928f" width=500>
<br><br>

`./service_client localhost`
<br>
<img src="https://github.com/ConSpd/concurrent-RPC-server/assets/74179715/4ffc076b-0350-4962-85cb-a7544edca5cc" width=500>
<br><br>

`./client`
<br>
<img src="https://github.com/ConSpd/concurrent-RPC-server/assets/74179715/534513c6-40af-4ecf-a337-93457bfe81b4" width=500>
<br><br>

Application Running
<br>
<img src="https://github.com/ConSpd/concurrent-RPC-server/assets/74179715/dc3a788c-4bf9-4991-ac22-179a981b2781" width=200>
<br><br>

Multiple Clients
<br>
<img src=https://github.com/ConSpd/concurrent-RPC-server/assets/74179715/7852d98b-375d-4b2a-82d9-c54e9e711069 width=700>

