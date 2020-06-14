# Gameday-Parking-System
This is a prototype system made for game day parking. This system handles parking prices and stores the money generated by the parking tickets depending on the types and number of vehicles. 

I have implemented a deluxe parking system. The system requirements for a deluxe parking system are that, it can function for up to three vehicle types being (Sedan, Truck and SUV). Therefore, it can operate on different parking rates (The price required to pay to park a vehicle per day) for different vehicle types and also remember how many vehicles of each vehicle type are parked throughout the game day. It can handle, like the base system the complex math involving (The deposit amount but for every vehicle type (3), the change that’s left depending on how much money has been paid and how much money is actually required for all the vehicles depending on their type.) 

There are several modules implemented throughout the code. There is a module implemented to the handle the HEX_DISPLAY on the Altera board. There are three modules for each vehicle type implemented to store information about the how many vehicles of that vehicle type are stored in the parking lot and also how much money was made on that vehicle type. There is a money module to handle all the money calculation that happens when you use the program for each vehicle type. There are modules to handle that all the modules execute on time. 
