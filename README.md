# Embedded Systems & C Programming Practice Repository

This repository is a **collection of hands-on programs, experiments, and projects** focused on **C programming, Embedded Systems, Linux device drivers, data structures, and system-level development**. It reflects my learning journey, interview preparation, and real-world practice across bare‑metal, Linux kernel, and user‑space development.

---

## 📁 Repository Structure Overview

### 🔹 Bare_Metal

Bare‑metal embedded programs without an operating system.

* PWM generation using **TIM3**
* Direct register-level programming

```
Bare_Metal/
└── PWM_on_TIM3.c
```

---

### 🔹 ccodes

Core **C language practice programs** covering fundamentals and problem solving.

#### ➤ array

* Array traversal
* Sum of array elements
* Counting consecutive elements

#### ➤ basic

* Palindrome check
* `static` keyword usage

#### ➤ BitManipulation

* Even/Odd check
* Set, toggle, and check bits
* Power of 2 check
* Swap numbers without temp variable

#### ➤ DS

* Singly linked list implementations

#### ➤ string

* String reversal
* Longest substring problems
* String copy logic

#### ➤ threads

* Basic multithreading examples

---

### 🔹 CPlus

Basic **C++ programs**.

```
CPlus/
└── basics/
    └── hello.cpp
```

---

### 🔹 jetson-nano-health-monitoring

A **system‑level project** for monitoring device health on **NVIDIA Jetson Nano**.

**Key components:**

* CLI tool for health diagnostics
* Python daemon for monitoring & alerting
* Linux kernel driver
* Sysfs interface
* Systemd service

**Tech stack:**

* Python
* Linux kernel modules
* Sysfs
* Systemd

---

### 🔹 ldd (Linux Device Drivers)

Linux kernel module examples and driver development practice.

**Includes:**

* Hello world kernel module
* Character device drivers
* Major/minor number handling
* I2C drivers
* UART interrupt driver
* Kernel module parameters

```
ldd/basics/
```

---

### 🔹 preparation

Interview preparation and algorithm practice.

#### ➤ Data Structures

* Stack, queue, linked list
* Sorting and searching algorithms

#### ➤ LeetCode problems

* Longest palindrome substring
* Longest substring without repeating characters
* Median of two sorted arrays
* Zigzag conversion

#### ➤ Mini projects

* Car parking system (Python)
* Reverse linked list

---

### 🔹 WiFi_driver

Custom **WiFi driver experiments**.

* Driver source code
* Debug utilities

---

## 🛠 Technologies & Skills Used

* **Languages:** C, C++, Python
* **Embedded Systems:** Bare‑metal programming, PWM, timers
* **Linux Internals:** Kernel modules, device drivers, sysfs
* **Data Structures & Algorithms**
* **Multithreading & Bit Manipulation**
* **System Programming**

---

## 🎯 Purpose of This Repository

* Strengthen **C & Embedded Systems fundamentals**
* Practice **Linux kernel and driver development**
* Prepare for **technical interviews**
* Maintain a structured reference for future projects

---

## 🚀 How to Use

* Each folder is **self‑contained**
* Compile C programs using:

```bash
gcc file.c -o output
```

* Kernel modules contain Makefiles for building

---

## 📌 Notes

* This repository is **actively evolving**
* Code is written for **learning and experimentation**
* Suggestions and improvements are welcome

---

## 👤 Author

**Naveen Yadav**
Embedded Software Engineer

---

⭐ If you find this repository useful, consider starring it!
