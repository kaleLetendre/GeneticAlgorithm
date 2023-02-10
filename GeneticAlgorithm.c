#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int count = 0;
void printIndividual();
int* initialisePopulation();
int testIndividual();
int* naturalSelection();
int* getFitness();
int* reproduction();
//prints invidual at specified index
void printIndividual(char* pre, int* population, int GENE_SIZE, int i)
{
    printf("%s",pre);
    for (int j = 0; j < GENE_SIZE; j++)
    {
        printf("|%d", *(population+i*GENE_SIZE+j));
    }
}
void printPopulation(int* population, int POPULATION_SIZE, int GENE_SIZE, int verbosity){
    if(verbosity > 0){
        for(int i = 0; i < POPULATION_SIZE ; i++){
            printf("%d/%d: ",i+1,POPULATION_SIZE);
            printIndividual("|",population,GENE_SIZE,i); 
            printf("|\n");
        }
        printf("_____________________________________________________________________\n");
    }
}
//create  population of genes that are 0 or 1
int* initialisePopulation(int POPULATION_SIZE, int GENE_SIZE , int verbosity)
{
    printf("initialising population...\n");
    int i, j;
    int* population = (int*)malloc(POPULATION_SIZE * GENE_SIZE * sizeof(int));
    for (i = 0; i < POPULATION_SIZE*GENE_SIZE; i++)
    {
        *(population+i) = rand() % 2;
    }
    printPopulation(population,POPULATION_SIZE,GENE_SIZE, verbosity);
    return population;
}
//test the fitness of an individual
int testIndividual(int* population, int GENE_SIZE, int* target, int index)
{   
    int fitness = 0;
    for (int i = 0; i < GENE_SIZE; i++)
    {
        if (*(population+index*GENE_SIZE+i) == *(target+i))
        {
            fitness++;

        }
    }
    return fitness;
}
//returns an array of fitness values for each individual in the population
int* naturalSelection(int* population, int* fitness, int POPULATION_SIZE, int GENE_SIZE,int* target, int verbosity){
   int* primePopulation = (int*)malloc(3 * GENE_SIZE * sizeof(int));
   int success = 1;
   for (int h = 0; h < 3; h++)
    {
        for (int i = 0; i < POPULATION_SIZE; i++)
        {
            for (int j = 0; j < POPULATION_SIZE; j++)
            {
                if (fitness[i] < fitness[j])
                {
                    success = 0;
                    break;
                }
            }
            if (success == 1)
            {
                for (int k = 0; k < GENE_SIZE; k++)
                {
                    *(primePopulation+h*GENE_SIZE+k) = *(population+i*GENE_SIZE+k);
                }
            }
            h++;
        }
    }
    free(fitness);
    //printf("Big boys\n");
    printPopulation(primePopulation,3,GENE_SIZE,verbosity);
    
    return reproduction(population,primePopulation,POPULATION_SIZE,GENE_SIZE, target, verbosity);
}
//returns the fitness of the population
int* getFitness(int* population, int POPULATION_SIZE, int GENE_SIZE, int* target, int verbosity){
    //printf("Gen Pop\n");
    printPopulation(population,POPULATION_SIZE,GENE_SIZE, verbosity);
    int i;
    int* fitness = (int*)malloc(POPULATION_SIZE * sizeof(int));
    for (i = 0; i < POPULATION_SIZE; i++)
    {
        fitness[i] = testIndividual(population, GENE_SIZE,target, i);
    }   
    return (naturalSelection(population,fitness,POPULATION_SIZE,GENE_SIZE, target, verbosity));
}
//creates a new population by randomly selecting individuals from the prime population with a chang of mutation
int* reproduction(int* population,int* primePopulation, int POPULATION_SIZE, int GENE_SIZE,int* target, int verbosity){
    if(testIndividual(primePopulation,GENE_SIZE,target, 0) == GENE_SIZE){
        int* temp = (int*)malloc(GENE_SIZE * sizeof(int));
        for (int i = 0; i < GENE_SIZE; i++){
            *(temp+i) = *(primePopulation+i);
        }
        free(population);
        free(primePopulation);
        return temp;
    };
   printIndividual("\nBest",primePopulation,GENE_SIZE,0);
    memset(population, 0, POPULATION_SIZE * GENE_SIZE * sizeof(int));
    int i, j;
    for (i = 0; i < POPULATION_SIZE; i++)
    {
        for (j = 0; j < GENE_SIZE; j++)
        {
            int random = rand() % 4;
            if(random < 3){
                *(population+i*GENE_SIZE+j) = *(primePopulation+random*GENE_SIZE+j);
            }
            else{
                *(population+i*GENE_SIZE+j) = rand() % 2;
            }
        }
    }
    free(primePopulation);
    
    
    return getFitness(population,POPULATION_SIZE,GENE_SIZE,target, verbosity);
}
int* geneticAlgorithm(int POPULATION_SIZE, int GENE_SIZE, int*target, int verbosity){
    return getFitness(initialisePopulation(POPULATION_SIZE,GENE_SIZE, verbosity),POPULATION_SIZE,GENE_SIZE,target, verbosity);
}
int main()
{
    int pop = 420;
    int size = 5;
    int* target = malloc(size*sizeof(int));
    for (int i = 0; i < size; i++)
    {
        *(target+i) = 1;//rand() % 2;
    }
   
    printIndividual("Result:",geneticAlgorithm(pop,size,target,0),5,0);
    printIndividual("\nTarget:",target,5,0);
    printf("\n\n\n");
    
    free(target);
    return 0;  
}