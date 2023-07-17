
### Basic Operations

| Operation            | poll | epoll | kqueue | io_uring | iocp |
| async_accept         | X    | X     | X      | X        | -    |
| async_connect        | X    | X     | X      | X        | -    |
| async_read_some      | X    | X     | X      | X        | -    |
| async_receive        | X    | X     | X      | X        | -    |
| async_receive_from   | X    | X     | X      | X        | -    |
| async_send           | X    | X     | X      | X        | -    |
| async_send_to        | X    | X     | X      | X        | -    |
| async_wait           | -    | -     | -      | -        | -    |
|    async_sleep_for   | X    | X     | X      | X        | -    |
|    async_sleep_until | -    | -     | -      | -        | -    |
| async_write_some     | X    | X     | X      | X        | -    |

### Algorithms

- async_read
- async_read_until
- async_resolve
+ async_write

### Extended Operations

Anything beyond that (based on the
[`io_uring`](https://man.archlinux.org/man/io_uring_enter.2.en)?

- async_nop (currently I'm using this operation to schedule non-I/O work on the I/O scheduler)
- async_read_fixed/async_write_fixed: I imagine these being used based on special buffers
- async_fsync
- async_close
- async_fallocate
- async_fadvise
- async_madvise
- async_stat (whatever that is called in the file system library)
- async_shutdown
- async_unlink
- async_link
- async_symlink
- async_mkdir
- async_rename

- async_open
- async_poll
- various operations present for io_uring
