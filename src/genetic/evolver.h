#pragma once

#include <vector>

namespace genetic
{
using Genome = std::vector<double>;

// Interface for genetic classes whose instances have a Genome and can be evolved
class IOrganism
{
public:
    virtual const Genome& getGenome() const = 0; // Get this individual's genome
    virtual double fitness() const = 0; // Individual fitness score; the higher the better
    virtual IOrganism* clone(const Genome& genome) const = 0; // Spawn a new organism from the given genome
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
    std::vector<IOrganism*> mIndividuals;
};

}
