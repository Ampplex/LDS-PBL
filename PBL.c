#include <stdio.h>
#include <string.h>

#define MAX_BUSES 10
#define MAX_SEATS 50
#define MAX_WAITLIST 10  // Max waitlisted passengers per bus

// Structure for waitlist queue
typedef struct {
    int front, rear, size;
    int passengers[MAX_WAITLIST];
} Queue;

// Function to initialize the queue
void initQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->size = 0;
}

// Function to check if queue is empty
int isQueueEmpty(Queue *q) {
    return q->size == 0;
}

// Function to check if queue is full
int isQueueFull(Queue *q) {
    return q->size == MAX_WAITLIST;
}

// Function to enqueue a passenger
void enqueue(Queue *q, int passenger) {
    if (isQueueFull(q)) {
        printf("Waitlist is full!\n");
        return;
    }
    q->rear = (q->rear + 1) % MAX_WAITLIST;
    q->passengers[q->rear] = passenger;
    q->size++;
}

// Function to dequeue a passenger
int dequeue(Queue *q) {
    if (isQueueEmpty(q)) {
        return -1;
    }
    int passenger = q->passengers[q->front];
    q->front = (q->front + 1) % MAX_WAITLIST;
    q->size--;
    return passenger;
}

// Structure to represent a bus
typedef struct {
    int bus_id;
    char source[50];
    char destination[50];
    int total_seats;
    int available_seats;
    int booked_seats[MAX_SEATS];
    Queue waitlist;  // Queue for waitlisted passengers
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
    for (int i = 0; i < seats; i++) {
        buses[bus_count].booked_seats[i] = 0;
    }
    initQueue(&buses[bus_count].waitlist); // Initialize waitlist queue
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

// Function to book a seat or add to waitlist
void book_ticket(int bus_id, int seat_number, int passenger_id) {
    for (int i = 0; i < bus_count; i++) {
        if (buses[i].bus_id == bus_id) {
            if (buses[i].available_seats > 0) {
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
                printf("Seat %d booked successfully for Passenger %d on Bus %d.\n", seat_number, passenger_id, bus_id);
            } else {
                if (!isQueueFull(&buses[i].waitlist)) {
                    enqueue(&buses[i].waitlist, passenger_id);
                    printf("Bus is full! Passenger %d added to the waitlist for Bus %d.\n", passenger_id, bus_id);
                } else {
                    printf("Bus is full, and waitlist is full! No booking possible for Passenger %d.\n", passenger_id);
                }
            }
            return;
        }
    }
    printf("Bus ID not found.\n");
}

// Function to cancel a booked seat and assign to waitlist if available
void cancel_ticket(int bus_id, int seat_number) {
    for (int i = 0; i < bus_count; i++) {
        if (buses[i].bus_id == bus_id) {
            if (seat_number < 1 || seat_number > buses[i].total_seats || buses[i].booked_seats[seat_number - 1] == 0) {
                printf("Invalid cancellation. Seat %d is not booked.\n", seat_number);
                return;
            }
            buses[i].booked_seats[seat_number - 1] = 0;
            buses[i].available_seats++;

            // Check if waitlisted passengers exist
            if (!isQueueEmpty(&buses[i].waitlist)) {
                int next_passenger = dequeue(&buses[i].waitlist);
                buses[i].booked_seats[seat_number - 1] = 1;
                buses[i].available_seats--;
                printf("Seat %d is now assigned to waitlisted Passenger %d.\n", seat_number, next_passenger);
            } else {
                printf("Seat %d canceled successfully on Bus %d.\n", seat_number, bus_id);
            }
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
    int choice, bus_id, seat_number, passenger_id;
    char source[50], destination[50];
    int seats;

    while (1) {
        printf("\nBus Ticket Booking System\n");
        printf("1. Add Bus\n");
        printf("2. Display Buses\n");
        printf("3. Book Ticket\n");
        printf("4. Cancel Ticket\n");
        printf("5. Display Booked Seats\n");
        printf("6. Exit\n");
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
                printf("Enter Passenger ID: ");
                scanf("%d", &passenger_id);
                book_ticket(bus_id, seat_number, passenger_id);
                break;
            case 4:
                printf("Enter Bus ID: ");
                scanf("%d", &bus_id);
                printf("Enter Seat Number: ");
                scanf("%d", &seat_number);
                cancel_ticket(bus_id, seat_number);
                break;
            case 5:
                printf("Enter Bus ID: ");
                scanf("%d", &bus_id);
                display_booked_seats(bus_id);
                break;
            case 6:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
