#include "parallelschedulingsolver/generator.hpp"

#include "parallelschedulingsolver/instance_builder.hpp"

using namespace parallelschedulingsolver;

Instance parallelschedulingsolver::generate(
        const GenerateInput& input,
        std::mt19937_64& generator)
{
    InstanceBuilder instance_builder;
    instance_builder.set_objective(input.objective);
    instance_builder.set_number_of_machines(input.number_of_machines);
    JobId number_of_jobs = input.number_of_machines * input.number_of_jobs_per_machine;
    std::uniform_int_distribution<Time> distribution_p(1, input.processing_times_range);
    std::uniform_int_distribution<Time> distribution_w(1, input.weights_range);
    for (JobId job_id = 0; job_id < number_of_jobs; ++job_id) {
        instance_builder.add_job();
        Time processing_time_sum = 0;
        for (MachineId machine_id = 0;
                machine_id < input.number_of_machines;
                ++machine_id) {
            Time processing_time = distribution_p(generator);
            instance_builder.set_job_processing_time(
                    job_id,
                    machine_id,
                    processing_time);
            processing_time_sum += processing_time;
        }
        instance_builder.set_job_weight(job_id, distribution_w(generator));
        Time due_date = input.due_date_tightness_factor * processing_time_sum / input.number_of_machines;
        instance_builder.set_job_due_date(job_id, due_date);
    }
    return instance_builder.build();
}
