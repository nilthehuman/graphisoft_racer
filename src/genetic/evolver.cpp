#include "evolver.h"

#include <cassert>
#include <cmath>
#include <algorithm>
#include <random>

// Our source of pseudo-random integers
std::mt19937 mersenne(42);

constexpr double geneEpsilon = 0.0001;
constexpr double fitnessEpsilon = 1;


genetic::Evolver::Evolver(const IOrganism* adam, size_t population /*= 1000*/, double survivalRate /*= 0.3*/)
    : mAdam(adam)
    , mPopulation(population)
    , mSurvivalRate(survivalRate)
    , mMinimumGenes(adam->getMinimumGenes())
    , mMaximumGenes(adam->getMaximumGenes())
{
    assert(mMinimumGenes.size() == mMaximumGenes.size());
    for (size_t g = 0; g < mMinimumGenes.size(); ++g)
    {
        assert(mMinimumGenes[g] < mMaximumGenes[g]);
    }

    const size_t dimensions = mMinimumGenes.size();
    const size_t valuesPerDimension = (size_t)std::pow(mPopulation, 1.0 / dimensions);
    assert(1 < valuesPerDimension);
    // Calculate the size of steps in each dimension (<=> each gene)
    for (size_t g = 0; g < dimensions; ++g)
    {
        const double range = mMaximumGenes[g] - mMinimumGenes[g];
        const double gap = range / (valuesPerDimension - 1);
        const_cast< std::vector<double>& >(mGeneGaps).push_back(gap);
    }
}

genetic::Evolver::~Evolver()
{
    // Goodbye
    for (const IOrganism* org : mIndividuals)
    {
        delete org;
    }
    delete mAdam;
}

void genetic::Evolver::runFor(size_t generations)
{
    generateInitialLattice();
    for (size_t gen = 0; gen < generations; ++gen)
    {
        select(mSurvivalRate);
        repopulate();
    }
}

void genetic::Evolver::generateInitialLattice()
{
    // Generate equidistant individuals in k-dimensional parameter space

    const size_t dimensions = mMinimumGenes.size();
    const size_t valuesPerDimension = (size_t)std::pow(mPopulation, 1.0/dimensions);

    mIndividuals.reserve((size_t)std::pow(valuesPerDimension, dimensions));
    Genome genome = mMinimumGenes;
    // TODO: maybe there's a simpler way to write this loop?
    bool done = false;
    while (!done)
    {
        mIndividuals.push_back(mAdam->spawn(genome));
        size_t g = 0;
        // Take one step in iteration through parameter space
        while (mMaximumGenes[g] + geneEpsilon < (genome[g] += mGeneGaps[g]))
        {
            genome[g] = mMinimumGenes[g];
            if (++g == dimensions)
            {
                done = true;
                break;
            }
        }
    }

    assert(mIndividuals.size() >  0.5 * mPopulation);
    assert(mIndividuals.size() <= 1.0 * mPopulation);
}

void genetic::Evolver::select(double survivalRate /*= 0.3*/)
{
    assert(0 < survivalRate && survivalRate < 1);

    // TODO: this looks pretty slow, do something about it
    std::vector<double> fitnessScores(mIndividuals.size());
    std::transform(mIndividuals.cbegin(), mIndividuals.cend(), fitnessScores.begin(), [](IOrganism* o) { return o->measureFitness(); });
    std::sort(fitnessScores.begin(), fitnessScores.end(), std::greater<double>());
    if (fitnessScores.front() - fitnessScores.back() < fitnessEpsilon)
    {
        // Looks like this generation is all morons (or all geniuses)
        // You should consider raising the population number
        assert(false);
    }

    // Take the score at the 70th percentile and throw away everyone below
    const double cutoff = fitnessScores[ (size_t)(survivalRate * fitnessScores.size()) ];
    const size_t populationBeforeSelection = mIndividuals.size();
    for (const IOrganism* org : mIndividuals)
    {
        if (org->getFitness() < cutoff + fitnessEpsilon)
        {
            delete org;
        }
    }
    mIndividuals.erase(
        std::remove_if(mIndividuals.begin(), mIndividuals.end(), [cutoff](const IOrganism* org) { return org->getFitness() < cutoff + fitnessEpsilon; }),
        mIndividuals.end()
    );
    const size_t populationAfterSelection = mIndividuals.size();
    assert(populationAfterSelection < populationBeforeSelection * survivalRate * 1.1);
}

void genetic::Evolver::repopulate()
{
    assert(mIndividuals.size() <= mSurvivalRate * mPopulation);
    while (mIndividuals.size() < mPopulation)
    {
        // Apply a bit of mutation
        const IOrganism* orgA = mIndividuals[ mersenne() % mIndividuals.size() ];
        mIndividuals.push_back( mutate(orgA) );

        // Apply a bit crossover
        const IOrganism* orgB = mIndividuals[ mersenne() % mIndividuals.size() ];
        mIndividuals.push_back( crossover(orgA, orgB) );
    }
    assert(mIndividuals.size() <= mPopulation + 1);
}

genetic::IOrganism* genetic::Evolver::mutate(const IOrganism* org) const
{
    Genome newGenome = org->getGenome();
    // Pick a random gene
    const size_t g = mersenne() % mGeneGaps.size();
    // Determine value to offset the gene by
    const double offset = mGeneGaps[g] * (((mersenne() % 200) / 100.0) - 1.0);
    newGenome[g] += offset;
    newGenome[g] = std::max(newGenome[g], mMinimumGenes[g]);
    newGenome[g] = std::min(newGenome[g], mMaximumGenes[g]);
    return mAdam->spawn(newGenome);
}

genetic::IOrganism* genetic::Evolver::crossover(const IOrganism* orgA, const IOrganism* orgB) const
{
    const Genome& genomeA = orgA->getGenome();
    const Genome& genomeB = orgB->getGenome();
    Genome newGenome(genomeA.size());
    for (size_t g = 0; g < genomeA.size(); ++g)
    {
        // Flip a coin on each gene
        if (mersenne() % 2)
        {
            newGenome[g] = genomeA[g];
        }
        else
        {
            newGenome[g] = genomeB[g];
        }
    }
    return mAdam->spawn(newGenome);
}
