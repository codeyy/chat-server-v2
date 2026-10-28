import sys
import struct
import asyncio
from prompt_toolkit import PromptSession
from prompt_toolkit.patch_stdout import patch_stdout


username = "_"

async def receive_messages(reader: asyncio.StreamReader):
    """
    Listens for data from the server.
    Because of patch_stdout(), standard print() statements here 
    will cleanly push the user's active typing line down.
    """
    try:
        while True:
            data = await reader.read(1024)
            if not data:
                print("\n[System] Connection closed by the server.")
                break
            
            message = data.decode('utf-8').strip().split(">")

            if message[0] == username:
                print(f"\n[from {message[2]}]  {message[1]}")
            
    except asyncio.CancelledError:
        pass
    except Exception as e:
        print(f"\n[Error] Connection issue: {e}")
        sys.exit()



async def send_messages(writer: asyncio.StreamWriter):
    """
    Handles the asynchronous prompt loop for user input.
    """
    session = PromptSession()

    receipient = ""
    print(username)
    with patch_stdout():
        while True:
            try:
                inp =  await session.prompt_async("{: ")

                cleaned_input = inp.strip()

                if "@" in cleaned_input:
                    receipient = cleaned_input[1:]
                    print("receipient is "+receipient)
                    continue
                elif receipient == "": 
                    print("please specify a receipient with @ symbol folowed by the receipient's username and enter.")
                    continue

                payload = (receipient+ ">" +cleaned_input+ ">" +username+ ">").encode("utf-8")
                

                header = struct.pack('!I', len(payload))

                message = header + payload
                writer.write(message)
                await writer.drain()  # Ensures data is flushed to the network
                print(f"[Sent]: {message.strip()}")
            except asyncio.CancelledError:
                pass


async def set_username(writer, reader):
    global username

    while True:
        inp = input("Enter Desired Username: ")

        payload = ("Server>" +inp+ ">_>").encode("utf-8")

        header = struct.pack('!I', len(payload))

        writer.write(header+payload)
        await writer.drain()

        data = await reader.read(1024)
        recvd = data.decode().strip().split(">")
        
        if "username configured successfully" == recvd[1] or "its YOUR username" in recvd[1]:
            username = recvd[0]
            print(f"username set to: {username}")
            return username
        else:
            print(recvd[1]+ "\nretry:\n")
            continue






async def main():
    # Connect to the server
    server_ip = '127.0.0.1'
    server_port = 8080
    
    print(f"Connecting to {server_ip}:{server_port}...")
    reader, writer = await asyncio.open_connection(server_ip, server_port)
    print("Connected!")

    # Run both tasks concurrently on the single main thread
    # asyncio.gather schedules both loops to run cooperatively
    rs = await set_username(writer, reader)

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


