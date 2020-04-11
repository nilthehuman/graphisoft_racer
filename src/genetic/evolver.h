#pragma once

#include <vector>

namespace genetic
{
using Genome = std::vector<double>;

// Interface for genetic classes whose instances have a Genome and can be evolved
class IOrganism
{
public:
    virtual const Genome& getGenome() const { return mGenome; }
    virtual double fitness() = 0; // Individual fitness score; the higher the better
    virtual IOrganism* spawn(const Genome& genome) const = 0; // Create a new organism with the given genome

    const Genome mGenome; // I know, it's a member in an interface, sue me
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
    // Generate equidistant individuals in k-dimensional parameter space
    // void generateInitialLattice
    // void select(double fraction = 0.3) // Keep the top 30% fittest individuals
    // IOrganism* mutate(IOrganism*)
    // IOrganism* crossover(IOrganism*, IOrganism*)
    const IOrganism* const mAdam; // The prototype organism, acts as a factory for new organisms
    std::vector<IOrganism*> mIndividuals; // The current crop of new organisms
};

}
