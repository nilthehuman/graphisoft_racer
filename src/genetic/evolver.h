#pragma once

#include <vector>

namespace genetic
{
using Genome = std::vector<double>;

// Interface for genetic classes whose instances have a Genome and can be evolved
class IOrganism
{
public:
    virtual const Genome getMinimumGenes() const { return {}; }; // Define the lowest acceptable value for each gene of this organism
    virtual const Genome getMaximumGenes() const { return {}; }; // Define the highest acceptable value for each gene of this organism
    virtual const Genome getMaximumMutations() const { return {}; }; // Define the largest amounts each gene can be mutated by in a single generation
    virtual double measureFitness() = 0; // Calculate individual fitness score, the higher the better
    virtual IOrganism* spawn(const Genome& genome) const = 0; // Create a new organism with the given genome

    const Genome& getGenome() const { return mGenome; }
    double getFitness() const { return mFitness; }

protected:
    const Genome mGenome; // I know, it's a member in an interface, sue me
    double mFitness; // Hey, look at that, another member!
};

//// Interface for the classic Factory design pattern. The Evolver is going to use this
//class IFactory
//{
//public:
//    template <typename StratT>
//    virtual IOrganism* create<StratT>(const Genome& genome) const = 0; // Spawn a new organism from the given genome
//};

// A genetic incubator for fitter and fitter individuals in each generation
class Evolver
{
public:
    Evolver(const IOrganism* adam, size_t population = 1000, double survivalRate = 0.3);
    ~Evolver();

    void generateInitialLattice(); // Initialize population with a lattice of individuals
    void select(double fraction = 0.3); // Keep the top 30% fittest individuals
    void repopulate(); // Fill up the pool of individuals to at most mPopulation

private:
    IOrganism* mutate(IOrganism*) const; // Semi-randomly mutate a single gene in a single individual
    IOrganism* crossover(IOrganism*, IOrganism*) const; // Create a third individual by meshing together two different ones

private:
    const IOrganism* const mAdam; // The prototype organism, acts as a factory for new individuals. Note that Adam's genome actually does not matter
    const size_t mPopulation = 1000; // The number of individuals in each fresh generation
    const double mSurvivalRate = 0.3; // The amount of individuals kept alive from a generation to the next generation

    std::vector<IOrganism*> mIndividuals; // The current crop of new individuals

    const Genome mMinimumGenes; // Lower bounds on each gene
    const Genome mMaximumGenes; // Upper bounds on each gene
    const std::vector<double> mGeneGaps; // Gaps in the initial genome lattice along each dimension
};

}
