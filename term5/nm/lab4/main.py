import math
import numpy as np
import matplotlib.pyplot as plt

from sklearn.metrics import mean_squared_error

import interpolation_lagrange_polynomail
import interpolation_newton_polynomial


def f(x):
    return math.sin(x ** 2) * math.tan(x)


def chebyshev_polynomial_zero(k, n, a, b):
    return ((a + b) / 2) + ((b - a) / 2) * math.cos(((2 * k + 1) * math.pi) / (2 * n))


def print_measurements(measurements):
    for i in range(len(measurements)):
        print(f'{i}:\t{measurements[i]:.20f}')


def main():
    node_count = 9
    a = 2
    b = 10

    nodes = []

    # Init nodes
    for i in range(node_count):
        nodes.append(chebyshev_polynomial_zero(i, node_count, a, b))

    # Find interpolation polynomials
    lagrange_form = interpolation_lagrange_polynomail.find_lagrange_form(nodes, f)
    newton_form = interpolation_newton_polynomial.find_newton_form(nodes, f)

    # Init measurement
    f_measurements = np.empty([1, node_count]).flatten()
    lagrange_form_measurements = np.empty([1, node_count]).flatten()
    newton_form_measurements = np.empty([1, node_count]).flatten()

    for i in range(node_count):
        f_measurements[i] = f(nodes[i])
        lagrange_form_measurements[i] = lagrange_form(nodes[i])
        newton_form_measurements[i] = newton_form(nodes[i])

        print(
            f'{i}:\t{f_measurements[i]:.30f}\t'
            f'{lagrange_form_measurements[i]:.30f}\t'
            f'{newton_form_measurements[i]:.30f}'
        )

    print('--------------------')

    # Find mean squared error
    lagrangian_form_mse = mean_squared_error(f_measurements, lagrange_form_measurements)
    newton_form_mse = mean_squared_error(f_measurements, newton_form_measurements)

    print(f'lagrange mse: {lagrangian_form_mse}')
    print(f'newton mse: {newton_form_mse}')

    # Draw a chart
    figure, axis = plt.subplots(3)

    figure.tight_layout(pad=1.5)
    figure.set_size_inches(10.5, 10.5, forward=True)
    figure.set_dpi(100)

    axis[0].set_title('f(x) = math.sin(x ** 2) * math.tan(x)')
    axis[0].plot(nodes, f_measurements, 'o-', linewidth=2)

    axis[1].set_title('Lagrange form')
    axis[1].plot(nodes, lagrange_form_measurements, 'go-', linewidth=2)

    axis[2].set_title('Newton form')
    axis[2].plot(nodes, newton_form_measurements, 'co-', linewidth=2)

    plt.show()


if __name__ == '__main__':
    main()
