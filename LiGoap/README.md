
A journey implementing a simple c GOAP

## Naive Implementation

| Implementation                                                                      |    Debug Mode |   Release Mode |
|:------------------------------------------------------------------------------------|--------------:|---------------:|
| First naive Implementation                                                          |    8 plans /s |   135 plans /s |
| Using scratch vector for reuse and reserving vector space to minimize growth/copies |  9.8 plans /s |   147 plans /s |
| Replaced String keys for Enums, removing all string compares                        |   47 plans /s |   698 plans /s |
| Gathered data for open list average size and increased reserved space               |   51 plans /s |   873 plans /s |
| Prevent Copy of Actions. Use array of pointers. Prevent allocations.                |   84 plans /s |  1330 plans /s |
| Change WorldState from UnorderedMap<Key, Bool> to uint64_t (used as bitset)         | 1109 plans /s | 26685 plans /s |
| Change Priority Queue, with a container with reserved space                         | 1300 plans /s | 28888 plans /s |









