#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
int count = 1;
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
        printf("%d", *(population+i*GENE_SIZE+j));
    }
}
//print each individual in the population
void printPopulation(int* population, int POPULATION_SIZE, int GENE_SIZE, int verbosity, int* fitness){
    if(verbosity > 0){
        for(int i = 0; i < POPULATION_SIZE ; i++){
            printf("%d/%d: ",i+1,POPULATION_SIZE);
            printIndividual("|",population,GENE_SIZE,i); 
            printf("|>>%d",fitness[i]);
            printf("\n");
        }
        printf("_____________________________________________________________________\n");
    }
}
//create  population of genes that are 0 or 1
int* initialisePopulation(int* population,int POPULATION_SIZE, int GENE_SIZE , int verbosity, int* target){
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
int* naturalSelection(int* primePopulation,int* population, int* fitness, int POPULATION_SIZE, int GENE_SIZE,int* target, int verbosity,int numParents){
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
int* getFitness(int* fitness, int* population, int POPULATION_SIZE, int GENE_SIZE, int* target, int verbosity,int numParents){
    int i;
    for (i = 0; i < POPULATION_SIZE; i++)
    {
        fitness[i] = testIndividual(population, GENE_SIZE,target, i);
        if(*(population+i) > 1||*(population+i) < 0 ){
            printf("%d<%d",*(population+i),i);
            exit(0); 
        }

    }

    if(count % 100 == 0){
    printf("Gen %d|",count++);
    printf("Fit %d|",fitness[0]);
    printIndividual("Genes|",population,GENE_SIZE,0);
    printf("\r");
    } else {
        count++;
    }
    return fitness;
}
//creates a new population by randomly selecting individuals from the prime population with a chang of mutation
int* reproduction(int* fitness, int* population,int* primePopulation, int POPULATION_SIZE, int GENE_SIZE,int* target, int verbosity,int numParents){
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
            if(rand() % 5*numParents < numParents){
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
int* geneticAlgorithm(int POPULATION_SIZE, int GENE_SIZE, int*target, int verbosity, int numParents){
    int* population = (int*)malloc(POPULATION_SIZE * GENE_SIZE * sizeof(int));
    int* fitness = (int*)malloc(POPULATION_SIZE * sizeof(int));
    int* primePopulation = (int*)malloc(numParents * GENE_SIZE * sizeof(int));
    
     fitness = getFitness(fitness,initialisePopulation(population,POPULATION_SIZE,GENE_SIZE, verbosity,target),POPULATION_SIZE,GENE_SIZE,target, verbosity, numParents);
    while(fitness[0] != GENE_SIZE){
        fitness = getFitness(fitness,reproduction(fitness,population,naturalSelection(population,population,fitness,POPULATION_SIZE,GENE_SIZE,target, verbosity,numParents),POPULATION_SIZE,GENE_SIZE,target, verbosity,numParents),POPULATION_SIZE,GENE_SIZE,target, verbosity,numParents);
    
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
int main()
{
    int pop = 100;
    int size = 20;
    int numParents = 3;
    int* target = malloc(size*sizeof(int));
    for (int i = 0; i < size; i++)
    {
        *(target+i) = 1;//rand() % 2;
    }
   
    printIndividual("Result:",geneticAlgorithm(pop,size,target,0,numParents),size,0);
    printIndividual("\nTarget:",target,size,0);
    printf("\n\n\n");
    
    free(target);
    return 0;  
}