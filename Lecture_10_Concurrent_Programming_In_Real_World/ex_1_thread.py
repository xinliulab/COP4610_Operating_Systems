import threading
import time

# Thread worker that simulates an I/O-bound task
def thread_worker(id):
    print(f"Thread {id} is running")
    time.sleep(1)  # Simulates a blocking I/O operation
    print(f"Thread {id} has finished")

if __name__ == "__main__":
    threads = []
    start_time = time.time()

    # Create 1000 threads
    for i in range(1000):
        thread = threading.Thread(target=thread_worker, args=(i,))
        threads.append(thread)
        thread.start()

    # Wait for all threads to finish
    for thread in threads:
        thread.join()

    print(f"All threads finished in {time.time() - start_time} seconds")
