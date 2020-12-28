import struct
import serial
from sys import stderr
import signal
from typing import Tuple
import math


# This script is supposed to work with a modified version of the firmware by Esben Rossel
# The modifications are made to take advantage of the internal FPU and the ARM math lib to do the needed data
# processing and output a single value, that should be the shadow edge position in pixels
#
# The modified firmware will output only 8 bytes of data, of which the first two are control characters
# 'PI', then there's a 32-bit integer with the pixel value, and the last 2 are a 16-bit unsigned
# short with a checksum
#
# A weird behavior of my CCD is that when the light strikes the sensor on the right side (connectors at the top)
# the readings are bad and out of scale. When light strikes the left side of the sensor, and the shadow is
# on the right side, instead, the readings are in range and the output curve is nice
# I don't know if it's a characteristic of this particular sensor or I have a faulty one.
# Anyway this behavior is visible in the sample datasets included in this folder. (You can view them through the
# "display_datasets.py" script. You can see that when the shape of the curve is an increasing sigmoid, the curve
# shape is nice, while when the sigmoid is reversed the shape is somehow "cut"


SH = 200
ICG = 100000
averaging = True
AVGS_NUM = 5

continuous = True
intheloop = False


def tx_data(sh: int, icg: int, integrations: int, single: bool = True) -> bytes:
    return struct.pack('>BBIIBB', 0x45, 0x52, sh, icg, 0x00 if single is True else 0x01, integrations)


def rx_parse_data(data: bytes) -> int:
    (cc, res, cs) = struct.unpack("<2siH", data)
    if str(cc, encoding='ascii') != "PI":
        raise Exception("Invalid data received. Control characters do not match: "+str(cc))
    checksum = 0
    checksum += int(cc[0]) + int(cc[1])
    barr = struct.pack("i", res)
    checksum += int(barr[0]) + int(barr[1]) + int(barr[2]) + int(barr[3])
    if checksum != cs:
        raise Exception("Invalid checksum")
    return res


def request_data() -> int:
    try:
        with serial.Serial('COM8', baudrate=115200, timeout=3) as ccd:
            tx_dt = tx_data(sh=SH, icg=ICG, integrations=1, single=True)
            wrt_count = ccd.write(tx_dt)
            if not (wrt_count and wrt_count == len(tx_dt)):
                raise ConnectionError(f"Couldn't write TX data. {wrt_count} bytes written")

            recv = ccd.read(8)

        rx_data = rx_parse_data(recv)
        return rx_data
    except Exception as e:
        print("An error occurred", file=stderr)
        print(e, file=stderr)


def request_avgs() -> Tuple[float,float]:
    averages = []
    for i in range(0, AVGS_NUM):
        averages.append(request_data())
    average_res = 0.0
    for avg_measure in averages:
        average_res += avg_measure
    average_res /= AVGS_NUM

    variance = 0.0
    for i in range(0, AVGS_NUM):
        variance += (averages[i] - average_res)**2
    variance /= (AVGS_NUM-1)

    return average_res, math.sqrt(variance)/math.sqrt(AVGS_NUM)


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


def update_averaging():
    global averaging, AVGS_NUM
    newavg = input(f"Enable averaging? (Current: {'yes' if averaging is True else 'no'}) (y/n): ")
    if newavg == "y":
        averaging = True
    elif newavg == "n":
        averaging = False
    else:
        print("Input not recognized")

    newavgnum = input(f"How many averages (each one takes ~1.5 sec)? ({AVGS_NUM}): ")
    if newavgnum.isnumeric():
        AVGS_NUM = int(newavgnum)
    else:
        print("Number not recognized")


def show_help():
    print(f"Current values: SH={str(SH)} ICG={str(ICG)} Averaging={'yes' if averaging is True else 'no'} Averages={str(AVGS_NUM)}")
    print("")
    print("Available commands:")
    print("\tr -> read value (default)")
    print("\tc -> continuous reading")
    print("\ts -> set sh, icg")
    print("\ta -> set averaging")
    print("\th -> print this help")
    print("\tq -> quit")
    print("")


def main():
    signal.signal(signal.SIGINT, termination)
    signal.signal(signal.SIGTERM, termination)

    print()
    try:
        show_help()
        while 1:
            command = input("Enter the action: ")
            if len(command) == 0 or command == "r":
                if averaging is True:
                    avgs, err = request_avgs()
                    print("Shadow edge: %.1f +- %.1f px" % (avgs, err))
                else:
                    res = request_data()
                    print(f"Shadow edge: {str(res)}px")
            elif command == "h":
                show_help()
            elif command == "c":
                global continuous, intheloop
                continuous = True
                intheloop = True

                while continuous:
                    res = request_data()
                    print(f"Shadow edge: {str(res)}px")

                print("Readings stopped")
                intheloop = False
            elif command == "s":
                update_params()
            elif command == "a":
                update_averaging()
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