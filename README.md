# NOTE: this project is not yet finished, so the following is just a demonstration of concept, not production.

# Welcome to a done-for-AI clipboard program
This is a program for use between your project and your GPT
to provide structural evidence and insight into how your project
works and context for the LLM to improve upon your existing code.

## Industry-standard README
Ever observe a README that seems to have lack of context? This README is organized in industry-standard fashion
like a <a href="https://www.inflectra.com/Ideas/Topic/Requirements-Definition.aspx">Software Requirements Specification</a>.

## How It Works
An executable that converts your passed root directory to text and copies it to your clipboard.

## License
This project uses the [MIT license](./LICENSE). Because the solution is not intended to be a standalone product, I've chosen this license only to validate <a href="https://mikesinclair66.github.io">my work</a>.

# Build Features (Ctrl/Cmd + Click to open in a new tab)
- [CMake Integration]("./readme_subfolder/CMake.md")[^1]
- [CI integrations - GitHub Actions]("./readme_subfolder/CI-integrations.md")[^2]</a>
[^1]: Location: ["./readme_subfolder/CMake.md"]("./readme_subfolder/CMake.md")
[^2]: Location: ["./readme_subfolder/CI-integrations.md"]("./readme_subfolder/CI-integrations.md")

> [!IMPORTANT]
You will have to have <a href="https://cmake.org">CMake</a> installed to run the [Project Build Steps](#project-build-steps).

# Software Requirements Spec

## 1. Purpose

### 1.1 System Overview

**What the system is:**  
RepoContextClipboardForAI is a cross-platform C++ command-line executable that converts a specified root directory into a structured text representation and copies that output to the system clipboard.

**Core problem it solves:**  
Developers who want AI assistance must manually copy and paste multiple code snippets or explain project structure (“codesplaining”). This process is repetitive, incomplete, and inefficient.

**Primary outcome it delivers:**  
The system generates a comprehensive, readable textual representation of a repository and places it directly into the clipboard, enabling immediate pasting into AI tools for contextual understanding.

---

### 1.2 Success Criteria

Measurable indicators of success:

- The program successfully converts a given directory into text format.
- The full formatted output is copied to the system clipboard.
- A user can paste the clipboard contents directly into an AI interface without additional formatting.
- The tool reduces manual copy-paste effort to a single execution step.
- The project builds successfully via CMake on Windows using Visual Studio 2022.
- GitHub Actions confirms successful CI builds on Windows.

---

## 2. Users

### 2.1 User Roles

**Role: Developer (Primary User)**  
- Executes the program from the command line  
- Provides a root directory as input  
- Pastes generated output into AI tools  
- Uses the tool to enhance AI-assisted development  

There are no administrative or multi-user roles. The application is single-user, local-execution software.

---

## 3. Functional Requirements

### Core Execution

FR-1: Accepts a root directory via a `--root` command-line flag.
FR-2: Recursively traverses the provided directory and generates a structured textual representation of the directory contents.
FR-3: Outputs directory structure independently of file content representation.

[!NOTE] This project uses pattern support to indicate directories and files to include as a command
 - "*.yml,*.txt"
 - ".git/*,build/*"
 
FR-4: Enables pattern support for what textual structure and content to exclude or include via `--include` and `--exclude`.
FR-5: Enables pattern support like FR-4 but derived from inclusive and exclusive files (e.g. `--include-from .repoinclude` and `--exclude-from .repoignore`).
FR-6: Adds a `--use-repo-config` tag so that `--include-from` and `--exclude-from` tags don't need to be specified.

FR-7: Applies default exclusions for common build and version-control directories (e.g., `.git`, `build`).
FR-8: Limits content to a specified amount of lines based on the configurable `--max-lines` tag.


### Advanced Execution
FR-X: Completes execution interactively when invoked with an `--interactive` tag. What this means is that the initial output wil default to `--include` all supported file extensions (unless used in conjunction with `--use-repo-config`) and then the user will be able to toggle using `space` any files or directories that they want the output to include / disinclude. Once the user finalizes their configuration and presses `Enter`, The console will output the corresponding command follow either one of the following conventions:
1. If the user typed `--interactive` with `--use-repo-config`, the `.repoinclude` and `.repoignore` files will be updated and the output won't contain the `--include` and `--exclude` tags.
2. If the user typed `--interactive` without `--use-repo-config` and without `--include-from` and `exclude-from`, the `.repoinclude` and `.repoexclude` files won't be updated and the output will contain the `--include` and `--exclude` tags to apply to the new configuration.

### Build & Tooling

FR-9: Builds using CMake version 3.20 or higher.
FR-10: Supports C++17.
FR-11: Generates a Visual Studio 2022 solution via CMake.
FR-12: Supports out-of-source builds in a /build directory.
FR-13: Successfully builds inside of a Windows CI pipeline using GitHub Actions.  

---

## 4. Non-Functional Requirements

### 4.1 Performance

- The system shall process small-to-medium repositories (e.g., <10MB total size) in under 3 seconds on a standard developer machine.  
- Clipboard copying shall complete immediately after generation.  
- The system shall not require network connectivity.  

---

### 4.2 Security

- The system shall operate entirely locally.  
- The system shall not transmit repository data over a network.  
- The system shall not persist repository contents beyond runtime unless explicitly modified in future versions.  
- Sensitive repository contents are handled solely within user-controlled environments.  

---

### 4.3 Maintainability

- The system shall be buildable via CMake to maintain IDE independence.  
- The build process shall not depend on manually created Visual Studio project files.  
- The project shall support modular expansion for future enhancements (e.g., filtering, exclusion rules, formatting strategies).  
- The repository shall include CI validation via GitHub Actions.  
- The codebase shall remain small and comprehensible for iterative expansion.  

---

## 5. Data Model

> NOTE: This application does not use persistent storage. The data model represents transient runtime structures only.

### Entity: RepositoryContext
- rootPath (string)  
- generatedText (string)  
- totalFilesProcessed (integer)  
- generationTimestamp (timestamp)  

### Entity: FileEntry
- filePath (string)  
- relativePath (string)  
- fileContent (string)  
- fileSize (integer)  

---

## Architectural Notes

- Stateless  
- Single-executable  
- Local-only  
- Clipboard-output driven  
- Designed for AI-context augmentation  

# Who/What does this project exist to serve?

This project is for:
- Back and forth code reports between the coder and an LLM

# What problem does this project exist to solve?

Sometimes, a developer dislikes ‘vibe-coding’ but wants to continue coding in the traditional sense. Well this project helps you find out how reliant you can be on AI while still following the traditional pursuit.

# What solution does this project hope to achieve?

Providing readable, simple, and comprehensive text to the user’s clipboard so they can paste it all into ChatGPT.

# What solution is required to alleviate this project and competing subscriptions if necessary?

Copying and pasting code snippets of a given solution and a snippet for each of its dependencies.

# CMake integration
This project integrates with <a href="https://cmake.org">CMake</a> with the goal to:
- Enable cross-platform builds
- Eliminate reliance on manually creating Visual Studio projects
- Support clean out-of-source builds

## Why use CMake?
This project uses CMake as its build system.

CMake allows us to:
- Generate a Visual Studio .sln automatically
- Keep the project IDE-independent
- Easily expand the project later (libraries, multiple executables, testing, etc.)
- Avoid committing Visual Studio-specific files

Instead of manually managing .sln and .vcxproj files, CMake can generate them consistently in a /build folder.

## CMake configuration
`CMakeLists.txt`
```
cmake_minimum_required(VERSION 3.20)

project(dir2md VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(dir2md main.cpp)
```

What this does:
- `cmake_minimum_required(VERSION 3.20)` ensures a compatible CMake version
- `project(HelloWorld ...)` defines the solution name and language
- `set(CMAKE_CXX_STANDARD 17)` enforces C++17
- `add_executable(HelloWorld main.cpp)` creates an executable target named HelloWorld from main.cpp

When CMake runs, it generates:
- `dir2md.sln`
- Associated `.vcxproj` files

## Project Build Steps

## 1: Building a Visual Studio Project (2022)
From the project root:
```
cmake -S . -B build -G "Visual Studio 17 2022"
```

## 2: Building the compiled .exe
To build the program .exe, you can do either of the following:
### Building the solution through Visual Studio
1. Navigate to the build/ directory.
2. Open RepoContextClipboardForAi.sln.
3. **In Solution Explorer**, right-click RepoContextClipboardForAi.
4. Click "Set as Startup Project".
5. **In Solution Explorer**, right-click RepoContextClipboardForAi.
6. Click "Properties"
7. Click "Configuration Properties" <> "Debugging"
8. In the "Command Arguments" field, paste in the path to the root directory of RepoContextClipboardForAi.sln
- Make sure that the path is open-ended and doesn't contain a '\' at the end
    - i.e. 'C:\Users\micha\local-dev-workspace\repo-context-for-ai'
9. Press **Run** or **Local Windows Debugger**.

### Through the command line
After running the following command:
```
cmake --build build --config Debug
```
The executable will be output to:
```
build/Debug/RepoContextClipboardForAi.exe
```

## This project now works with GitHub Actions to ensure the CMake build commands work on a given local Windows machine.
You can check the CI script in `.github/workflows/windows-build.yml`.
