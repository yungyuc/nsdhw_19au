import calc_angle

if __name__ == '__main__':
    # should be 1.5707963705062866
    print(calc_angle.calc_angle((1.0, 0.0), (0.0, 1.0)))
    # should be 0.0
    print(calc_angle.calc_angle((1.0, 1.0), (1.0, 1.0)))
