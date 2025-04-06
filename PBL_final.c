#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUSES 10
#define MAX_SEATS 50
#define MAX_WAITLIST 10

// Structure to represent a passenger in the waitlist
typedef struct {
    int passenger_id;
    char passenger_name[50];
} WaitlistPassenger;

// Structure to represent a bus
typedef struct
{
    int bus_id;
    char source[50];
    char destination[50];
    int total_seats;
    int available_seats;
    float fare;  // Added fare field
    int booked_seats[MAX_SEATS]; // Array to track booked seats
    
    // Waitlist queue
    WaitlistPassenger waitlist[MAX_WAITLIST];
    int waitlist_front;
    int waitlist_rear;
    int waitlist_count;
} Bus;

// Array to store all buses
Bus buses[MAX_BUSES];
int bus_count = 0;
int next_passenger_id = 1;

// Function to add a new bus
void add_bus(int id, char source[], char destination[], int seats, float fare)
{
    if (bus_count >= MAX_BUSES)
    {
        printf("Cannot add more buses. Limit reached.\n");
        return;
    }
    buses[bus_count].bus_id = id;
    strcpy(buses[bus_count].source, source);
    strcpy(buses[bus_count].destination, destination);
    buses[bus_count].total_seats = seats;
    buses[bus_count].available_seats = seats;
    buses[bus_count].fare = fare;  // Store the fare
    
    // Initialize booked seats
    for (int i = 0; i < seats; i++)
    {
        buses[bus_count].booked_seats[i] = 0;
    }
    
    // Initialize waitlist
    buses[bus_count].waitlist_front = 0;
    buses[bus_count].waitlist_rear = -1;
    buses[bus_count].waitlist_count = 0;
    
    bus_count++;
    printf("Bus added successfully!\n");
}

// Function to display all buses
void display_buses()
{
    if (bus_count == 0)
    {
        printf("No buses available.\n");
        return;
    }
    printf("Available Buses:\n");
    printf("ID\tSource\t\tDestination\tFare($)\tTotal Seats\tAvailable Seats\tWaitlist\n");
    for (int i = 0; i < bus_count; i++)
    {
        printf("%d\t%s\t\t%s\t\t%.2f\t%d\t\t%d\t\t%d\n",
               buses[i].bus_id, buses[i].source, buses[i].destination,
               buses[i].fare, buses[i].total_seats, buses[i].available_seats, 
               buses[i].waitlist_count);
    }
}

// Function to book a ticket
void book_ticket(int bus_id, char passenger_name[])
{
    for (int i = 0; i < bus_count; i++)
    {
        if (buses[i].bus_id == bus_id)
        {
            // If seats are available, book a seat
            if (buses[i].available_seats > 0)
            {
                // Find first available seat
                for (int j = 0; j < buses[i].total_seats; j++)
                {
                    if (buses[i].booked_seats[j] == 0)
                    {
                        buses[i].booked_seats[j] = 1;
                        buses[i].available_seats--;
                        printf("Seat %d booked successfully on Bus %d for %s.\n", 
                               j + 1, bus_id, passenger_name);
                        printf("Fare: $%.2f\n", buses[i].fare);  // Display fare on booking
                        return;
                    }
                }
            }
            
            // If no seats available, check waitlist
            if (buses[i].waitlist_count < MAX_WAITLIST)
            {
                // Add to waitlist
                buses[i].waitlist_rear = (buses[i].waitlist_rear + 1) % MAX_WAITLIST;
                strcpy(buses[i].waitlist[buses[i].waitlist_rear].passenger_name, passenger_name);
                buses[i].waitlist[buses[i].waitlist_rear].passenger_id = next_passenger_id++;
                buses[i].waitlist_count++;
                
                printf("No seats available. %s added to waitlist. Waitlist position: %d\n", 
                       passenger_name, buses[i].waitlist_count);
                return;
            }
            
            // Waitlist is full
            printf("Sorry, bus is fully booked and waitlist is full.\n");
            return;
        }
    }
    printf("Bus ID not found.\n");
}

// Function to display booked seats for a bus
void display_booked_seats(int bus_id)
{
    for (int i = 0; i < bus_count; i++)
    {
        if (buses[i].bus_id == bus_id)
        {
            printf("Bus %d: %s to %s (Fare: $%.2f)\n", 
                   bus_id, buses[i].source, buses[i].destination, buses[i].fare);  // Display fare
            
            printf("Booked seats:\n");
            for (int j = 0; j < buses[i].total_seats; j++)
            {
                if (buses[i].booked_seats[j] == 1)
                {
                    printf("Seat %d\n", j + 1);
                }
            }
            
            // Display waitlist
            printf("\nWaitlist for Bus %d:\n", bus_id);
            if (buses[i].waitlist_count == 0)
            {
                printf("No passengers in waitlist.\n");
            }
            else
            {
                for (int j = 0; j < buses[i].waitlist_count; j++)
                {
                    int index = (buses[i].waitlist_front + j) % MAX_WAITLIST;
                    printf("Position %d: %s (ID: %d)\n", 
                           j + 1, 
                           buses[i].waitlist[index].passenger_name,
                           buses[i].waitlist[index].passenger_id);
                }
            }
            return;
        }
    }
    printf("Bus ID not found.\n");
}

// Function to cancel a ticket and potentially fill from waitlist
void cancel_ticket(int bus_id, int seat_number)
{
    for (int i = 0; i < bus_count; i++)
    {
        if (buses[i].bus_id == bus_id)
        {
            if (seat_number < 1 || seat_number > buses[i].total_seats)
            {
                printf("Invalid seat number.\n");
                return;
            }
            
            if (buses[i].booked_seats[seat_number - 1] == 0)
            {
                printf("Seat %d is not booked.\n", seat_number);
                return;
            }
            
            // Cancel the seat
            buses[i].booked_seats[seat_number - 1] = 0;
            buses[i].available_seats++;
            printf("Seat %d cancelled on Bus %d.\n", seat_number, bus_id);
            
            // Check if there's a waitlist passenger to fill the seat
            if (buses[i].waitlist_count > 0)
            {
                // Get the first passenger from waitlist
                WaitlistPassenger next_passenger = buses[i].waitlist[buses[i].waitlist_front];
                
                // Book the newly available seat for the waitlist passenger
                buses[i].booked_seats[seat_number - 1] = 1;
                buses[i].available_seats--;
                
                // Remove passenger from waitlist
                buses[i].waitlist_front = (buses[i].waitlist_front + 1) % MAX_WAITLIST;
                buses[i].waitlist_count--;
                
                printf("Waitlist passenger %s has been allocated seat %d.\n", 
                       next_passenger.passenger_name, seat_number);
                printf("Fare: $%.2f\n", buses[i].fare);  // Display fare on waitlist allocation
            }
            return;
        }
    }
    printf("Bus ID not found.\n");
}

int main()
{
    int choice, bus_id, seat_number;
    char source[50], destination[50], passenger_name[50];
    int seats;
    float fare;

    while (1)
    {
        printf("\nBus Ticket Booking System\n");
        printf("1. Add Bus\n");
        printf("2. Display Buses\n");
        printf("3. Book Ticket\n");
        printf("4. Display Booked Seats & Waitlist\n");
        printf("5. Cancel Ticket\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter Bus ID: ");
            scanf("%d", &bus_id);
            printf("Enter Source: ");
            scanf("%s", source);
            printf("Enter Destination: ");
            scanf("%s", destination);
            printf("Enter Total Seats: ");
            scanf("%d", &seats);
            printf("Enter Fare ($): ");
            scanf("%f", &fare);  // Get fare input
            add_bus(bus_id, source, destination, seats, fare);
            break;
        case 2:
            display_buses();
            break;
        case 3:
            printf("Enter Bus ID: ");
            scanf("%d", &bus_id);
            printf("Enter Passenger Name: ");
            scanf("%s", passenger_name);
            book_ticket(bus_id, passenger_name);
            break;
        case 4:
            printf("Enter Bus ID: ");
            scanf("%d", &bus_id);
            display_booked_seats(bus_id);
            break;
        case 5:
            printf("Enter Bus ID: ");
            scanf("%d", &bus_id);
            printf("Enter Seat Number to Cancel: ");
            scanf("%d", &seat_number);
            cancel_ticket(bus_id, seat_number);
            break;
        case 6:
            printf("Exiting...\n");
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}