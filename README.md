# 📄 Smart Document Generator

A lightweight C++ console application that converts CLI user inputs into styled, print-ready HTML documents. Designed as an applied showcase for Data Structures and Algorithms (DSA), the system generates customized **Certificates**, **Resumes**, **Report Cards**, **Invitation Cards**, and **Portfolios**.

> ⚠️ **Key Feature:** Built completely without relying on standard STL containers (`std::vector`, `std::list`, `std::stack`, `std::queue`). All data structures were implemented from scratch.

---

## 🛠️ Architecture Overview

The codebase is built on a 3-layer architecture to ensure clean separation of concerns:

- **Helpers Layer (`helpers.h`):** Input reading, HTML escaping, shared HTML template and CSS.
- **DSA Layer (`dsa_library.h`):** Custom `LinkedList`, `ItemArray`, `ActionStack`, and `StringQueue` implementations.
- **Module Layer (`certificate.h`, `resume.h`, `reportcard.h`, `invitation.h`, `portfolio.h`, `main.cpp`):** Application logic and terminal interface.

---

## 📊 Core Data Structures Usage

| Module | Linked List | Array | Stack (Undo Log) | Queue (Processing) |
| :--- | :--- | :--- | :--- | :--- |
| **Certificate** | Body text lines | Recipients (Search & Sort) | Title, Date, Recipient undo | Generation queue |
| **Resume / CV** | Education, Experience, Projects | Rated Skills (1–5) | Section edit & deletion undo | — |
| **Report Card** | — | Subjects & Marks | Mark correction history | Student report queue |
| **Invitation** | Event details | Guest list | Details undo | Bulk invitation queue |
| **Portfolio** | Projects list | Skills & Achievements | Activity & edit undo | — |
| **Global (`main.cpp`)** | — | — | — | Document Request Queue |

---

## 🚀 Modules & Capabilities

1. **Certificate Generator** (`certificate.h`): Generates gold-bordered certificates for single or batch recipients.
2. **Resume / CV Generator** (`resume.h`): Generates clean, single-page professional resumes with skill proficiency.
3. **Report Card Generator** (`reportcard.h`): Calculates percentages, letter grades (A+ to F), pass/fail statuses, and ranks subjects.
4. **Invitation Card Generator** (`invitation.h`): Creates personalized invitations with custom schedules, dress codes, and details.
5. **Portfolio Generator** (`portfolio.h`): Creates personal showcase pages with project cards, skill levels, and achievements.

---

## 💻 Tech Stack

- **Language:** C++ (C++11 or higher)
- **Compiler:** `g++` / MinGW / Clang
- **Output:** HTML5 & CSS3 (rendered in browser, printable to PDF)

---
