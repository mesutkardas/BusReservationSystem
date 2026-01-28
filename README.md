# Bus Reservation System (C Project)

This project is a console-based **Bus Reservation and Ticketing System** developed in C language. It serves as a comprehensive example of file handling, pointer manipulation, and structured programming concepts.

## 🚀 Features

* **Trip Management:** Create, update, delete, and list bus trips.
* **Ticketing:** Sell tickets with seat conflict checks (preventing double booking).
* **Inquiry:** Search for specific trip details using Trip ID.
* **Cancellation:** Cancel sold tickets and safely update records.
* **Receipt Generation:** Automatically generates a `Ticket_Receipt.txt` file after a sale.
* **Data Persistence:** All data (trips and passengers) are stored in `.txt` files, ensuring no data loss when the program is closed.

## 🛠️ Technical Details

* **Language:** C
* **Data Storage:** File Handling (`travel_information.txt`, `passenger_information.txt`)
* **Key Concepts:**
    * Structs & Arrays
    * Pointers & Memory Management (Pass-by-reference)
    * File I/O (Read, Write, Append, Temporary file operations)
    * Modular Programming (Function-based structure)

## 📂 Project Structure

* `main.c`: The core source code containing the application logic.
* `travel_information.txt`: Database for storing trip details.
* `passenger_information.txt`: Database for storing passenger info.
* `Ticket_Receipt.txt`: Output file for the last generated ticket receipt.

## 👨‍💻 Author

**Mesut Kardaş** - Computer Engineering Student