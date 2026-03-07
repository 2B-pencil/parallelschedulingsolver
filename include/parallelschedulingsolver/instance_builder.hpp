#pragma once

#include "parallelschedulingsolver/instance.hpp"

namespace parallelschedulingsolver
{

class InstanceBuilder
{

public:

    /** Constructor. */
    InstanceBuilder() { }

    /** Read instance from a file. */
    void read(
            const std::string& instance_path,
            const std::string& format);

    /** Set the objective. */
    void set_objective(Objective objective) { instance_.objective_ = objective; }

    /**
     * Set the number of machines.
     *
     * This resets the jobs of the instance.
     */
    void set_number_of_machines(MachineId number_of_machines);

    /** Add a job. */
    JobId add_job();

    /** Set the processing-time of a job for a given machine. */
    void set_job_processing_time(
            JobId job_id,
            MachineId machine_id,
            Time processing_time);

    /** Set the release date of a job. */
    void set_job_release_date(
            JobId job_id,
            Time release_date);

    /** Set the due date of a job. */
    void set_job_due_date(
            JobId job_id,
            Time due_date);

    /** Set the weight of a job. */
    void set_job_weight(
            JobId job_id,
            Time weight);

    /*
     * Build
     */

    /** Build. */
    Instance build();

private:

    /*
     * Private methods
     */

    /** Read an instance from a file in 'json' format. */
    void read_json(std::ifstream& file);

    /*
     * Private attributes
     */

    /** Instance. */
    Instance instance_;

};

}
