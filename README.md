# PA#2 - Concurrent Hash Table
## COP4600: Operating Systems

### A concurrent hash table implemented using linked lists.
#### Reads in a list of commands (*commands.txt*) and executes each one, creating a hash record with each command being executed by a separate thread.

##### Other code used:
- *common.h* (https://github.com/remzi-arpacidusseau/ostep-code/tree/master/include)
- *common_threads.h* (https://github.com/remzi-arpacidusseau/ostep-code/tree/master/include)

##### References:
- rwlock.c (https://github.com/remzi-arpacidusseau/ostep-code/blob/master/threads-sema/rwlock.c)