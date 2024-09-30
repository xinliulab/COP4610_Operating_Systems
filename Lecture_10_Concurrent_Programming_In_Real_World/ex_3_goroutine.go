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
