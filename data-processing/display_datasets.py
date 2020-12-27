from dsp import show_dsp


# Conclusions
####################
# After testing SavGol filter and the moving average one, the moving average is the least appropriate one
# because it moves the peak position along the X axis, a behavior that has to be absolutely avoided
# The savitzky-golay one does not change the peak shape or position, although it's not that good
# at reducing noise
#
# The interesting observation is that after the convolution (or cross-correlation, which in this case
# yields the same result) is executed the result is the same whether the savgol filter is applied or not.
# This is most likely due to the mathematical properties of the filter and the convolution operation
# And even applying the FIR filter the only observed change in the convolution is the peak being moved
# (which is, again, bad)
#
# Although it's been tested with few datasets, the initial conclusions are that the only operations worth applying
# are the initial data-cleaning process (to remove meaningless points) and the convolution/correlation between
# the first derivatives of a well made logistic curve and the dataset, which will yield a nice peak
# at the max-correlation point
#
# To recap, here's the procedure I came up with
# 1 -> Remove meaningless data points
# 2 -> Scale the Y values to a 0-1 range
# 3 -> Compute the first derivative
# 4 -> Negate the first derivative if needed (we need a positive peak shape)
# 5 -> Compute the convolution/correlation between the derivative of the dataset, and the derivative of
#      a logistic curve
# 6 -> Pick the X position of the maximum in the convolution/correlation curve
#####################
# Paolo Infante
#####################


def main():
    show_dsp()


if __name__ == "__main__":
    main()
