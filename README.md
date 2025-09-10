# Simplified Git-like Version Control : README

**Project:** Simplified in-memory Git-like version control

**Language:** C++ (C++17)

---

## Abstract

This project implements a simplified, in-memory version-control system for individual files, inspired by the core ideas of Git. The system allows users to create, edit, and manage multiple versions of files, supporting operations such as inserting and updating content, creating immutable snapshots, rolling back to previous versions, and querying file history. The project emphasizes modular design, custom data structures, and robust error handling, providing a hands-on experience with version control concepts, memory management, and C++ best practices. The implementation is designed for clarity, extensibility, and educational value, closely following the learning objectives outlined in the assignment brief.

---

## Class Member Variables

### File class
```cpp
class File {
    TreeNode* root;                // Root of the version tree
    TreeNode* active_version;      // Current version
    map<int, TreeNode*> version_map; // Version lookup
    int total_versions;            // Version counter
    // Additional members:
    std::string name;              // Filename for identification and output
    time_t last_modified;          // Last modification time for analytics
};
```

### TreeNode class
```cpp
class TreeNode {
    int version_id;
    std::string content;
    std::string message;           // Empty if not a snapshot
    time_t created_timestamp;
    time_t snapshot_timestamp;     // -1 if not a snapshot
    TreeNode* parent;
    std::vector<TreeNode*> children;
};
```

**Note:**
- `name` is used for user feedback, analytics, and mapping file objects to logical names.
- `last_modified` is used for analytics (e.g., RECENT_FILES) and is standard in file systems/version control.

---

## Key Functions & Commands (with Code Mapping)

Below are the most important user-facing commands, each mapped to where it is handled in the code and how it works internally:

### CREATE <filename>
- **Where:** `main.cpp` (command parsing), calls `File` constructor
- **How:** Checks if the file already exists in the global map. If not, creates a new `File` object, which initializes the version tree with a root node (version 0, empty content, message "Root Snapshot"). Adds the new file to the global map and analytics heaps.

### INSERT <filename> <content...>
- **Where:** `main.cpp` (command parsing), calls `File::insert`
- **How:** Validates arguments, then calls the `insert` method of the corresponding `File` object. `File::insert` appends the new content to the current version's content, always on a new line. Updates the file's last modified time and analytics heaps. Prints a success or error message.

### UPDATE <filename> <content...>
- **Where:** `main.cpp` (command parsing), calls `File::update`
- **How:** Validates arguments, then calls the `update` method of the `File` object. `File::update` replaces the content of the current version node with the new content (does not create a new version). Updates last modified time and analytics heaps. Prints a success or error message.

### SNAPSHOT <filename> <message...>
- **Where:** `main.cpp` (command parsing), calls `File::snapshot`
- **How:** Calls the `snapshot` method of the `File` object, which creates a new `TreeNode` as a child of the current version, copies the current content, assigns a new version ID, sets the snapshot flag and message, and updates timestamps. The new node becomes the active version. Updates analytics heaps.

### ROLLBACK <filename> [versionID]
- **Where:** `main.cpp` (command parsing), calls `File::rollback`
- **How:** If a version ID is provided, attempts to set the active version to the specified snapshot. If not, rolls back to the most recent snapshot. Validates the version exists and is a snapshot. Updates last modified time and analytics heaps. Prints a success or error message.

### HISTORY <filename>
- **Where:** `main.cpp` (command parsing), calls `File::history`
- **How:** Calls the `history` method of the `File` object, which traverses the version tree from root to current, printing version IDs, messages, and timestamps for each snapshot.

### READ <filename>
- **Where:** `main.cpp` (command parsing), calls `File::read`
- **How:** Calls the `read` method of the `File` object, which prints the content of the current (active) version node.

### CURRENT <filename>
- **Where:** `main.cpp` (command parsing), calls `File::getActiveVersionID`
- **How:** Prints the version ID of the current (active) version node.

### RECENT_FILES [num]
- **Where:** `main.cpp` (command parsing), uses `recentFilesHeap`
- **How:** Pops up to `num` files from the max-heap of files sorted by last modified time, printing their names and timestamps.

### BIGGEST_TREES [num]
- **Where:** `main.cpp` (command parsing), uses `BiggestTreesHeap`
- **How:** Pops up to `num` files from the max-heap of files sorted by version tree size, printing their names and sizes.

### EXIT
- **Where:** `main.cpp` (command parsing)
- **How:** Exits the program cleanly, deleting all allocated objects.

All commands provide clear feedback, and invalid usage is met with descriptive error messages.

---

## Project files (overview)

- `main.cpp` — Program entry point and interactive CLI. Maintains global data structures and interprets user commands.
- `file.h` / `file.cpp` — `File` class: header contains declarations, source contains definitions. Manages per-file version tree, version map, metadata and public file operations (INSERT, UPDATE, SNAPSHOT, ROLLBACK, HISTORY, READ, etc.).
- `tree.h` / `tree.cpp` — `TreeNode` class: header contains declarations, source contains definitions. Node used by `File` representing one version (content, parent, children, timestamps, message, snapshot flag, id).
- `map.h` / `map.cpp` — Template `HashMap<K,V>`: header contains declarations, source contains definitions and specializations. Separate-chaining hash table with rehashing. The template relies on `hash` specializations for key types used in this code.
- `heap.h` — Template `Heap<V,K>`: max-heap priority queue with an external index map (a `HashMap<K,int>`) for O(log n) updates. (Template implementation remains in header for portability.)
- `COL_106_Long_Assignment_1.pdf` — Original assignment PDF (for reference).

---

## Build & run

**Requirements:** g++ or another C++17-capable compiler.

From the project directory:

```powershell
# Compile all source files
# (This will automatically pick up all .cpp files, including main.cpp, file.cpp, tree.cpp, map.cpp, heap.cpp)
g++ -std=c++17 -Wall -Wextra -o tfs.exe *.cpp

# Run
./tfs.exe
```

Alternatively create a small script `build.ps1` (for PowerShell):

```powershell
# build.ps1
g++ -std=c++17 -Wall -Wextra -o tfs.exe *.cpp
if ($?) { Write-Host "Build successful" }
```

**To use the provided build/run script on Unix-like systems:**
1. Make the script executable:
   ```sh
   chmod +x .bashrc
   ```
2. Then run it:
   ```sh
   ./bashrc
   ```

---

## CLI commands & usage

**Syntax:**
- `<...>` = compulsory argument
- `[...]` = optional argument

**Note:** Commands are case-insensitive (e.g., `create`, `CREATE`, and `CrEaTe` all work the same), but filenames are case-sensitive and must match exactly.

Commands are typed into the running program. Multi-word content or messages are supported — anything after the filename is treated as content/message.

```
CREATE <filename>
READ <filename>
INSERT <filename> <content...>
UPDATE <filename> <content...>
SNAPSHOT <filename> <message...>
ROLLBACK <filename> [versionID]
HISTORY <filename>
CURRENT <filename>
RECENT_FILES [num]
BIGGEST_TREES [num]
EXIT
```

---

### Example Output

### Example session (input lines prefixed with "> ", output lines follow):
```
> CREATE notes.txt
File created successfully: notes.txt
> CREATE report.txt
File created successfully: report.txt
> INSERT notes.txt Hello world!
Success Inserting into File: notes.txt
> SNAPSHOT notes.txt initial commit
Success Creating Snapshot for File: notes.txt
> INSERT report.txt First report entry
Success Inserting into File: report.txt
> SNAPSHOT report.txt v1
Success Creating Snapshot for File: report.txt
> INSERT notes.txt More notes
Success Inserting into File: notes.txt
> SNAPSHOT notes.txt second commit
Success Creating Snapshot for File: notes.txt
> RECENT_FILES 2
File: notes.txt | Last Modified: <timestamp>
File: report.txt | Last Modified: <timestamp>
> BIGGEST_TREES 2
File: notes.txt | Tree Size: 3
File: report.txt | Tree Size: 2
> INSERT report.txt Second report entry
Success Inserting into File: report.txt
> SNAPSHOT report.txt v2
Success Creating Snapshot for File: report.txt
> RECENT_FILES 2
File: report.txt | Last Modified: <timestamp>
File: notes.txt | Last Modified: <timestamp>
> BIGGEST_TREES 2
File: notes.txt | Tree Size: 3
File: report.txt | Tree Size: 3
> INSERT report.txt Third report entry
Success Inserting into File: report.txt
> SNAPSHOT report.txt v3
Success Creating Snapshot for File: report.txt
> BIGGEST_TREES 2
File: report.txt | Tree Size: 4
File: notes.txt | Tree Size: 3
> INSERT notes.txt Final note
Success Inserting into File: notes.txt
> SNAPSHOT notes.txt third commit
Success Creating Snapshot for File: notes.txt
> BIGGEST_TREES 2
File: notes.txt | Tree Size: 4
File: report.txt | Tree Size: 4
> RECENT_FILES 2
File: notes.txt | Last Modified: <timestamp>
File: report.txt | Last Modified: <timestamp>
> HISTORY notes.txt
[Ver: 0] [Msg: Root Snapshot] Time: <timestamp>
[Ver: 1] [Msg: initial commit] Time: <timestamp>
[Ver: 2] [Msg: second commit] Time: <timestamp>
[Ver: 3] [Msg: third commit] Time: <timestamp>
> HISTORY report.txt
[Ver: 0] [Msg: Root Snapshot] Time: <timestamp>
[Ver: 1] [Msg: v1] Time: <timestamp>
[Ver: 2] [Msg: v2] Time: <timestamp>
[Ver: 3] [Msg: v3] Time: <timestamp>
> EXIT
```

---

## Error Handling Philosophy

- All user input is validated in `main.cpp` before calling any file operations. Missing or invalid arguments result in clear, descriptive error messages.
- All exceptions thrown by `File` methods (e.g., invalid version ID, file not found) are caught in `main.cpp` and reported to the user without crashing the program.
- Every command prints either a success message or a specific error, so users always know the result of their action.
- Defensive programming is used throughout: pointers are checked before dereferencing, and all memory is cleaned up on exit.

---

## Design decisions

- **Header/source file split for maintainability and clarity**
  All classes and functions are split into `.h` (declarations) and `.cpp` (definitions) files. This modular approach improves code readability, maintainability, and build efficiency, and is a best practice in C++ development.

- **Encapsulation: private member variables + getters/setters**
  Class member variables are declared `private` and accessed or modified only via getter/setter methods. This prevents accidental or out-of-order modifications to internal state, centralizes validation logic, and makes invariants easier to reason about and maintain.

- **Explicit success / failure messages for every command**
  Every command prints a clear success or error message (for example, `Success Inserting into File: notes.txt` or a descriptive validation error). Exceptions thrown by `File` methods are caught in `main.cpp` and displayed to the user.

- **Support for multi-word content/messages**
  Commands that accept content or snapshot messages treat "everything after the filename" as the content/message (so multi-word strings are supported without special quoting).

- **Custom HashMap design (separate chaining + rehashing)**
  `HashMap` uses separate chaining with linked lists inside buckets and doubles capacity when load factor crosses a threshold.

- **External index map in `Heap` for efficient updates**
  The heap stores `(priority, value)` pairs in a vector and relies on an external `HashMap<value,int>` to track indices, enabling `updateval` operations in O(log n).

- **Timestamps and lightweight analytics**
  Each `File` and each snapshot stores timestamps. Program-level heaps (`recentFilesHeap`, `BiggestTreesHeap`) use these timestamps and sizes to provide quick analytics without traversing all files for every query.

- **Manual memory management with clear ownership**
  `File` objects and `TreeNode`s are managed using raw pointers and explicit `new`/destructor cleanup. `File` maintains an `all_versions` container to ensure every allocated node is deleted in the destructor.

- **Defensive input validation and error propagation**
  `main.cpp` validates command arguments and converts textual inputs (for example, version IDs) safely. Where `File` methods may throw exceptions (`const char*`), the top-level catches and prints them.

- **Single-responsibility and modular code layout**
  Each class has a focused responsibility: `TreeNode` models a version node, `File` manages a version tree and file-specific logic, `HashMap` and `Heap` provide supporting data structures, and `main.cpp` handles CLI and orchestration.

- **INSERT appends content on a new line**
  INSERT always appends new content on a separate line (implementation prepends "\n"), but only if the file is not empty; if the file is empty, the content is added directly without a leading newline. This avoids unnecessary blank lines at the start of a file.

- **RECENT_FILES time complexity**
  `RECENT_FILES [num]` executes in `O(num * log n)` time (with num < n validated) by popping the top num elements from the max-heap of n files.

- **heap update helpers in main.cpp**
  `updateRecentFilesHeap`, `updateBiggestTreesHeap`, and `updateeval` centralize and encapsulate the logic for updating analytics heaps and their index maps whenever a file's metadata or tree size changes.
- **BIGGEST_TREES tie-breaker:**
If two files have the same tree size, the file with the lexicographically smaller filename (alphabetical order) is ranked higher in `BIGGEST_TREES`. This is enforced in the heap logic.  

- **Commands are case-insensitive (Git-style)**
  All commands are recognized regardless of letter case (e.g., `create`, `CREATE`, `CrEaTe` all work). This is implemented by converting input to uppercase before matching. This approach is more user-friendly, reduces user errors, and is consistent with the behavior of popular tools like Git. **Filenames, however, remain case-sensitive.**

- **Use of size_t and time_t for safety and portability**
  All container sizes, capacities, and indices use `size_t` (an unsigned type guaranteed to hold any container size), which prevents negative values and matches standard library conventions. All timestamps use `time_t`, which is the standard type for time values in C/C++. This improves type safety, prevents subtle bugs, and ensures portability across platforms, compared to using `int`.

- **RECENT_FILES always reflects true recency, even for simultaneous updates**
  If two files are updated in the same second, the system guarantees that the file updated later is always considered more recent in RECENT_FILES. This ensures analytics are robust and unambiguous.
---

## Implementation details

### File (file.h / file.cpp)
**Variables:**
- `std::string name` — filename for identification and output
- `TreeNode* root` — root version node
- `TreeNode* active_version` — current version
- `HashMap<int, TreeNode*> version_map` — version lookup
- `int total_versions` — version counter
- `time_t last_modified` — last modification time

**Methods:**
- `insert`, `update`, `snapshot`, `rollback`, `history`, `read`, `getName`, `getLastModified`, `getTreeSize`, `getActiveVersionID`, destructor

### TreeNode (tree.h / tree.cpp)
**Variables:**
- `int version_id`
- `std::string content`
- `std::string message`
- `time_t created_timestamp`
- `time_t snapshot_timestamp`
- `TreeNode* parent`
- `std::vector<TreeNode*> children`

**Methods:**
- `getContent`, `addChild`, `isSnapshot`, `setContent`, `setSnapshot`, `setMessage`, `setSnapshotTimestamp`, `getVersionID`, `getParent`, `getMessage`, `getSnapshotTimestamp`

### HashMap (map.h / map.cpp)
**Variables:**
- `std::vector<HashNode<K,V>*> table`
- `size_t capacity`
- `size_t size`
- `float load_factor`

**Methods:**
- `insert`, `get`, `remove`, `contains`, `rehash`, `getSize`, `get_size`, `begin`, `end`, `hash` (with specializations)

### Heap (heap.h)
**Variables:**
- `std::vector<std::pair<V,K>> elements`
- `HashMap<K,int>& index_map`

**Methods:**
- `insert`, `extract`, `updateval`, `isEmpty`, `get`, `siftUp`, `siftDown`, `printall`

---
