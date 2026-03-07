#pragma once

#include "parallelschedulingsolver/instance.hpp"

#include <random>

namespace parallelschedulingsolver
{

struct GenerateInput
{
    Objective objective = Objective::Makespan;
    MachineId number_of_machines = 2;
    JobId number_of_jobs_per_machine = 5;
    Time processing_times_range = 100;
    Time weights_range = 1;
    double due_date_tightness_factor = 3.0;
};

Instance generate(
        const GenerateInput& input,
        std::mt19937_64& generator);

}
