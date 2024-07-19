# mmap

- When mmap is called, it returns a contiguous segment of memory in the heap that can be used for dynamic allocation. This segment is often referred to as a "memory-mapped region" or simply "mapped memory.
- Each segment will have its own metadata and will manage multiple blocks within that segment

## Heap Segment Structure:

- Heap Segment Metadata: Metadata specific to this segment, including pointers to the first block and potentially other management information.
- Blocks: Each block within this segment contains its own metadata and data. The blocks are linked together, typically using a linked list.
```
|---------------------- Memory Mapped Region (Heap Segment) ----------------------|
| Heap Segment Metadata | Block Metadata | Data | Block Metadata | Data | Free Space |
```
- Heap Segment Metadata: 
At the start of each memory-mapped region, there is metadata for managing this segment, including:
    - Size of the segment.
    - Pointers to the first block in this segment.
    - Possibly other management information.

- Block Metadata: 
Each block within the segment has its own metadata, such as:
    - Size of the block.
    - Status: A flag indicating whether the block is free or allocated.
    - Pointers to the next and previous blocks

- Data: 
The actual memory allocated to the program within each block.

- Free Space: 
Any space in the segment not currently allocated to a block.

How Block Metadata is Used
- Size: Helps in navigating from one block to the next and managing the memory layout.
- Status: Indicates whether the block is currently in use or available for allocation.
- Pointers (Next and Previous): Allow the memory manager to traverse the list of blocks within the heap segment. This is crucial for finding free blocks, coalescing adjacent free blocks, and splitting blocks as needed.

- exemple:
```
|--------------------------------------------------- Heap Segment ----------------------------------------------------|
| Segment Metadata | Block Metadata (size=128, free=0, next=ptr2, prev=NULL) | Data | Block Metadata (size=256, free=0, next=NULL, prev=ptr1) | Data | Free Space |
```