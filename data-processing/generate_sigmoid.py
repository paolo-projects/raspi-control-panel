import numpy as np
import sys
from decimal import Decimal

CCD_SIZE = 3694
CCD_MEANINGFUL_SIZE = 3694 - 32 - 14 - 1


# derive sigmoid from logistic curve
def logistic(l, k, x0): return lambda x: l/(1+np.exp(-k*(x-x0)))


def main():
    stdout = True
    out_file = ""

    if len(sys.argv) > 1:
        stdout = False
        out_file = sys.argv[1]
        res = input(f"Result will be saved in {out_file}. Do you want to continue? (y/N) ")
        if res.lower() != "y":
            exit(0)

    sigm_der = []
    function = logistic(1.0, 0.05, CCD_MEANINGFUL_SIZE/2)
    for i in range(1, CCD_MEANINGFUL_SIZE):
        sigm_der.append(function(i) - function(i-1))

    base = min(sigm_der)
    factor = max(sigm_der)

    for i in range(0, len(sigm_der)):
        sigm_der[i] = (sigm_der[i] - base) / (factor - base)

    stream = sys.stdout
    if stdout is not True:
        stream = open(out_file, "w")

    print("{", file=stream)
    for value in sigm_der:
        print("%.e," % Decimal(value), file=stream)

    print("}", file=stream)

    if stdout is not True:
        stream.close()


if __name__ == "__main__":
    main()