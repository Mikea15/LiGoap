
# LiGOAP

LiGOAP is a "little" GOAP (Goal Oriented Action Planner) implementation done as an exercise and to research the topic. Its unsupported and provided as is.

## Features
- Facts has booleans only
- Uses bitset as an unordered_map<enum, bool>
- A* Planner implementation
- Simple Benchmark loop over 10s to understand perf. implication on each change I make
- Each Fact is identifiable via an ENum key.
- Actions are Non Copiable

## Performance Journey

| Implementation                                                                      |   Release Mode |
|:------------------------------------------------------------------------------------|---------------:|
| First naive Implementation                                                          |   135 plans /s |
| Using scratch vector for reuse and reserving vector space to minimize growth/copies |   147 plans /s |
| Replaced String keys for Enums, removing all string compares                        |   698 plans /s |
| Gathered data for open list average size and increased reserved space               |   873 plans /s |
| Prevent Copy of Actions. Use array of pointers. Prevent allocations.                |  1330 plans /s |
| Change WorldState from UnorderedMap<Key, Bool> to uint64_t (used as bitset)         | 26685 plans /s |
| Change Priority Queue, with a container with reserved space                         | 28888 plans /s |


| Replace vector-based priority queue with Binary Heap | 11k -> 17k plans /s |
| Replace vector-based binary-heap with arena allocated mem block | |

## Licence

Do whatever.









