# Demo: Structured Networking Echo Server

## Changes

 1. Initial program with minimal setup
 2. Add io_context
 3. Add socket_acceptor
 4. Start sender pipeline: scheduler
 5. add async_accept
 6. pipe completion: just printing -> RUN (does nothing)
 7. run the context: accept, terminate -> RUN (accepts once)
 8. run the pipeline directly
 9. add repeat_effect -> RUN (keeps accepting)
10. add error checking on the result
11. start a basic run_client function (unused)
12. name stream_socket and pass it off
13. start a connection class
14. create connection object with stream
15. add connection dtor logging destruction
16. start client pipeline: schedule
17. add a buffer to read into
18. async_read into buffer
19. start the client -> RUN (waits for a read)
20. add connection move ctor and capture connecton in let_value
21. pipe completion reporting the read -> RUN (prints the read buffer)
22. repeat_effect the client -> RUN (prints multiple buffers)
23. repeat_effect_until -> RUN (destroys the client upon disconnect)
24. pipe let_value to capture n and async_write -> RUN (actually echos half-duplex)
25. when_all + separate pipelines but not quite ready
26. start ring_buffer class
27. operation state for the ring buffer
28. sender for the ringer buffer
29. define the completion signatures for the sender
30. gibberish to be deleted: sender/receiver implementation isn't up to date
31. consume() return a sender
32. add the ring buffer to the connection
33. add sender connect and use ring consume for the writer
34. move consume into the let_value in preparation of adding termination
35. producer() return a sender
36. change the sender to support const (consume)/mutable (producer) buffer
37. change the read side to use produce() buffer
38. add an empty complete() to the ring buffer operation state
39. add receiver to ring buffer's operation state (and store in connect)
40. also capture a pointer to the ring buffer in the state
41. add a template helper choosing the buffer type based on an side index
42. change the sender to use side index instead of buffer type as template argument
43. change the operation state to call "complete()"
44. compute the available size for the completion as the distance between consumer and producer
45. compute start of available buffer and complete if a buffer is available
46. add base for operation state and make complete virtual
47. provide storage for pointer to uncomplete state
48. store the state in case it couldn't complete
49. add advance to record processed buffer and potentially unblock
50. change the client code to advance the respective side
51. add cancel() virtual function and cancel the other upon 0 advance
52. also add immediate completion in operation state when done  and remove done handling (also remove the unused buffer) -> RUN full-duplex echo server
53. replace the explicit accepter pipeline by a coroutine
54. replace the explicit client pipelines by coroutines
55. actually loop in the client coroutines



