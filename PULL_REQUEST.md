### See `Reviewer Guide` below to get a firm grasp on what to prioritize in this PR.

---

## Tree construction system
If a directory is too deep, the recursions can cause a stack overhead or stack overflow. To avoid this, we build the tree iteratively instead of recursively.

---

# Reviewer Guide (Comparing this branch with `the last one`)

Suggested order for reviewing this PR:

### 1. Separation of tree concerns

In PR (9), the recursion functionality of each file or folder dependent operation in `main.cpp` (about 3 total) were doing two jobs:
1. Traversing the filesystem whether through std::filesystem or FolderRep*
2. Processing the given folder structure to yield a specificied result

Confirm that `main.cpp` has been reorganized so that:
- A traversal function is provided for further operations that traverse the FolderRep* object so that new operations don't have to be dependent on FolderRep* logic

Focus on / Look for / Confirm that (something that was previously there was removed or replaced):
- Impact bullet #1
- Impact bullet #2
- Impact bullet #3