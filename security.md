# Security & Robustness Report

## Phase 14 — Security & Robustness

### Objective

Improve server reliability and security by handling malformed requests, oversized headers, invalid HTTP messages, slow clients, and directory traversal attacks.

---

## Security Features Implemented

### 1. Request Header Size Limit

To prevent excessive memory consumption, the server enforces a maximum request header size.

Configuration:

```cpp
const size_t MAX_HEADER_SIZE = 8192;
```

Behavior:

* Requests exceeding 8 KB of header data are rejected.
* Server returns:

```http
HTTP/1.1 431 Request Header Fields Too Large
```

Protection:

* Memory exhaustion attacks
* Oversized header attacks
* Malicious clients sending unbounded request data

---

### 2. Maximum Header Count Limit

The server limits the number of HTTP headers processed per request.

Configuration:

```cpp
const size_t MAX_HEADER_COUNT = 100;
```

Behavior:

* Requests containing more than 100 headers are rejected.
* Server returns:

```http
HTTP/1.1 431 Request Header Fields Too Large
```

Protection:

* Header flooding attacks
* Excessive parsing overhead
* Resource exhaustion

---

### 3. Request Line Validation

The request line is validated before processing.

Required format:

```http
METHOD PATH HTTP_VERSION
```

Examples of rejected requests:

```http
GET
```

```http
HELLO
```

```http
INVALID REQUEST
```

Response:

```http
HTTP/1.1 400 Bad Request
```

Protection:

* Malformed HTTP requests
* Invalid protocol messages

---

### 4. HTTP Version Validation

Supported versions:

```http
HTTP/1.0
HTTP/1.1
```

Unsupported versions are rejected.

Examples:

```http
HTTP/2.0
```

```http
HTTP/3.0
```

Response:

```http
HTTP/1.1 400 Bad Request
```

Protection:

* Invalid protocol versions
* Undefined server behavior

---

### 5. Header Format Validation

Each header must contain a valid colon separator.

Valid:

```http
Host: localhost
```

Invalid:

```http
Host localhost
```

```http
Connection keep-alive
```

Response:

```http
HTTP/1.1 400 Bad Request
```

Protection:

* Malformed HTTP packets
* Invalid header parsing

---

### 6. HTTP Method Validation

Supported methods:

```http
GET
POST
```

Unsupported methods:

```http
PUT
DELETE
PATCH
OPTIONS
TRACE
```

Response:

```http
HTTP/1.1 405 Method Not Allowed
```

Protection:

* Unexpected request handling
* Unsupported protocol operations

---

### 7. Request Timeout Handling

Socket receive timeouts are configured to prevent slow clients from holding server resources indefinitely.

Configuration:

```cpp
SO_RCVTIMEO
```

Behavior:

* Inactive connections are terminated.
* Server returns:

```http
HTTP/1.1 408 Request Timeout
```

Protection:

* Slowloris-style attacks
* Resource starvation
* Worker thread exhaustion

---

### 8. Directory Traversal Protection

The server validates requested paths before accessing the filesystem.

Blocked patterns:

```text
..
```

```text
\
```

Examples:

```http
GET /../../../etc/passwd
```

```http
GET /..\..\windows\system32
```

Response:

```http
HTTP/1.1 403 Forbidden
```

Protection:

* Directory traversal attacks
* Unauthorized file access
* Filesystem disclosure

---

### 9. Connection Error Handling

Server responses are transmitted through a dedicated response-sending mechanism with error checking.

Behavior:

* Failed socket writes are detected.
* Invalid or disconnected clients are handled gracefully.

Protection:

* Broken pipe errors
* Unexpected client disconnects
* Incomplete response delivery

---

## HTTP Error Responses Supported

| Status Code | Description                     |
| ----------- | ------------------------------- |
| 400         | Bad Request                     |
| 403         | Forbidden                       |
| 404         | Not Found                       |
| 405         | Method Not Allowed              |
| 408         | Request Timeout                 |
| 431         | Request Header Fields Too Large |

---

## Threat Model Coverage

| Threat                | Mitigation                |
| --------------------- | ------------------------- |
| Oversized Headers     | Header Size Limit         |
| Header Flooding       | Header Count Limit        |
| Malformed Requests    | Request Validation        |
| Invalid HTTP Versions | Version Validation        |
| Malformed Headers     | Header Format Validation  |
| Unsupported Methods   | Method Validation         |
| Slowloris Attacks     | Receive Timeouts          |
| Directory Traversal   | Path Sanitization         |
| Client Disconnects    | Connection Error Handling |

---

## Limitations

The server currently uses a thread-pool architecture where worker threads remain associated with active client connections.

At very high concurrency levels, persistent connections may still reduce scalability due to thread occupancy.

Future improvements may include:

* epoll-based event loops
* non-blocking sockets
* io_uring integration
* rate limiting
* TLS/HTTPS support
* request body size limits
* per-IP connection limits

---

## Conclusion

Phase 14 significantly improved the security and robustness of the server by validating incoming requests, enforcing resource limits, handling malformed packets, protecting against common attack vectors, and providing standards-compliant HTTP error responses.

The server now safely handles a wide range of invalid and malicious client behaviors while maintaining stable operation under normal workloads.
