# FileSaver

## Overview
This project was developed as part of an operating systems course at University Politehnica of Timisoara. It focuses on system calls in C and is designed to process directories and identify potentially malicious files based on specific criteria.

## Functionality
- **Input:** The program takes any number of arguments, which must be directories. Invalid directories will not be processed. Paths to the directories can be either relative or full.
- **Processing:** For each valid directory provided as an argument, the program creates a new thread to process it.
  - Processing involves traversing the entire directory tree and taking a snapshot of the directory's contents, which is then saved at a specified path.
  - If a potentially malicious file is encountered during processing, additional a thread is created to analyze it further.
- **Malicious File Identification:**
  - A potentially malicious file is defined as:
    - Having no access rights.
    - Being less than 3 lines over 1000 words and characters exceeding 2000.
    - Containing specific words or non-ASCII characters.
  - Files meeting all these criteria are considered viruses and are moved to an isolated safe space directory.

## Verification
- A shell script is used for verifying potentially malicious files.
- The script returns "SAFE" if the file is deemed safe or the path of the virused file if it matches the defined rules for a virus.

## Usage
1. **Compilation:** Use `make` to compile the project. The Makefile provides the following targets:
   - `all`: Builds the entire project.
   - `clean`: Removes all compiled files and objects, cleaning the directory.
2. **Execution Flags:** The program `filesaver.efl` accepts the following flags (case insensitive):
   - `-o/-O`: Marks the cache directory where snapshots of tracked directories will be saved. If not provided, defaults to "FileSaverCache".
   - `-x/-X`: Marks the isolated space directory where malicious files will be moved. Must exist before running the program.
   - `-s/-S`: Specifies the path to the verification shell script for identifying malicious files. Defaults to "verify_for_malicious.sh".
   - `-d/-D`: Followed by a list of directories as arguments for processing.

3. **Usage Examples:**
   - Example 1: 
     ```
     ./filesaver.efl -o CacheDirectory -x IsolatedSpace -s verify_script.sh -d dir1 dir2
     ```
     This command processes `dir1` and `dir2`, saving snapshots in `CacheDirectory`, moving malicious files to `IsolatedSpace`, and using `verify_script.sh` for verification.
   - Example 2 (default settings):
     ```
     ./filesaver.efl -d dir3
     ```
     This command processes `dir3` using default settings for cache directory, isolated space, and verification script.

4. **Verification Script:**
   - The script `verify_for_malicious.sh` checks potentially malicious files.
   - **Usage:** `./verify_for_malicious.sh <malicious_file_path> <safe_directory_path>`
   - **Example:** `./verify_for_malicious.sh /path/to/malicious_file /path/to/safe_directory`

## Notes
- Ensure that the directories provided as arguments exist and are accessible.
- Customize the verification script or rules as needed for your specific use case.
