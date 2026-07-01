# 🧮 Advanced Scientific Calculator (C++)

An advanced scientific calculator program written in C++ that runs in the Command Line. It features a professional, colorful user interface (ANSI Colors) and comprehensive functionalities, including complex mathematical operations and extensive unit conversions.

## ✨ Key Features

*   **Professional UI:** Harmonious colors, ASCII art, and clear menu categorization.
*   **Calculation History:** Automatically saves all calculations and results, which can be viewed at any time.
*   **Operation Repetition:** Allows you to repeat the same mathematical operation with different numbers without returning to the main menu.
*   **Error Handling:** Robust protection against common errors (e.g., division by zero, invalid string input, square roots of negative numbers).
*   **Mathematical Constants:** Quick access to essential constants (π, e, φ, √2, √3, ln(2), ln(10)).

---

## 🛠️ Supported Mathematical Operations

Operations are organized into several categories for easy access:

### 1. Basic Arithmetic
*   Addition `(+)`, Subtraction `(-)`, Multiplication `(*)`, Division `(/)`
*   Modulus / Remainder `(%)`
*   Power `(x^y)`

### 2. Roots & Logarithms
*   Square Root `(sqrt)`, Cube Root `(cbrt)`, Nth Root `(nrt)`
*   Natural Logarithm `(ln)`
*   Logarithm Base 10 `(log10)`
*   Logarithm Base N `(logN)`

### 3. Trigonometry (Degrees)
*   Sine `(sin)`, Cosine `(cos)`, Tangent `(tan)`
*   Arc Sine `(asin)`, Arc Cosine `(acos)`, Arc Tangent `(atan)`

### 4. Hyperbolic Functions
*   Hyperbolic Sine `(sinh)`, Hyperbolic Cosine `(cosh)`, Hyperbolic Tangent `(tanh)`

### 5. Special Functions
*   Factorial `(n!)`
*   Absolute Value `(|x|)`
*   Exponential `(e^x)`
*   Reciprocal `(1/x)`
*   Combinations `(nCr)` and Permutations `(nPr)`

---

## 🔄 Unit Conversions

The program includes a comprehensive Unit Conversion Center featuring 11 different categories:

1.  **📏 Length & Distance:** Millimeter, Centimeter, Meter, Kilometer, Inch, Foot, Yard, Mile, Nautical Mile, Micrometer.
2.  **⚖️ Weight & Mass:** Milligram, Gram, Kilogram, Metric Ton, Ounce, Pound, Stone, US Ton, UK Ton.
3.  **⏱️ Time:** Millisecond, Second, Minute, Hour, Day, Week, Month, Year, Decade, Century.
4.  **🌡️ Temperature:** Celsius, Fahrenheit, Kelvin.
5.  **📐 Area:** mm², cm², m², km², Hectare, Acre, ft², yd², in², mi².
6.  **🧊 Volume:** Milliliter, Liter, m³, Gallon (US/UK), Pint, Cup, Fluid Ounce.
7.  **🚀 Speed:** m/s, km/h, mph, Knot, ft/s, Mach, Speed of Light (c).
8.  **💾 Data Storage:** Bit, Byte, Kilobyte, Megabyte, Gigabyte, Terabyte, Petabyte, and binary equivalents (KiB, MiB, etc.).
9.  **🗜️ Pressure:** Pascal, Kilopascal, Bar, Atmosphere (atm), PSI, mmHg (Torr), inHg.
10. **⚡ Energy:** Joule, Kilojoule, Calorie, Kilocalorie, Watt-hour, Kilowatt-hour, Electronvolt, BTU.
11. **💵 Currency:** Conversions between 13 currencies including USD, EUR, GBP, IQD, SAR, AED, TRY, KWD, EGP, JOD, JPY, INR, CNY *(Note: Exchange rates are static for reference purposes only)*.

Plus, quick Angle Conversions: Degrees to Radians `(D->R)` and Radians to Degrees `(R->D)`.

---

## 💻 How to Run

Since the program uses a colorful Command Line interface, it is highly recommended to run it in an environment that supports ANSI Colors (like Windows Terminal or an updated Command Prompt).

### 1. Using Dev-C++ (If installed)
*   Open the `advavced calculator.cpp` file in Dev-C++.
*   Press `F11` (Compile & Run) to compile and launch the application.

### 2. From the Terminal (PowerShell / Command Prompt)
If you have already compiled the program into an executable `calculator.exe`, you can run it directly by typing:
```powershell
& "d:\coding\U project\lectuer\calculator.exe"
```

## 🛠️ Requirements
*   A C++ Compiler supporting `C++14` standard or newer (such as `g++` provided with MinGW).
*   Windows Operating System (ANSI colors are automatically enabled in the code using system API calls).
