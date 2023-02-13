# Genetic Algorithm Implementation in C
This repository contains an implementation of a genetic algorithm in C. The purpose of this project is to demonstrate the basic concepts and mechanisms of a genetic algorithm in a simple, accessible manner.

### Getting Started
To get started, simply clone or download the repository to your local machine. The code is written in C and requires a C compiler to run. To compile the code, navigate to the directory containing the code and run the following command in your terminal:

```gcc genetic_algorithm.c -o genetic_algorithm```
This will compile the code and generate an executable file called genetic_algorithm. To run the code, simply enter the following command in your terminal:

```./genetic_algorithm```
The code will run and display the results of the genetic algorithm in the terminal.

### Understanding the Code
The code is structured into several functions, each performing a specific task in the genetic algorithm. These functions are:
| Function | Description |
| ------ | ------ |
|printIndividual()| This function takes a population, the size of each gene, and an index and prints the individual at the specified index.|
|initialisePopulation()| This function initialises the population of genes as either 0 or 1.|
|fitnessCalculator()| This function tests the fitness of an individual and returns a fitness score.|
|naturalSelection()| This function performs natural selection and returns the fittest individuals from the population.|
|getFitness()| This function calculates the fitness of the entire population and returns an array of fitness scores.|
|reproduction()| This function creates a new population by randomly selecting individuals from the prime population with a chance of mutation.|

The code contains comments explaining the purpose of each function and how it works. By reading the code and the comments, you should be able to gain a better understanding of the mechanisms of a genetic algorithm.

### Customising the Code
The code can be customised to fit your specific requirements. You can change the parameters such as the size of the population, the size of each gene, the number of parants for each individual, and the mutation rate to suit your needs.

Additionally, the fitnessCalculator() function is just a simple placeholder function to show a very basic example of how fitness can be calculated using the genes. You should modify the fitnessCalculator() or pass a different function that outputs an int and takes in the same arguments as a  pointer in through calc_ptr. View code comments above fitnessCalculator() for more information.

### Conclusion
This genetic algorithm implementation in C is a simple, basic demonstration of the concepts and mechanisms of a genetic algorithm. By studying and customising the code, you should be able to gain a deeper understanding of how genetic algorithms work and how to implement them in your own projects.
