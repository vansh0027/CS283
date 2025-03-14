1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

A length-prefixed protocol or an explicit EOF character are used by the remote client to determine when a command's output has ended. The client must handle partial reads by continuously using recv() until the EOF marker occurs or the expected byte count is received because TCP is a stream protocol. Methods include using delimiters to separate responses, buffering data, and checking for recv() returning 0. Using these, entire message delivery is guaranteed, data loss is usually avoided, and waiting endlessly for additional data after the message has been fully received is abstained.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A networked shell protocol can detect and specify command boundaries by employing length-prefixed messages (send the message size before the actual command) or explicit delimiters (the newline EOF marker). These techniques make sure that messages from the TCP stream can be precisely reconstructed by the recipient. Improper handling can lead to problems like client processing partial data, many instructions received simultaneously, or commands split across numerous packets. This may result in data corruption, improper command execution, or endless blocking while awaiting the completion of a command.

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols save session information between the client and server. The server manages client state, which enables smooth interactions and long-lasting connections (e.g.SSH, TCP). They need more memory and resources, to handle sessions,
Stateless protocols do not remember past exchanges and handle each request as a separate entity. Every request (such as HTTP) must contain all necessary information from the client. These protocols may need extra overhead to resend context with every request. However, they are fault-tolerant, more scalable, and simpler.


4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP can be used when less overhead and speed are more way more important than reliability. Compared to TCP, UDP is faster and more effective in this case. This is due to the fact that it does not create a connection, send lost packets again, or ensure that they are delivered in a particular order. Applications like gaming and video streaming are ideal where low latency is important, but some data loss is acceptable.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

Sockets are the primary network communication interface provided by the operating system. Whether an application uses TCP (reliable, connection-based) or UDP (rapid, connectionless) to send and receive data over a network, sockets function as endpoints. Programs can communicate over the internet in the same way that they read and write to files by using system functions like socket(), bind(), listen(), connect(), send(), and recv(). By managing lower-level features like protocols and data transmission, this abstraction simplifies networking and frees apps to concentrate on their usefulness rather than the intricacies of networking