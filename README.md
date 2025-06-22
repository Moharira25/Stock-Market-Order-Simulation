# 📈 Stock Market Order Simulation

A C++ project that simulates the processing, matching, and execution of trade orders in a simplified stock market environment. Developed as part of the CO3105 module, this project emphasizes core programming skills in C++, STL usage, file handling, and algorithmic logic.

---

## 📌 Overview

This simulation models how buy and sell orders are handled for a single company's stock on a stock exchange. Orders are processed sequentially from an input file. If matching orders exist, they are executed based on price, order type, and arrival time priorities.

---

## 🚀 Features

* ✅ **Market & Limit Orders**: Supports price-constrained (limit) and unconstrained (market) orders.
* ⚖️ **Order Matching**: Matches compatible buy/sell orders automatically.
* 🕒 **Priority Rules**: Based on price, order type (market > limit), and arrival time.
* ✂️ **Partial Execution**: Orders can be partially filled, with remaining quantities added back to the order book.
* 📊 **Real-time Order Book Display**: Buy/Sell orders displayed in structured format after each new input.
* 📝 **Trade Logging**: Executed and unmatched orders are written to a result file.

---

## 📥 Input Format

The program expects input via a command-line-specified file:

```bash
./main input1.txt
```

### Example `input1.txt`

```
10.00                    # Initial market price
ord001 B 100 9.75        # Buy 100 shares at 9.75
ord002 S 100             # Market sell order
ord003 S 100 10.25       # Sell 100 shares at 10.25
```

### Fields

* `Order ID`: Unique identifier (alphanumeric)
* `Type`: `'B'` for Buy, `'S'` for Sell
* `Quantity`: Number of shares
* `Limit Price`: Optional (use `-1` or omit for market orders)

---

## ⚙️ Matching Logic

Orders are matched based on the following criteria:

### 🔁 Matching Conditions

* Opposite order types (`B` vs `S`)
* One or both orders are market orders, OR
* Buy limit ≥ Sell limit

### 🏆 Priority Rules

1. **Price**: Higher buy / Lower sell
2. **Order Type**: Market > Limit
3. **Arrival Time**: Earlier orders first

### 💰 Execution Price Rules

* Both limit orders → price of the older order
* One market order → price of the limit order
* Both market → last traded price

---

## 🖥️ Output

### 🪟 Console

Shows live order book with pending orders:

```
Latest Price: 10.00
Buy                      | Sell
-------------------------+-------------------------
ord001 10.00 100         | ord002 M 50
ord005  9.75 60          | ord003 10.25 100
```

### 📄 Output File (e.g. `output1.txt`)

```
order ord001 20 shares purchased at price 9.75
order ord002 20 shares sold at price 9.75
order ord003 100 shares unexecuted
```

---

## 🔧 Build & Run

### Requirements

* C++ compiler with C++11 support or higher
* `make` utility (optional)

### Compile

```bash
make       # or manually: g++ main.cpp exchange.cpp -o main
```

### Run

```bash
./main input1.txt
```

---

## 📁 Project Structure

* `main.cpp` – Entry point (reads input, invokes Exchange logic)
* `exchange.cpp/.h` – Contains the `Exchange` class implementation
* `Order` structure – Defines attributes like ID, type, quantity, price, and timestamp
* `makefile` – Automates compilation
* `input#.txt` – Sample input files
* `output#.txt` – Generated result files

---

## 🧠 Key Implementation Details

* Uses **`std::set`** for maintaining buy/sell orders with custom sort logic
* Floating point prices shown with **2 decimal places**
* Handles both full and partial order execution
* Execution details saved using **robust file I/O**

---

## 🧪 Testing Guide

1. Use the provided `input#.txt` files.
2. Run the simulation:

   ```bash
   ./main input3.txt
   ```
3. Check console output for live matching.
4. Compare `output3.txt` to expected results.

---

## 🎓 Educational Context

This project was created for the **CO3105** module and demonstrates:

* Object-oriented C++ design
* Use of STL containers and algorithms
* Algorithmic thinking for matching engines
* File parsing and formatting
* Real-world application simulation

---

