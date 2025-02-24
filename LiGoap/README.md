
A journey implementing a simple c GOAP

## Naive Implementation

| Implementation                                                                      |   Debug Mode | Release Mode |
|:------------------------------------------------------------------------------------|-------------:|-------------:|
| First naive Implementation                                                          |   8 plans /s | 135 plans /s |
| Using scratch vector for reuse and reserving vector space to minimize growth/copies | 9.8 plans /s | 147 plans /s |
| Replaced String keys for Enums, removing all string compares                        |  47 plans /s | 698 plans /s |
| Gathered data for open list average size and increased reserved space               |  51 plans /s | 873 plans /s |



