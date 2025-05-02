# VINAc Archiver

## Overview
VINAc is a command-line archiver program that supports compression using the LZ77 algorithm. It allows users to create, manage, and extract archives containing multiple files (members). The program is designed to efficiently handle file storage while providing options for both compressed and uncompressed data.

## Features
- **Add Members**: Insert files into the archive with options for compression or without.
- **Extract Members**: Retrieve files from the archive, either individually or all at once.
- **Remove Members**: Delete specific files from the archive.
- **List Members**: Display the contents of the archive along with metadata such as size and modification date.

## File Structure
The project is organized into the following directories and files:

```
vinac
├── src
│   ├── main.c          # Entry point of the application
│   ├── archiver.c      # Implementation of archiving functions
│   ├── archiver.h      # Header for archiving functions
│   ├── directory.c      # Directory management functions
│   ├── directory.h      # Header for directory functions
│   ├── member.c        # Member-related functions
│   ├── member.h        # Header for member functions
│   └── lz
│       ├── lz.c        # LZ77 compression implementation
│       └── lz.h        # Header for LZ77 functions
├── include
│   └── vinac.h         # Main interface for the VINAc project
├── test
│   ├── test_archiver.c # Unit tests for archiver functions
│   ├── test_directory.c # Unit tests for directory functions
│   └── test_member.c   # Unit tests for member functions
├── Makefile             # Build instructions
├── README.md            # Project documentation
└── LEIAME.md            # Additional project information
```

## Installation
To build the VINAc project, ensure you have a C compiler installed. Clone the repository and navigate to the project directory. Run the following command to compile the project:

```
make
```

This will generate the executable named `vina`.

## Usage
The VINAc program can be executed with the following command structure:

```
./vina <option> <archive> [member1 member2 ...]
```

### Options
- **-ip (or -p)**: Insert one or more members without compression.
- **-ic (or -i)**: Insert one or more members with compression.
- **-m member**: Move a specified member to a new position in the archive.
- **-x**: Extract specified members from the archive.
- **-r**: Remove specified members from the archive.
- **-c**: List the contents of the archive with member properties.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.

## Acknowledgments
This project utilizes the LZ77 compression algorithm for efficient data storage. Special thanks to the contributors and resources that made this project possible.