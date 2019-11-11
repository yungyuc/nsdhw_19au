import time
import _matrix


def bench(multiply, ma, mb):
    start = time.time()
    multiply(ma, mb)
    end = time.time()
    return end - start


def main():
    ma = _matrix.Matrix(1200, 1200)
    for i in range(1200):
        for j in range(1200):
            ma[i, j] = i + j - 1

    mb = _matrix.Matrix(1200, 1200)
    for i in range(1200):
        for j in range(1200):
            mb[i, j] = i - j + 1

    tm = bench(_matrix.multiply_mkl, ma, mb)
    print('multiply_mkl', tm)
    tn = bench(_matrix.multiply_naive, ma, mb)
    print('multiply_naive', tn)


if __name__ == '__main__':
    main()
