# Baklava Making Simulation Program

This program simulates the process of making baklava, a traditional pastry. The program involves a wholesaler and six chefs. The chefs have access to an unlimited supply of two specific ingredients and a limited supply of two other specific ingredients. The wholesaler supplies the necessary ingredients, and the chefs carry out the baklava making process.

## Usage

To compile and run the program, follow these steps:

1. Compile the program using the following command:
   ```bash
   gcc -o baklava main.c
   ```

2. Run the program with an input file as an argument:
   ```bash
   ./baklava [input file]
   ```

## Program Flow

1. The program initializes the required ingredients for the chefs and the necessary semaphores.
2. It reads the specified input file, which contains pairs of characters representing ingredients. Each pair corresponds to the ingredients bought by the wholesaler.
3. The program adds the ingredients to the wholesaler's inventory.
4. While there are enough ingredients to make baklava, the following steps are repeated:
   - The chefs check if they have the required ingredients.
   - If the ingredients are available, the chef obtains the ingredients from the wholesaler, prepares the baklava, and delivers it to the wholesaler.
   - The wholesaler receives the baklava and continues to sell it.
5. When there are not enough ingredients to make baklava, the program terminates.
6. The program cleans up resources and exits.

Note: The program utilizes semaphores for synchronization between the chefs and the wholesaler to ensure the availability of ingredients and the orderly preparation and delivery of baklava.

Feel free to modify and enhance the program as needed!
