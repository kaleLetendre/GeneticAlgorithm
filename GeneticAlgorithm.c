#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
void printIndividual();
int* initialisePopulation();
int* naturalSelection();
int* getFitness();
int* reproduction();
int* geneticAlgorithm();

//prints invidual at specified index
void printIndividual(char* pre, int* population, int GENE_SIZE, int i, int (*clac_ptr)(), int verbosity)
{
    if (verbosity > 0){
        printf("%s",pre);
        for (int j = 0; j < GENE_SIZE; j++)
        {
            printf("%d", *(population+i*GENE_SIZE+j));
        }
        printf(" fitness: %d                                                       ",clac_ptr(population, GENE_SIZE, i));
    }
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
int* initialisePopulation(int* population,int POPULATION_SIZE, int GENE_SIZE , int minGeneValue, int maxGeneValue){
    int i, j;
    for (i = 0; i < POPULATION_SIZE*GENE_SIZE; i++)
    {
        *(population+i) = (rand() % (maxGeneValue - minGeneValue + 1)) + minGeneValue;
        if(*(population+i) < minGeneValue || *(population+i) > maxGeneValue){
                        printf("%d<\n",*(population+i));
                        printf("error, gene value out of range");
                        exit(1);      
                    }
    }
    return population;
}
//returns an array of fitness values for each individual in the population
int* naturalSelection(int* primePopulation,int* population, int* fitness, int POPULATION_SIZE, int GENE_SIZE,int numParents){
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
            if (success && h < numParents)
            {
                for (int k = 0; k < GENE_SIZE; k++)
                {
                    *(primePopulation+h*GENE_SIZE+k) = *(population+i*GENE_SIZE+k);                        
                }
                h++;
                //printf("h: %d\n",h);
            }
        }
    }
    return primePopulation;
}
//returns the fitness of the population
int* getFitness(int* fitness, int* population, int POPULATION_SIZE, int GENE_SIZE, int numParents,int (*calc_ptr)()){
    int i;
    for (i = 0; i < POPULATION_SIZE; i++)
    {
        fitness[i] = calc_ptr(population,GENE_SIZE,i);
    }
    return fitness;
}
//creates a new population by randomly selecting individuals from the prime population with a chang of mutation
int* reproduction(int* population,int* primePopulation, int POPULATION_SIZE, int GENE_SIZE,int numParents, int mutationRate, int minGeneValue, int maxGeneValue){
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
                *(population+i*GENE_SIZE+j) = (rand() % (maxGeneValue - minGeneValue + 1)) + minGeneValue;
            }
        }
    }
    for (i = POPULATION_SIZE-POPULATION_SIZE/10; i < POPULATION_SIZE/10*GENE_SIZE; i++)
    {
        *(population+i) = (rand() % (maxGeneValue - minGeneValue + 1)) + minGeneValue;
    }
    return population;
}
//runs the genetic algorithm
int* geneticAlgorithm(int POPULATION_SIZE, int GENE_SIZE, long int maxFitness, int maxGens, int verbosity, int numParents,int mutationRate, int (*calc_ptr)(),int minGeneValue, int maxGeneValue){
    int* population = (int*)malloc(POPULATION_SIZE * GENE_SIZE * sizeof(int));
    int* fitness = (int*)malloc(POPULATION_SIZE * sizeof(int));
    int* primePopulation = (int*)malloc(numParents * GENE_SIZE * sizeof(int));
    int* output = (int*)malloc(GENE_SIZE * sizeof(int));

    fitness = getFitness(fitness,initialisePopulation(population,POPULATION_SIZE,GENE_SIZE,minGeneValue,maxGeneValue),POPULATION_SIZE,GENE_SIZE, numParents,calc_ptr);
    int count = 1;
    while(fitness[0] < maxFitness && count <= maxGens){
        fitness = getFitness(fitness,reproduction(population,naturalSelection(primePopulation,population,fitness,POPULATION_SIZE,GENE_SIZE, numParents),POPULATION_SIZE,GENE_SIZE,numParents, mutationRate,minGeneValue,maxGeneValue),POPULATION_SIZE,GENE_SIZE, numParents, calc_ptr);
        
        if(count % 1 == 0 && verbosity == 1 || count % 1000 == 0){
            printf("Gen %d",count++);
            printIndividual("|Genes|",population,GENE_SIZE,0, calc_ptr,verbosity);
            printf("\r");
        } else {
            count++;
        }
    }
    for(int i = 0; i < GENE_SIZE; i++){
        *(output+i) = *(population+i);
    }
    free(population);
    free(fitness);
    free(primePopulation); 
    return(output);
}

//__________Example Fitness Functions__________
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
//this is another fitness function that can be used to test the genes of an individual
// in this the goal is to have a fitness that is lower, so we return the negative of the fitness
//a lower fitness value makes the number more "prime"
//but less than 2 factors is not prime so we return a very low value to filter out those results
int primeCalculator(int* population, int size, int index){
    int fitness = 0;
    int num = 0;

    for (int i = 0; i < size; i++)
    {
        num += *(population+index*size+i) * pow(10,size-i-1);
    }

    int cnt = 0;

    for (int i = 1; i <= num; i++) {
        if (num % i == 0) {
            cnt++;
        }
    }
    if (cnt < 2) {
        return -1000000000;
    }
    return cnt*-1;   
}


int main(){
    int pop = 100;
    int minGeneValue = 1;
    int maxGeneValue = 3;
    int size = 8;
    int numParents = 3;
    int mutationRate = 100;
    int maxGens = 1000000;
    int maxFitness = size;
    int verbosity = 1;
    int (*calc_ptr)(int*population,int size, int index);
    calc_ptr = &fitnessCalculator;
    int * result = geneticAlgorithm(pop,size,maxFitness,maxGens,verbosity,numParents,mutationRate,calc_ptr,minGeneValue,maxGeneValue);
    printIndividual("Result:",result,size,0, calc_ptr,1);
    free(result);
    return 0;
}