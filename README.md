# Mini Project — Client-Server Communication (UDP & TCP in C / Unix)

> Implementation of a basic **client-server communication system** under Unix using **UDP** and **TCP** protocols.  
> Developed as part of the *System & Network Programming* module.

---

## Project Description
This mini project demonstrates how to implement communication between a **client** and a **server** under a Unix environment using two major transport protocols:

- **UDP (User Datagram Protocol)** — lightweight, fast but unreliable communication
- **TCP (Transmission Control Protocol)** — reliable and ordered communication

It also includes a simple **authentication mechanism** and an optional **GTK+ GUI**.

---

## Features
- **UDP communication** (connectionless):
  - Sending and receiving datagrams between client and server
  - No guarantee of delivery or order
- **TCP communication** (connection-oriented):
  - Three-way handshake connection
  - Authentication (username/password)
  - Menu-based interaction
- Server handles multiple client requests
- Optional GUI with **GTK+** (bonus part of the assignment)

---

## Architecture Overview

```

client/                 # Client-side source files
├─ client_udp.c
├─ client_tcp.c
└─ ...
server/                 # Server-side source files
├─ server_udp.c
├─ server_tcp.c
└─ ...
Makefile
tests/                  # Example test cases
doc/                    # Project report

````

---

## Tech Stack
- **Language:** C
- **OS:** Unix / Linux
- **Protocols:** UDP, TCP
- **Libraries:** `sys/socket.h`, `arpa/inet.h`, `gtk/gtk.h` (optional GUI)
- **Tools:** `gcc`, `make`, terminal

---

## How to Compile and Run

### 1. Compile the server and client
```bash
make
````

### 2. Start the server

```bash
./server_tcp
# or
./server_udp
```

### 3. Start the client

```bash
./client_tcp
# or
./client_udp
```

The TCP version requires **login credentials** (username & password) before interaction.

---

## Example Usage (TCP)

```
Client:  → connects to server
Server:  → accepts connection
Client:  → enters username/password
Server:  → validates credentials
Client:  → selects menu option
Server:  → processes request and sends response
```

---

## Problems Encountered

During the project, integrating **GTK+** for the graphical interface caused compilation and linking issues.
The core TCP/UDP implementation works properly, while GUI remains optional.

---

## Future Improvements

* Full integration of GTK+ GUI
* Multi-threaded server to handle concurrent clients
* Error handling and timeout management
* Encrypted communication (TLS/SSL)

---

## Author

Developed by **Barouni Abir**


Would you like me to make a **shorter version (2–3 sections)** suitable for a GitHub portfolio, or keep this **full academic version**?
```
