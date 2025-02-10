#include <stdio.h>
#include <string.h>

#define MAX_BUSES 10
#define MAX_SEATS 50

// Structure to represent a bus
typedef struct {
    int bus_id;
    char source[50];
    char destination[50];
    int total_seats;
    int available_seats;
    int booked_seats[MAX_SEATS]; // Array to track booked seats
} Bus;

// Array to store all buses
Bus buses[MAX_BUSES];
int bus_count = 0;

// Function to add a new bus
void add_bus(int id, char source[], char destination[], int seats) {
    if (bus_count >= MAX_BUSES) {
        printf("Cannot add more buses. Limit reached.\n");
        return;
    }
    buses[bus_count].bus_id = id;
    strcpy(buses[bus_count].source, source);
    strcpy(buses[bus_count].destination, destination);
    buses[bus_count].total_seats = seats;
    buses[bus_count].available_seats = seats;
    // Initialize all seats as available (0 = not booked, 1 = booked)
    for (int i = 0; i < seats; i++) {
        buses[bus_count].booked_seats[i] = 0;
    }
    bus_count++;
    printf("Bus added successfully!\n");
}

// Function to display all buses
void display_buses() {
    if (bus_count == 0) {
        printf("No buses available.\n");
        return;
    }
    printf("Available Buses:\n");
    printf("ID\tSource\t\tDestination\tTotal Seats\tAvailable Seats\n");
    for (int i = 0; i < bus_count; i++) {
        printf("%d\t%s\t\t%s\t\t%d\t\t%d\n",
               buses[i].bus_id, buses[i].source, buses[i].destination,
               buses[i].total_seats, buses[i].available_seats);
    }
}

// Function to book a seat
void book_ticket(int bus_id, int seat_number) {
    for (int i = 0; i < bus_count; i++) {
        if (buses[i].bus_id == bus_id) {
            if (seat_number < 1 || seat_number > buses[i].total_seats) {
                printf("Invalid seat number.\n");
                return;
            }
            if (buses[i].booked_seats[seat_number - 1] == 1) {
                printf("Seat %d is already booked.\n", seat_number);
                return;
            }
            buses[i].booked_seats[seat_number - 1] = 1;
            buses[i].available_seats--;
            printf("Seat %d booked successfully on Bus %d.\n", seat_number, bus_id);
            return;
        }
    }
    printf("Bus ID not found.\n");
}

// Function to display booked seats for a bus
void display_booked_seats(int bus_id) {
    for (int i = 0; i < bus_count; i++) {
        if (buses[i].bus_id == bus_id) {
            printf("Booked seats for Bus %d:\n", bus_id);
            for (int j = 0; j < buses[i].total_seats; j++) {
                if (buses[i].booked_seats[j] == 1) {
                    printf("Seat %d\n", j + 1);
                }
            }
            return;
        }
    }
    printf("Bus ID not found.\n");
}

// Main function
int main() {
    int choice, bus_id, seat_number;
    char source[50], destination[50];
    int seats;

    while (1) {
        printf("\nBus Ticket Booking System\n");
        printf("1. Add Bus\n");
        printf("2. Display Buses\n");
        printf("3. Book Ticket\n");
        printf("4. Display Booked Seats\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Bus ID: ");
                scanf("%d", &bus_id);
                printf("Enter Source: ");
                scanf("%s", source);
                printf("Enter Destination: ");
                scanf("%s", destination);
                printf("Enter Total Seats: ");
                scanf("%d", &seats);
                add_bus(bus_id, source, destination, seats);
                break;
            case 2:
                display_buses();
                break;
            case 3:
                printf("Enter Bus ID: ");
                scanf("%d", &bus_id);
                printf("Enter Seat Number: ");
                scanf("%d", &seat_number);
                book_ticket(bus_id, seat_number);
                break;
            case 4:
                printf("Enter Bus ID: ");
                scanf("%d", &bus_id);
                display_booked_seats(bus_id);
                break;
            case 5:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}