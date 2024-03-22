import requests
import time
import json

board_ids = []

address_format = "http://192.168.1.%s:60000"

board_addresses = {}

def update_addresses():
    for i in range(20): # out of 255
        url = address_format % i + "/health/info"
        try:
            print(f"Trying url {url}")
            response = requests.get(url, timeout=1)
            if response.status_code == 200:
                board_info = json.loads(response.text)
                if "boardId" in board_info:
                    board_addresses[board_info["boardId"]] = address_format % i
                    print(f"Found board {board_info["boardId"]} at {address_format % i}")
            else:
                print(f"IP: {url} gave response {response.status_code}, {response.text}")
        except Exception as ex:
            print("Exception scanning board at ip ", ex)



# If a board has been offline for this many seconds, rescan for it
boards_rescan_interval = 2 * 60

if __name__ == "__main__":
    update_addresses()

    filename = str(time.time()).split('.')[0]
    
    while True:
        with open(f"DataServer/saved_logs/log{filename}.log", "a") as logfile:
            for board_address in board_addresses.values():
                url = f"{board_address}/health/info"
                try:
                    response = requests.get(url)
                except Exception as ex:
                    print("Exception scanning board at ip ", ex)
                    continue
                data = json.loads(response.text)
                timestamp = str(time.time()).split('.')[0]
                print(f"{timestamp} {str(data)}")
                logfile.write(f"{timestamp} {response.text.splitlines()[4:6]}\n")
        logfile.close()
        time.sleep(15)
