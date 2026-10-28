import asyncio
import struct


username_success_flag = 0


def ask_for_user(receipient):
    username = input("Enter your username: ")
    
    payload = (receipient+ ">" +username+ ">_>").encode("utf-8")
    header = struct.pack('!I', len(payload))

    return header+payload

async def receive_messages(reader):
    """Continuously listens for incoming data."""
    global username_success_flag
    try:
        while True:
            data = await reader.read(1024)
            if not data:
                print("Server closed the connection.")
                break
            recvd = data.decode().strip().split(">")
            for i in recvd: print(i)
            if recvd[2] == "Server" and recvd[0] == "_":
                if "username already exists" in recvd[1]:
                    username_success_flag = -1
                elif "username configured successfully" == recvd[1] or "its YOUR username" in recvd[1]:
                    username_success_flag= 1
                else:
                    pass

                
            print(f"\n[Received]: {recvd}")
    except asyncio.CancelledError:
        pass  # Clean exit when task is cancelled

async def send_messages(writer):
    """Periodically sends data or handles an outbound queue."""
    global username_success_flag
    
    receipient = "Server"
    username = ""

    setup_payload = ask_for_user(receipient=receipient)
    while(True):
        writer.write(setup_payload)
        await writer.drain()
        if username_success_flag == 1:
            username = setup_payload.decode().split(">")[1]
            print(f"your username: '{username}' saved successfully ")
            break
        await asyncio.sleep(0.5)
        if username_success_flag == -1:
            print("username already exists. Try something else")
            setup_payload = ask_for_user
            username_success_flag = 0

    

    try:
        counter = 0
        while True:
            inp =  await asyncio.to_thread(input, ":} ")

            if "@" in inp:
                receipient = inp[1:]
                continue

            payload = (receipient+ ">" +inp+ ">" +username+ ">").encode("utf-8")
            print 
    
            header = struct.pack('!I', len(payload))

            message = header + payload
            writer.write(message)
            await writer.drain()  # Ensures data is flushed to the network
            print(f"[Sent]: {message.strip()}")
            counter += 1
    except asyncio.CancelledError:
        pass

async def main():
    # Connect to the server
    server_ip = '127.0.0.1'
    server_port = 8080
    
    print(f"Connecting to {server_ip}:{server_port}...")
    reader, writer = await asyncio.open_connection(server_ip, server_port)
    print("Connected!")

    # Run both tasks concurrently on the single main thread
    # asyncio.gather schedules both loops to run cooperatively
    try:
        await asyncio.gather(
            receive_messages(reader),
            send_messages(writer)
        )
    except KeyboardInterrupt:
        print("Closing connection...")
    finally:
        writer.close()
        await writer.wait_closed()

# Start the event loop
if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\nProgram stopped.")
