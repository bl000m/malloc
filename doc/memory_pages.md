# Memory pages
One of the problem in this project to have in mind is that for malloc to work well the mmap system call should allocate a number of bytes that align with the OS pages syze.

- what are pages ?
    The operating system manages memory in chunks called pages. The page size can vary by architecture, but it is typically 4096 bytes (4 KB) on many systems.

- why allocating memory in multiples of  the page size is important ?
    it ensures proper alignment and can optimize memory usage and performance, and avoid fragmentation

- which ffunction return the memory page size ?
    getpagesize()

- we need to define the tiny and medium (large is separate) size as multiples of page sizes
```
#define TINY_ZONE_SIZE (getpagesize() * 2)
#define SMALL_ZONE_SIZE (getpagesize() * 4)
```
- why we multiply * 2 and 4 ?
    - we need to find the right tradeoff between memory usage and performance => Larger zone sizes reduce the number of mmap calls but increase memory overhead ( extra memory used by the system beyond the user-requested allocations). mmap call returns a large block of memory, which can be divided into smaller allocations as needed. This batching approach can lead to more efficient use of system resources.
    - TINY Allocations: For very small allocations (up to 128 bytes), If the page size is 4096 bytes, then a TINY_ZONE_SIZE of getpagesize() * 2 equals 8192 bytes (8 KB). This is often enough to fit many small allocations while reducing the frequency of mmap calls.
    - SMALL Allocations: For slightly larger allocations (up to a few kilobytes), A SMALL_ZONE_SIZE of getpagesize() * 4 equals 16384 bytes (16 KB). This helps accommodate more medium-sized allocations efficiently.

- yes but whqt exactly is Memory overhead ?
    It includes: 
    - Metadata: Information required to manage allocated memory, such as headers and footers in blocks, and bookkeeping structures for free and used blocks.
    - Alignment Padding: Space added to ensure proper alignment of allocated memory according to system requirements.
    - Internal Fragmentation: The unused space within an allocated memory block due to its size not being a perfect fit for the requested size.

- in summary:
too large zones can lead to wasted space, while too small zones can lead to frequent mmap calls and potentially higher fragmentation.