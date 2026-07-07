# Ring Distributed Hash Table (Ring DHT)

A C++ implementation of a Chord-style Ring Distributed Hash Table, using a circular identifier space, SHA-1 based node/key hashing, finger tables for efficient routing, and AVL trees for per-node key storage.

## Overview

This project simulates a distributed hash table where machines (nodes) are arranged on a circular identifier space of configurable size. Each machine and key is hashed using SHA-1 into this space, and keys are assigned to the first machine whose identifier is equal to or follows the key's hash (the "successor" node), following the classic Chord DHT design.

Each node maintains:
- A **finger table** for logarithmic-time lookups across the ring.
- An **AVL tree** for storing and efficiently searching its local (key, value) pairs.

The program runs as an interactive CLI that lets you build a ring, add/remove machines and data, and inspect the internal state of any node.

## Features

- Configurable identifier space size (1–160 bits).
- Configurable number of machines, with automatic or manual ID assignment.
- Upload and remove `(key, value)` pairs to/from the network.
- Add and remove machines from a running ring, with automatic key redistribution.
- Print a machine's finger table.
- Print a machine's AVL tree of stored keys.

## Project Structure

| File | Description |
|---|---|
| `main.cpp` | Entry point and interactive CLI menu. |
| `ringDHT.h` | Core `RingDHT` class: ring construction, routing, and machine management. |
| `AVLTree.h` | AVL tree used for local key storage and lookup on each machine. |
| `LinkList.h` / `c_LinkList.h` | Linked list utilities used internally by the ring/finger table structures. |
| `SHA_1.h` | SHA-1 hashing implementation used to map machines and keys onto the identifier space. |
| `UtilityFunctions.h` | Shared helper functions (e.g. modular exponentiation for identifier space math). |
| `Ring_DHT.pdf` | Project write-up / assignment description. |
| `Ring_DHT_Implementation_Document.pdf` | Detailed implementation documentation. |

## Requirements

- A C++ compiler with support for `boost::multiprecision` (used for large integer identifiers, e.g. `int256_t`).
- [Boost](https://www.boost.org/) libraries installed and available on your include path.

## Building

```bash
g++ -std=c++17 main.cpp -o ring_dht
```

> Adjust the compiler flags/include paths as needed depending on where Boost is installed on your system.

## Usage

Run the compiled binary and follow the interactive prompts:

```bash
./ring_dht
```

You'll be asked to:
1. Enter the identifier space size in bits (1–160).
2. Enter the number of machines in the network.
3. Choose whether machine IDs are assigned automatically or manually.

Once the ring is initialized, use the menu to interact with it:

```
1. Upload Data to Network
2. Remove Data from Network
3. Print Finger Table
4. Print AVL Tree
5. Add New Machine to Network
6. Remove Machine from Network
0. Exit
```

## Background

This project was built as part of a Distributed Systems course assignment, implementing the core ideas behind Chord-style distributed hash tables: consistent hashing over a circular identifier space, finger tables for O(log N) routing, and successor-based key ownership.

## License

No license specified. All rights reserved by the author unless stated otherwise.
