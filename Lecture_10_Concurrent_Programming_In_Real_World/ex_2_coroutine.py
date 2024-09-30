import asyncio
import time

# Coroutine task that simulates an I/O-bound task
async def coroutine_worker(id):
    print(f"Coroutine {id} is running")
    await asyncio.sleep(1)  # Simulates a non-blocking I/O operation
    print(f"Coroutine {id} has finished")

async def main():
    tasks = []
    # Create 1000 coroutine tasks
    for i in range(1000):
        tasks.append(asyncio.create_task(coroutine_worker(i)))

    # Run all tasks concurrently
    await asyncio.gather(*tasks)

if __name__ == "__main__":
    start_time = time.time()
    asyncio.run(main())
    print(f"All coroutines finished in {time.time() - start_time} seconds")