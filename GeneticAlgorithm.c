#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
void printIndividual();
int* initialisePopulation();
int* naturalSelection();
int* getFitness();
int* reproduction();
//prints invidual at specified index
void printIndividual(char* pre, int* population, int GENE_SIZE, int i, int (*clac_ptr)())
{
    printf("%s",pre);
    for (int j = 0; j < GENE_SIZE; j++)
    {
        printf("%d", *(population+i*GENE_SIZE+j));
    }
    printf(" fitness: %d",clac_ptr(population, GENE_SIZE, i));
}
//print each individual in the population
void printPopulation(int* population, int POPULATION_SIZE, int GENE_SIZE, int verbosity, int* fitness){
    if(verbosity > 0){
        for(int i = 0; i < POPULATION_SIZE ; i++){
            printf("%d/%d: ",i+1,POPULATION_SIZE);
            //printIndividual("|",population,GENE_SIZE,i); 
            printf("|>>%d",fitness[i]);
            printf("\n");
        }
        printf("_____________________________________________________________________\n");
    }
}
//create  population of genes that are 0 or 1
int* initialisePopulation(int* population,int POPULATION_SIZE, int GENE_SIZE , int verbosity){
    int i, j;
    for (i = 0; i < POPULATION_SIZE*GENE_SIZE; i++)
    {
        *(population+i) = rand() % 2;
        if(*(population+i) < 0 || *(population+i) > 1){
                        printf("%d<\n",*(population+i));
                        printf("uhoh3");
                        exit(1);      
                    }
    }
    return population;
}
//returns an array of fitness values for each individual in the population
int* naturalSelection(int* primePopulation,int* population, int* fitness, int POPULATION_SIZE, int GENE_SIZE, int verbosity,int numParents){
   for (int h = 0; h < numParents; h++)
    {
        for (int i = 0; i < POPULATION_SIZE; i++)
        {
            int success = 1;
            for (int j = 0; j < POPULATION_SIZE; j++)
            {
                if (fitness[i] < fitness[j])
                {
                    success = 0;
                    break;
                }
            }
            if (success)
            {
                for (int k = 0; k < GENE_SIZE; k++)
                {
                    *(primePopulation+h*GENE_SIZE+k) = *(population+i*GENE_SIZE+k);                        
                }
                h++;
            }
        }
    }
    return primePopulation;
}
//returns the fitness of the population
int* getFitness(int* fitness, int* population, int POPULATION_SIZE, int GENE_SIZE, int verbosity,int numParents,int (*calc_ptr)()){
    int i;
    for (i = 0; i < POPULATION_SIZE; i++)
    {
        fitness[i] = calc_ptr(population,GENE_SIZE,i);
        if(*(population+i) > 1||*(population+i) < 0 ){
            printf("%d<%d",*(population+i),i);
            exit(0); 
        }

    }
    return fitness;
}
//creates a new population by randomly selecting individuals from the prime population with a chang of mutation
int* reproduction(int* fitness, int* population,int* primePopulation, int POPULATION_SIZE, int GENE_SIZE, int verbosity,int numParents, int mutationRate){
    for(int j = 0; j <  numParents; j++){
        for(int i = 0; i < GENE_SIZE; i++){
            *(population+j*GENE_SIZE+i) = *(primePopulation+j*GENE_SIZE+i);
        }
    }
    int i, j;
    srand(time(NULL));
    int total = 0;
    for (i = numParents; i < POPULATION_SIZE-POPULATION_SIZE/10; i++)
    {
        for (j = 0; j < GENE_SIZE; j++)
        {
            if(rand() % mutationRate*numParents < numParents){
                *(population+i*GENE_SIZE+j) = *(primePopulation+(rand() % numParents)*GENE_SIZE+j);
            }
            else{
                total++;
                *(population+i*GENE_SIZE+j) = rand() % 2;
            }
        }
    }
    for (i = POPULATION_SIZE-POPULATION_SIZE/10; i < POPULATION_SIZE/10*GENE_SIZE; i++)
    {
        *(population+i) = rand() % 2;
    }
    return population;
}
//runs the genetic algorithm
int* geneticAlgorithm(int POPULATION_SIZE, int GENE_SIZE, int maxFitness, int maxGens, int verbosity, int numParents,int mutationRate, int (*calc_ptr)()){
    int* population = (int*)malloc(POPULATION_SIZE * GENE_SIZE * sizeof(int));
    int* fitness = (int*)malloc(POPULATION_SIZE * sizeof(int));
    int* primePopulation = (int*)malloc(numParents * GENE_SIZE * sizeof(int));
    
     fitness = getFitness(fitness,initialisePopulation(population,POPULATION_SIZE,GENE_SIZE, verbosity),POPULATION_SIZE,GENE_SIZE, verbosity, numParents,calc_ptr);
     int count = 1;
    while(fitness[0] < maxFitness && count <= maxGens){
        fitness = getFitness(fitness,reproduction(fitness,population,naturalSelection(population,population,fitness,POPULATION_SIZE,GENE_SIZE, verbosity,numParents),POPULATION_SIZE,GENE_SIZE, verbosity,numParents, mutationRate),POPULATION_SIZE,GENE_SIZE, verbosity,numParents, calc_ptr);
        if(count % 100 == 0){
            printf("Gen %d|",count++);
            printIndividual("Genes|",population,GENE_SIZE,0, calc_ptr);
            printf("\r");
        } else {
            count++;
        }
    }
    int* output = (int*)malloc(GENE_SIZE * sizeof(int));
    for(int i = 0; i < GENE_SIZE; i++){
        *(output+i) = *(population+i);
    }
    free(population);
    free(fitness);
    free(primePopulation); 
    return(output);
}
//Custom fitness function designed by user, the population, the size of the genes and the index as an input
//this can be used to test the genes of an individual as shown below
//genes of an individual are in the range (population+index*size) to (population+index*size+size)
int fitnessCalculator(int* population, int size, int index){
    int fitness = 0;
    for (int i = 0; i < size; i++)
    {
        if(*(population+index*size+i) == 1){
            fitness++;
        }
    }
    return fitness;   
}
int main()
{
    int pop = 1000;
    int size = 100;
    int numParents = 30;
    int mutationRate = 100;
    int maxGens = 1000000;
    int (*calc_ptr)(int*population,int size, int index);
    calc_ptr = &fitnessCalculator;
    int * result= geneticAlgorithm(pop,size,size,maxGens,0,numParents,mutationRate,calc_ptr);
    printIndividual("Result:",result,size,0, calc_ptr);
    //printIndividual("\nTarget:",target,size,0);
    printf("\n\n\n");
    //printf("%d\n",(*calc)(0,0));s
    return 0;
}