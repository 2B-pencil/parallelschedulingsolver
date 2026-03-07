#pragma once

#include "parallelschedulingsolver/algorithm_formatter.hpp"

namespace parallelschedulingsolver
{

Output greedy_longest_processing_time(
        const Instance& instance,
        const Parameters& parameters = {});

}
