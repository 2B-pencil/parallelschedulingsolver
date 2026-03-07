#include "parallelschedulingsolver/solution.hpp"

using namespace parallelschedulingsolver;

double Solution::objective_value() const
{
    if (!feasible()) {
        return (objective_direction(this->instance().objective()) == optimizationtools::ObjectiveDirection::Minimize)?
            +std::numeric_limits<double>::infinity():
            -std::numeric_limits<double>::infinity();
    }
    switch (this->instance().objective()) {
    case Objective::Makespan:
        return this->makespan();
    case Objective::TotalFlowTime:
        return this->total_flow_time();
    case Objective::TotalTardiness:
        return this->total_tardiness();
    case Objective::MaximumLateness:
        return this->maximum_lateness();
    case Objective::Throughput:
        return this->throughput();
    }
    return -1;
}

bool Solution::feasible() const
{
    return (this->number_of_jobs_ == instance().number_of_jobs());
}

bool Solution::strictly_better(const Solution& solution) const
{
    if (!this->feasible())
        return false;
    if (!solution.feasible())
        return true;
    switch (solution.instance().objective()) {
    case Objective::Makespan: {
        return this->makespan() < solution.makespan();
    } case Objective::TotalFlowTime: {
        return this->total_flow_time() < solution.total_flow_time();
    } case Objective::Throughput: {
        return this->throughput() > solution.throughput();
    } case Objective::TotalTardiness: {
        return this->total_tardiness() < solution.total_tardiness();
    } case Objective::MaximumLateness: {
        return this->maximum_lateness() < solution.maximum_lateness();
    }
    }
    return false;
}

void Solution::write(
        const std::string& certificate_path,
        const std::string& format) const
{
    if (certificate_path.empty())
        return;
    std::ofstream file{ certificate_path };
    if (!file.good()) {
        throw std::runtime_error(
                FUNC_SIGNATURE + ": "
                "unable to open file \"" + certificate_path + "\".");
    }

    if (format == "json") {
        nlohmann::json json;
        for (MachineId machine_id = 0;
                machine_id < this->instance().number_of_machines();
                ++machine_id) {
            const MachineSchedule& machine_schedule = this->machine(machine_id);

            for (JobId job_pos = 0;
                    job_pos < (JobId)machine_schedule.jobs.size();
                    ++job_pos) {
                const ScheduledJob& scheduled_job = machine_schedule.jobs[job_pos];
                JobId job_id = scheduled_job.job_id;
                const parallelschedulingsolver::Job& job = this->instance().job(job_id);

                json["machines"][machine_id]["jobs"][job_pos]["job_id"] = job_id;
                json["machines"][machine_id]["jobs"][job_pos]["release_date"] = job.release_date;
                json["machines"][machine_id]["jobs"][job_pos]["due_date"] = job.due_date;
                json["machines"][machine_id]["jobs"][job_pos]["weight"] = job.weight;
                json["machines"][machine_id]["jobs"][job_pos]["processing_time"] = job.processing_times[machine_id];
                json["machines"][machine_id]["jobs"][job_pos]["start"] = scheduled_job.start;
                json["machines"][machine_id]["jobs"][job_pos]["completion"] = scheduled_job.completion;
            }

            json["machines"][machine_id]["makespan"] = machine_schedule.makespan;
            json["machines"][machine_id]["total_flow_time"] = machine_schedule.total_flow_time;
            json["machines"][machine_id]["total_tardiness"] = machine_schedule.total_tardiness;
            json["machines"][machine_id]["maximum_lateness"] = machine_schedule.maximum_lateness;
        }
        file << json.dump(4);
    }
}

nlohmann::json Solution::to_json() const
{
    return nlohmann::json {
        {"NumberOfJobs", this->number_of_jobs()},
        {"Makespan", this->makespan()},
        {"TotalFlowTime", this->total_flow_time()},
        {"Throughput", this->throughput()},
        {"TotalTardiness", this->total_tardiness()},
        {"MaximumLateness", this->maximum_lateness()},
    };
}

void Solution::format(
        std::ostream& os,
        int verbosity_level) const
{
    if (verbosity_level >= 1) {
        os
            << "Number of jobs:                " << this->number_of_jobs() << std::endl
            << "Feasible:                      " << this->feasible() << std::endl
            << "Makespan:                      " << this->makespan() << std::endl
            << "Total flow time:               " << this->total_flow_time() << std::endl
            << "Throughput:                    " << this->throughput() << std::endl
            << "Total tardiness:               " << this->total_tardiness() << std::endl
            << "Maximum lateness:              " << this->maximum_lateness() << std::endl
            ;
    }

    const Instance& instance = this->instance();
    if (verbosity_level >= 2) {
        os << std::right << std::endl
            << std::setw(12) << "Machine"
            << std::setw(12) << "Job"
            << std::setw(12) << "Start"
            << std::setw(12) << "Completion"
            << std::setw(12) << "Proc. time"
            << std::endl
            << std::setw(12) << "-------"
            << std::setw(12) << "---"
            << std::setw(12) << "-----"
            << std::setw(12) << "----------"
            << std::setw(12) << "----------"
            << std::endl;
        for (MachineId machine_id = 0;
                machine_id < instance.number_of_machines();
                ++machine_id) {
            const Solution::MachineSchedule& machine_schedule = this->machine(machine_id);
            for (const Solution::ScheduledJob& scheduled_job: machine_schedule.jobs) {
                os
                    << std::setw(12) << machine_id
                    << std::setw(12) << scheduled_job.job_id
                    << std::setw(12) << scheduled_job.start
                    << std::setw(12) << scheduled_job.completion
                    << std::setw(12) << instance.job(scheduled_job.job_id).processing_times[machine_id]
                    << std::endl;
            }
        }
    }
}
