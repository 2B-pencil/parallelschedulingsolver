#include "parallelschedulingsolver/algorithms/greedy_longest_processing_time.hpp"

#include "parallelschedulingsolver/algorithm_formatter.hpp"

#include <numeric>

using namespace parallelschedulingsolver;

Output parallelschedulingsolver::greedy_longest_processing_time(
    const Instance& instance,
    const Parameters& parameters)
{
    Output output(instance);
    AlgorithmFormatter algorithm_formatter(instance, parameters, output);
    algorithm_formatter.start("Greedy, longest processing-time");
    algorithm_formatter.print_header();

    SolutionBuilder solution_builder;
    solution_builder.set_instance(instance);

    MachineId machine_id = 0; // single machine

    // Sort jobs in decreasing order of processing time.
    std::vector<JobId> indices(instance.number_of_jobs());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(),
            [&instance, machine_id](JobId job_1_id, JobId job_2_id)
            {
                return instance.job(job_1_id).processing_times[machine_id]
                    > instance.job(job_2_id).processing_times[machine_id];
            });

    Time current_time = 0;
    for (JobId job_id: indices) {
        const Job& job = instance.job(job_id);
        Time start = (std::max)(current_time, job.release_date);
        solution_builder.append_job_to_machine(machine_id, job_id, start);
        current_time = start + job.processing_times[machine_id];
    }

    Solution solution = solution_builder.build();
    algorithm_formatter.update_solution(solution, "greedy");
    algorithm_formatter.end();
    return output;
}
