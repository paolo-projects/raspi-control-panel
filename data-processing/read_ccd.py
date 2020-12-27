from ccd import request_data
import argparse
from sys import stdout

SH = 500
ICG = 100000


parser = argparse.ArgumentParser(description="Read CCD values into a CSV file or print them to screen")
parser.add_argument('-s', '--sh', type=int, help="The SH period. Default is 500")
parser.add_argument('-i', '--icg', type=int, help="The ICG period. Default is 100000")
parser.add_argument('-o', '--output', type=str, help="The output CSV file")
parser.add_argument('-t', '--dev', type=str, help="The tty device or COM port", required=True)


def main():
    parsed = parser.parse_args()
    sh = 500
    icg = 100000
    if parsed.sh: sh = parsed.sh
    if parsed.icg: icg = parsed.icg

    data = request_data(parsed.dev, sh, icg)

    output_stream = stdout
    if parsed.output: output_stream = open(parsed.output, "w")

    # Discard meaningless values
    data = data[33:-14]

    # Reverse the scale
    for i in range(0, len(data)):
        data[i] = 4095 - data[i]

    # Scale the values to fit in the 0-1 range
    maxval = max(data)
    for i in range(0, len(data)):
        data[i] /= maxval

    print("Pixel,Intensity", file=output_stream)
    for i in range(0, len(data)):
        print("%d,%e" % (i, data[i]), file=output_stream)

    if parsed.output: output_stream.close()


if __name__ == "__main__":
    main()