# Thread Support Library

## Threads: `<thread>`

| Status | `std` | Name |
| ----- | ---- | --- |
| :recycle: | `std::thread` | `nstd::thread::thread` |
| :recycle: | `std::this_thread::get_id` | `nstd::thread::this_thread::get_id` |
| :recycle: | `std::this_thread::yield` | `nstd::thread::this_thread::yield` |
| :recycle: | `std::this_thread::sleep_until` | `nstd::thread::this_thread::sleep_until` |
| :recycle: | `std::this_thread::sleep_for` | `nstd::thread::this_thread::sleep_for` |

## Mutual Exclusion: `<mutex>`

| Status | `std` | Name |
| ----- | ---- | --- |
| :recycle: | `std::mutex` | `nstd::thread::mutex` |
| :recycle: | `std::recursive_mutex` | `nstd::thread::recursive_mutex` |
| :recycle: | `std::timed_mutex` | `nstd::thread::timed_mutex` |
| :recycle: | `std::recursive_timed_mutex` | `nstd::thread::recursive_timed_mutex` |
| :recycle: | `std::defer_lock_t` | `nstd::thread::defer_lock_t` |
| :recycle: | `std::try_to_lock_t` | `nstd::thread::try_to_lock_t` |
| :recycle: | `std::adopt_lock_t` | `nstd::thread::adopt_lock_t` |
| :recycle: | `std::defer_lock` | `nstd::thread::defer_lock` |
| :recycle: | `std::try_to_lock` | `nstd::thread::try_to_lock` |
| :recycle: | `std::adopt_lock` | `nstd::thread::adopt_lock` |
| :recycle: | `std::lock_guard` | `nstd::thread::lock_guard` |
| :recycle: | `std::unique_lock` | `nstd::thread::unique_lock` |
| :recycle: | `std::try_lock` | `nstd::thread::try_lock` |
| :recycle: | `std::lock` | `nstd::thread::lock` |
| :recycle: | `std::once_flag` | `nstd::thread::once_flag` |
| :recycle: | `std::call_once` | `nstd::thread::call_once` |

## Condition Variables: `<condition_variable>`

| Status | `std` | Name |
| ----- | ---- | --- |
| :recycle: | `std::condition_variable` | `nstd::thread::condition_variable` |
| :recycle: | `std::condition_variable_any` | `nstd::thread::condition_variable_any` |
| :recycle: | `std::notify_all_at_thread_exit` | `nstd::thread::notify_all_at_thread_exit` |
| :recycle: | `std::cv_status` | `nstd::thread::cv_status` |
