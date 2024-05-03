# Webserv_42 - HTTP Server Project

![Language](https://img.shields.io/badge/language-C++98-blue.svg)
![Platform](https://img.shields.io/badge/platform-MacOS-lightgrey.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## Introduction

`webserv` is a custom HTTP server written in C++98 as part of the 42 school curriculum. This project aims to deepen knowledge of HTTP protocol and server-side software architecture, focusing on non-blocking I/O handling using system-level multiplexing.

## Features

- **HTTP 1.1 Compliance**: Handles standard HTTP methods such as GET, POST, and DELETE.
- **Non-blocking I/O**: Utilizes `pol()` for efficient multiplexing.
- **File Upload**: Supports file uploads within multipart/form-data requests.
- **Configuration**: Uses custom configuration files similar to NGINX for server settings.
- **Error Handling**: Custom error pages for different HTTP status codes.

## Getting Started

### Prerequisites

- MacOS (as per project specifications)
- C++98 compiler (e.g., clang or g++)

### Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/yourusername/webserv.git
   
### Running the Server
2. To start the server with the default configuration:

   ```bash
   ./webserv config_files/default.conf

### Testing
The server is tested extensively using:

curl for manual HTTP request testing.
siege for stress testing and performance analysis.
telnet to manually craft HTTP requests and examine raw responses.

### Contributors
fbardeau
