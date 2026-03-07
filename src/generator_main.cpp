#include "parallelschedulingsolver/generator.hpp"

#include <boost/program_options.hpp>

using namespace parallelschedulingsolver;

int main(int argc, char *argv[])
{
    namespace po = boost::program_options;

    GenerateInput input;
    Seed seed = 0;
    std::string output_path = "";

    // Parse program options
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("objective,", po::value<Objective>(&input.objective), "set objective")
        ("number-of-machines,", po::value<MachineId>(&input.number_of_machines), "set number of machines")
        ("number-of-jobs-per-machine,", po::value<JobId>(&input.number_of_jobs_per_machine), "set number of jobs per machine")
        ("processing-times-range,", po::value<Time>(&input.processing_times_range), "set processing times range")
        ("weights-range,", po::value<Time>(&input.weights_range), "set weights range")
        ("seed,", po::value<Seed>(&seed), "set seed")
        ("output,", po::value<std::string>(&output_path)->required(), "set output path")
        ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    if (vm.count("help")) {
        std::cout << desc << std::endl;;
        return 1;
    }
    try {
        po::notify(vm);
    } catch (const po::required_option& e) {
        std::cout << desc << std::endl;;
        return 1;
    }

    std::mt19937_64 generator(seed);
    Instance instance = generate(
            input,
            generator);
    instance.write(output_path, "json");

    return 0;
}
