# Concurrent HTTP Server — Engineering Report

## Overview

This project implements a multi-threaded HTTP server in C++ using POSIX sockets, a thread pool, request routing, static file serving, connection persistence, and security hardening.

The objective was to progressively evolve a simple socket server into a production-inspired HTTP server architecture while measuring performance and improving robustness at each stage.

---

# System Architecture

The server follows a producer-consumer architecture.

```text
Client
  │
  ▼
Acceptor Thread
  │
  ▼
Task Queue
  │
  ▼
Worker Thread Pool
  │
  ▼
HTTP Request Parser
  │
  ▼
Router / Static File Handler
  │
  ▼
HTTP Response Generator
  │
  ▼
Client
```

---

## Request Lifecycle

### 1. Connection Acceptance

The main thread listens on a TCP socket and accepts incoming client connections.

```cpp
accept()
```

Accepted client sockets are pushed into a shared task queue.

---

### 2. Task Queue

Incoming client sockets are stored in a thread-safe queue protected by:

* mutex
* condition variable

This decouples connection acceptance from request processing.

---

### 3. Worker Pool

A fixed-size worker pool continuously waits for new tasks.

Each worker:

1. Removes a socket from the queue.
2. Processes the request.
3. Generates a response.
4. Supports persistent connections when Keep-Alive is enabled.

Benefits:

* Reduced thread creation overhead
* Better scalability
* Controlled resource usage

---

### 4. Request Processing

The server parses:

```http
METHOD PATH HTTP_VERSION
```

and extracts HTTP headers.

Supported methods:

```text
GET
POST
```

Supported versions:

```text
HTTP/1.0
HTTP/1.1
```

---

### 5. Routing

Requests are dispatched through a lightweight router.

Examples:

```cpp
router.get("/")
router.get("/about")
router.post("/api/data")
```

Static files are served from the configured document root.

---

### 6. Response Generation

Responses include:

* Status Line
* Content-Type
* Content-Length
* Connection Header

Example:

```http
HTTP/1.1 200 OK
Content-Type: text/plain
Content-Length: 9
```

---

# Performance Evaluation

## Benchmark Environment

Tool:

```bash
ApacheBench (ab)
```

Endpoint:

```http
GET /
```

Response Size:

```text
9 bytes
```

Worker Threads:

```text
100
```

---

## Throughput and Latency

### Concurrency = 10

Command:

```bash
ab -n 10000 -c 10 http://127.0.0.1:8081/
```

| Metric          | Value |
| --------------- | ----- |
| Requests/sec    | 3013  |
| P95 Latency     | 6 ms  |
| Failed Requests | 0     |

---

### Concurrency = 100

Command:

```bash
ab -n 10000 -c 100 http://127.0.0.1:8081/
```

| Metric          | Value |
| --------------- | ----- |
| Requests/sec    | 2665  |
| P95 Latency     | 84 ms |
| Failed Requests | 0     |

---

### Concurrency = 500

Command:

```bash
ab -n 10000 -c 500 http://127.0.0.1:8081/
```

| Metric          | Value  |
| --------------- | ------ |
| Requests/sec    | 2859   |
| P95 Latency     | 243 ms |
| Failed Requests | 0      |

---

## Performance Summary

| Clients | Req/s | P95 Latency |
| ------- | ----: | ----------: |
| 10      |  3013 |        6 ms |
| 100     |  2665 |       84 ms |
| 500     |  2859 |      243 ms |

### Observations

* Throughput remains relatively stable under increasing load.
* Latency increases significantly as concurrency grows.
* The thread pool prevents uncontrolled thread creation.
* Queueing delays become dominant at higher concurrency levels.

---

# Optimization History

## Baseline

Initial implementation:

* Single-threaded
* One request per connection
* No routing
* No persistence

Characteristics:

* Low throughput
* High connection overhead
* Limited scalability

---

## Thread Pool Introduction

Added:

* Shared task queue
* Worker pool
* Condition variables

Impact:

* Reduced thread creation overhead
* Improved throughput
* Better CPU utilization

---

## Router Integration

Added:

* Route registration
* Dynamic endpoint handling

Impact:

* Cleaner architecture
* Easier extensibility

---

## Keep-Alive Support

Added:

```http
Connection: keep-alive
```

Benchmark Results:

| Metric       | Without Keep-Alive | With Keep-Alive |
| ------------ | -----------------: | --------------: |
| Requests/sec |               1871 |            1930 |
| Mean Latency |           53.45 ms |        51.80 ms |

Impact:

* Reduced TCP connection overhead
* Improved throughput by approximately 3%

---

## Security Hardening

Added:

* Header size limits
* Header count limits
* Request validation
* Timeout handling
* Directory traversal protection
* Malformed request detection

Impact:

* Improved robustness
* Better resilience against malformed and malicious clients

---

# Limitations

Current architecture uses a thread-per-active-connection model.

At very high concurrency levels:

* Worker threads become occupied by active clients.
* Latency increases.
* Keep-Alive scalability is limited.

Production-grade systems typically use:

* epoll
* kqueue
* io_uring
* event-driven architectures

to support significantly larger connection counts.

---

# Future Improvements

Potential next steps:

1. Non-blocking sockets
2. epoll-based event loop
3. TLS/HTTPS support
4. HTTP request body parsing
5. Rate limiting
6. Access logging
7. Connection pooling
8. Zero-copy file transfers using sendfile()

---

# Conclusion

The project evolved from a basic socket server into a robust multi-threaded HTTP server supporting:

* Thread pools
* Routing
* Static file serving
* Keep-Alive connections
* Security hardening
* Performance monitoring

Benchmark results demonstrate stable throughput under concurrent workloads and measurable gains from connection persistence while maintaining correct HTTP behavior and improved security.

