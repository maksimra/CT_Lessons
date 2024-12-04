import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Данные из таблицы
x_data = np.array([
    4.5221e-6, 2.2818e-6, 1.3458e-6, 0.8866e-6, 0.6279e-6, 0.4678e-6, 0.3620e-6
])
y_data = np.array([
    4.6830, 2.2971, 1.334, 1.0276, 0.6504, 0.4643, 0.4064
]) * 1.5  # Увеличение всех значений y в 1.5 раза
x_err = np.array([0.0016e-6, 0.0009e-6, 0.0005e-6, 0.0004e-6, 0.0003e-6, 0.0002e-6, 0.0001e-6])
y_err = np.array([0.0012, 0.0007, 0.0004, 0.0003, 0.0002, 0.0002, 0.0001]) * 1.5  # Пропорционально увеличим ошибки

# Линейная функция для аппроксимации
def linear(x, a, b):
    return a * x + b

# Аппроксимация данных
params, covariance = curve_fit(linear, x_data, y_data, sigma=y_err)
slope, intercept = params
slope_err = np.sqrt(covariance[0, 0])

# Построение графика
plt.figure(figsize=(8, 6))
plt.errorbar(x_data, y_data, xerr=x_err, yerr=y_err, fmt='o', label='Данные', capsize=5)
x_fit = np.linspace(min(x_data), max(x_data), 500)
y_fit = linear(x_fit, slope, intercept)
plt.plot(x_fit, y_fit, 'r-', label=f'Аппроксимация: y = {slope:.3e}x + {intercept:.3f}\nУгловой коэффициент: {slope:.3e} ± {slope_err:.3e}')

# Оформление графика
plt.xlabel(r'$1 / R_{\Sigma}^2$, $10^{-6}$', fontsize=12)
plt.ylabel(r'$1 / \Theta^2$', fontsize=12)
# plt.title('График зависимости $1 / \Theta^2$ от $1 / R_{\Sigma}^2$', fontsize=14)
plt.legend()
plt.grid()
plt.tight_layout()
plt.show()
