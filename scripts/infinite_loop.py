# infinite_loop.py

import time

def main():
    while True:
        print("Questo è un loop infinito. Premi Ctrl+C per interrompere.")
        time.sleep(1)  # Dorme per 1 secondo per evitare di sovraccaricare la CPU

if __name__ == "__main__":
    main()
