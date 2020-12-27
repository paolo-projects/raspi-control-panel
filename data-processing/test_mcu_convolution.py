import struct
import serial
from sys import stderr
import signal
import matplotlib.pyplot as plt
from typing import List


# This script is supposed to work with a modified version of the firmware by
# Esben Rossel
# The modifications are made to take advantage of the internal FPU and the ARM
# math lib to do the needed data
# processing and output the convolution operation performed directly by the MCU
# The convolution is calculated against a pre-made sigmoid first derivative
# curve
CCD_SIZE = 3694
CCD_MEANINGFUL_SIZE = CCD_SIZE - 33 - 14
CONVOL_SIZE = ((CCD_MEANINGFUL_SIZE - 1) * 2 - 1) * 4


SH = 500
ICG = 100000

continuous = True
intheloop = False


def tx_data(sh: int, icg: int, integrations: int, single: bool=True) -> bytes:
    return struct.pack('>BBIIBB', 0x45, 0x52, sh, icg, 0x00 if single is True else 0x01, integrations)


def rx_parse_data(data: bytes) -> List[float]:
    result = []
    # Data ranges between 0-1, it is sent as 32-bit unsigned int sith a scale
    # factor of 10^7
    for value in struct.iter_unpack("<I", data):
        result.append(value[0] / 10e7)

    return result


def request_data():
    with serial.Serial('COM8', baudrate=115200, timeout=3) as ccd:
        tx_dt = tx_data(sh=1000, icg=200000, integrations=4, single=True)
        wrt_count = ccd.write(tx_dt)
        if not (wrt_count and wrt_count == len(tx_dt)):
            raise ConnectionError(f"Couldn't write TX data. {wrt_count} bytes written")

        recv = bytearray()
        tries = 0
        while len(recv) < CONVOL_SIZE:
            buffer = ccd.read(1024)
            if len(buffer) == 0:
                tries += 1
            else:
                recv.extend(buffer)

            if tries > 3:
                raise Exception("Error. Couldn't read all the bytes")

    rx_data = rx_parse_data(recv)
    return rx_data


def update_params():
    global SH, ICG
    newsh = input(f"Enter the new SH value ({str(SH)}): ")
    if newsh.isnumeric():
        SH = int(newsh)
    else:
        print("Number not recognized")

    newicg = input(f"Enter the new ICG value ({str(ICG)}): ")
    if newicg.isnumeric():
        ICG = int(newicg)
    else:
        print("Number not recognized")


def show_help():
    print("Available commands:")
    print("\tr -> read value (default)")
    print("\tc -> continuous reading")
    print("\ts -> set sh, icg")
    print("\th -> print this help")
    print("\tq -> quit")
    print("")


def main():
    signal.signal(signal.SIGINT, termination)
    signal.signal(signal.SIGTERM, termination)

    print(f"Defaults: SH={str(SH)} ICG={str(ICG)}")
    print()
    try:
        show_help()
        while 1:
            command = input("Enter the action: ")
            if len(command) == 0 or command == "r":
                try:
                    data = request_data()
                    plt.plot(data)
                    plt.show()

                except Exception as e:
                    print("An error occurred", file=stderr)
                    print(e, file=stderr)
            elif command == "h":
                show_help()
            elif command == "c":
                global continuous, intheloop
                continuous = True
                intheloop = True

                while continuous:
                    request_data()

                print("Readings stopped")
                intheloop = False
            elif command == "s":
                update_params()
            elif command == "q":
                exit(0)
            else:
                print("Command not supported")

            print()



    except Exception as e:
        print("An error occurred", file=stderr)
        print(e, file=stderr)
        raise e


def termination(_, __):
    if intheloop is True:
        global continuous
        continuous = False
    else:
        exit(0)


if __name__ == "__main__":
    main()