/*
 * How to install Go and run this program:
 *
 * 1. Install Go:
 *    - On Linux:
 *      sudo apt update
 *      sudo apt install golang
 *    - On macOS:
 *      brew install go
 *    - On Windows:
 *      Download and install Go from the official website: https://golang.org/dl/
 *
 * 2. Save this code in a file named 'ex_3_goroutine.go'.
 *
 * 3. Open a terminal and navigate to the directory where the file is saved.
 *
 * 4. Compile and run the Go program:
 *    - To compile:
 *      go build ex_3_goroutine.go
 *    - To run the compiled program:
 *      ./ex_3_goroutine
 *
 * 5. Alternatively, you can directly run the Go program without compiling:
 *    go runex_3_goroutine.go
 *
 * 6. During the execution, you can open another terminal and use 'htop' or 'top' to monitor CPU and memory usage
 *    to see how goroutines handle concurrency efficiently.
 *
 * 7. The program will print the start and finish of each goroutine, and the total time taken for all goroutines to complete.
 */



package main

import (
    "fmt"
    "sync"
    "time"
)

const numGoroutines = 1000 // Number of Goroutines to create

// Goroutine worker that simulates an I/O-bound task
func worker(id int, wg *sync.WaitGroup) {
    defer wg.Done() // Signal that this Goroutine is done
    fmt.Printf("Goroutine %d is running\n", id)
    time.Sleep(1 * time.Second) // Simulate I/O operation by sleeping
    fmt.Printf("Goroutine %d has finished\n", id)
}

func main() {
    var wg sync.WaitGroup
    wg.Add(numGoroutines) // Set the number of Goroutines to wait for

    start := time.Now()

    // Create and start 1000 Goroutines
    for i := 0; i < numGoroutines; i++ {
        go worker(i, &wg) // Start each Goroutine
    }

    // Wait for all Goroutines to finish
    wg.Wait()

    fmt.Printf("All Goroutines finished in %v seconds\n", time.Since(start).Seconds())
}
