# Memory pages
One of the problem in this project to have in mind is that for malloc to work well the mmap system call should allocate a number of bytes that align with the OS pages syze.

- what are pages ?
    The operating system manages memory in chunks called pages. The page size can vary by architecture, but it is typically 4096 bytes (4 KB) on many systems.

- a command in shell tells you how many bytes per pages there are in your OS: `getconf PAGESIZE`

- why allocating memory in multiples of  the page size is important ?
    it ensures proper alignment and can optimize memory usage and performance, and avoid fragmentation

- which ffunction return the memory page size ?
    - in macOS: `getpagesize()`
    - in Linux (what we have): `sysconf(_SC_PAGESIZE)`

- we need to define the tiny and medium (large is separate) size as multiples of page sizes and in a way that at least 100 allocations (blocks) can be stored in a zone.
So we need firstly to get the size in bytes of a page depending on the OS we run the program at runtime
```
#define PAGE_SIZE (sysconf(_SC_PAGESIZE))
```
and then we define the size in byte of each zone doing this:
```
#define TINY_ZONE_SIZE (PAGE_SIZE * ((TINY * 100 / PAGE_SIZE) + 1)) 
```
    - `((TINY * 100 / PAGE_SIZE) + 1)` => +1 to round up
    - `* PAGE_SIZE`=> to get the total zone size in bytes
    - exemple:
        - Calculate the number of pages:
        (128 * 100 / 4096) = 12800 / 4096 ≈ 3.125.
        - Add 1 to round up:
        3.125 rounded up becomes 4 pages.
        - Multiply by the page size to get the total zone size in bytes:
        4 * 4096 = 16384 bytes.

- we need to find the right tradeoff between memory usage and performance => Larger zone sizes reduce the number of mmap calls but increase memory overhead ( extra memory used by the system beyond the user-requested allocations). mmap call returns a large block of memory, which can be divided into smaller allocations as needed. This batching approach can lead to more efficient use of system resources.


- yes but whqt exactly is Memory overhead ?
    It includes: 
    - Metadata: Information required to manage allocated memory, such as headers and footers in blocks, and bookkeeping structures for free and used blocks.
    - Alignment Padding: Space added to ensure proper alignment of allocated memory according to system requirements.
    - Internal Fragmentation: The unused space within an allocated memory block due to its size not being a perfect fit for the requested size.

- in summary:
too large zones can lead to wasted space, while too small zones can lead to frequent mmap calls and potentially higher fragmentation.
- Internal Fragmentation:
    - What is it?

    Internal fragmentation happens when memory is allocated in blocks that are larger than what is actually needed. The unused space within these allocated blocks is considered wasted because it cannot be used for other allocations.

- External Fragmentation:
    - What is it?
    External fragmentation occurs when there is enough total free memory available, but it is scattered in small, non-contiguous blocks across the memory space. As a result, even though there might be enough free memory to satisfy a large allocation request, the memory manager cannot find a single contiguous block large enough to fulfill the request.