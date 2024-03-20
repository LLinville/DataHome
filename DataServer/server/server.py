import requests
import time
import json

board_ids = [2, 3]

address_format = "http://192.168.1.%s:60000"

board_addresses = {
    board_id: None
    for board_id in board_ids
}



def update_addresses():
    for i in range(255):
        url = address_format % i + "/health/info"
        try:
            response = requests.get(address_format % i)
            if response.status_code == 200:
                board_info = json.loads(response.json)
                if "boardId" in board_info:
                    board_addresses[board_info["boardId"]] = address_format % i
                    print(f"Found board {board_info["boardId"]} at {address_format % i}")
        except Exception as ex:
            print("Exception scanning board at ip ", ex)



# If a board has been offline for this many seconds, rescan for it
boards_rescan_interval = 2 * 60

if __name__ == "__main__":
    url = "http://192.168.1.2:60000/health/info"
    
    filename = str(time.time()).split('.')[0]
    
    while True:
        with open(f"DataServer/saved_logs/log{filename}.log", "a") as logfile:
            response = requests.get(url)
            data = json.loads(response.text)
            timestamp = str(time.time()).split('.')[0]
            print(f"{timestamp} {response.text.splitlines()[4:6]}")
            logfile.write(f"{timestamp} {response.text.splitlines()[4:6]}\n")
        logfile.close()
        time.sleep(30)
