# TCP Server Benchmark Report

## Environment

Workers: 2

Port: 8081

## Test 1

Requests: 100

Concurrent Clients: 10

Requests/sec: 1101.36

P50 Latency: 6 ms

P95 Latency: 20 ms

P99 Latency: 25 ms

Failed Requests: 0

## Test 2

Requests: 5000

Concurrent Clients: 100

Requests/sec: 2676.36

P50 Latency: 34 ms

P95 Latency: 39 ms

P99 Latency: 73 ms

Failed Requests: 0

## Test 3

Requests: 10000

Concurrent Clients: 500

Requests/sec: 2551.49

P50 Latency: 197 ms

P95 Latency: 213 ms

P99 Latency: 219 ms

Failed Requests: 0

Notes:
Increasing the listen backlog from 5 to 1024 eliminated connection timeouts under high concurrency.

# Test 4

Requests: 20000

Concurrent Clients: 1000

Requests/sec: 2495.80

P50 Latency: 397 ms

P95 Latency: 442 ms

P99 Latency: 454 ms

Failed Requests: 0

---

# Conclusions

The server sustained approximately 2500-2700 requests/second under heavy concurrent load.

Latency increased significantly as concurrency increased, indicating queueing effects within the thread pool architecture.

A major bottleneck was identified in the TCP listen backlog. Increasing the backlog from 5 to 1024 eliminated connection timeouts at 500 concurrent clients.

The server successfully handled 20,000 requests at 1000 concurrent clients with zero failed requests.
