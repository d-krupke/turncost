//
// Created by Dominik Krupke, http://krupke.cc on 10/5/17.
//

#include <boost/program_options.hpp>
#include <iostream>
#include "problem_instance/grid_graph.h"
#include "problem_instance/save_and_load.h"
#include "apx/approximation.h"
#include "problem_instance/solution/visualization.h"
#include "arkin-apx/approximation.h"
#include "ip/general_cc_solver/ip_solver.h"
#include "ip/integer_programming.h"
#include "arkin-apx/approximation.h"

namespace po = boost::program_options;


enum class Algorithm {
  IP, KRUPKE_APX, ARKINETAL_APX
};

enum class Problem {
  CC, TOUR, SUBSET_CC, SUBSET_TOUR, PENALTY_CC, PENALTY_TOUR
};

struct LogEntry {
  std::string instance;
  std::string params;
  double lower_bound;
  double best_solution;
  double runtime;
  int nr_pixel_covered;
};

void run(std::string instance,
         turncostcover::Costs cost,
         Problem problem,
         Algorithm algorithm,
         bool use_callbacks,
         size_t timeout,
         LogEntry *log)
{
  using clock = std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using time_unit = std::chrono::milliseconds;

  namespace tc = turncostcover;
  namespace aapx = tc::arkinapx;

  auto start = clock::now();
  tc::IntegralSolution solution;
  tc::GridGraph gg;
  switch (problem) {
    case Problem::CC:gg = turncostcover::LoadInstance(instance);
      switch (algorithm) {
        case Algorithm::ARKINETAL_APX:
          solution = aapx::ApproximateFullCycleCoverViaStripCover( gg, cost);
          break;
        case Algorithm::KRUPKE_APX:
          solution = tc::apx::ApproximateCycleCover(gg, cost);
          break;
        case Algorithm::IP:
          solution = tc::ip::ComputeOptimalCycleCover(gg, cost, timeout);
          break;
        default: assert(false);
      }
      break;
    case Problem::TOUR:gg = tc::LoadInstance(instance);
      switch (algorithm) {
        case Algorithm::ARKINETAL_APX:
          solution = aapx::ApproximateFullCycleCoverViaStripCover(gg, cost);
          tc::apx::ConnectAdjacentCycles(&solution, cost);
          break;
        case Algorithm::KRUPKE_APX:
          solution = tc::apx::ApproximateTour(gg, cost);
          break;
        case Algorithm::IP:
          solution = tc::ip::ComputeOptimalTour(gg, cost, timeout, use_callbacks);
          break;
        default: assert(false);
      }
      break;
    case Problem::SUBSET_CC:
    case Problem::SUBSET_TOUR:
    case Problem::PENALTY_CC:
    case Problem::PENALTY_TOUR:
    default: assert(false);
  }
  auto end = clock::now();
  log->lower_bound = solution.GetLowerBound();
  log->best_solution = solution.GetCoverageObjectiveValue(cost);
  if (problem == Problem::PENALTY_CC || problem == Problem::PENALTY_TOUR) {
    //TODO add penalties
    assert(false);
  }
  int nr_covered_fields = 0;
  for (auto v: gg.GetVertices()) {
    if (solution.IsCovered(v)) nr_covered_fields++;
  }
  log->nr_pixel_covered = nr_covered_fields;
  log->runtime = duration_cast<time_unit>(end - start).count();
}

int
main(const int argc, const char **argv)
{

  po::options_description desc("Allowed options");
  std::string filename;
  std::string problem;
  std::string algorithm;
  std::string output_file;
  size_t timeout;
  turncostcover::Costs costs;

  desc.add_options()
      ("help,h", "produce help message")
      ("input,i", po::value<std::string>(&filename), "Input file")
      ("problem,p",
       po::value<std::string>(&problem),
       "Problem name: fcc,ft,scc,st,pcc,pt")
      ("algorithm,a",
       po::value<std::string>(&algorithm),
       "Algorithm to use: ip,apx,aapx")
      ("turncost,t",
       po::value<turncostcover::CostUnit>(&(costs.turn_costs)),
       "turn costs")
      ("distcost,d",
       po::value<turncostcover::CostUnit>(&(costs.dist_costs))->default_value(1),
       "distance costs")
      ("timeout",
       po::value<size_t>(&timeout)->default_value(36000, "10 hours"),
       "timeout for IP in s.")
      ("penalty", po::value<double>(), "Uniform penalty for all fields")
      ("output,o", po::value<std::string>(&output_file), "Output file")
      ("nocallbacks","Deactivates the usage of callbacks. Instead only to an optimally solved problem constraints are "
                     "added. This can sometimes be faster because the turn costs eliminate many subtours.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  bool use_callbacks = vm.count("nocallbacks")==0;

  //print help
  if (vm.count("help") > 0) {
    std::cout << desc << std::endl;
    return 0;
  }

  Problem p;
  if (problem == "fcc") p = Problem::CC;
  else if (problem == "ft") p = Problem::TOUR;
  else if (problem == "pcc") p = Problem::PENALTY_CC;
  else if (problem == "pt") p = Problem::PENALTY_TOUR;
  else {
    std::cerr << "Unknown Problem " << problem << std::endl;
    std::exit(1);
  }

  assert(vm.count("penalty") == 0 || algorithm != "aapx");

  Algorithm a;
  if (algorithm == "ip") a = Algorithm::IP;
  else if (algorithm == "apx") a = Algorithm::KRUPKE_APX;
  else if (algorithm == "aapx") a = Algorithm::ARKINETAL_APX;
  else {
    std::cerr << "Unkown Algorithm " << algorithm << std::endl;
    std::exit(1);
  }
  LogEntry log;
  log.instance = filename;
  for (int i = 0; i < argc; ++i) {
    log.params += argv[i] + std::string("|");
  }
  try {
    if (vm.count("penalty")) {
      assert(a == Algorithm::KRUPKE_APX);
      auto penalty = vm.at("penalty").as<double>();
      std::map<turncostcover::Field, double> penalties;
      auto gg = turncostcover::LoadInstance(filename);
      for (auto v: gg.GetVertices()) {
        penalties[v] = penalty;
      }
      using clock = std::chrono::high_resolution_clock;
      using std::chrono::duration_cast;
      using time_unit = std::chrono::milliseconds;

      auto start = clock::now();
      turncostcover::IntegralSolution solution;
      if (p == Problem::PENALTY_CC) {
        solution = turncostcover::apx::ApproximatePenaltyCycleCover(gg,
                                                                    penalties,
                                                                    costs);
      } else if (p == Problem::PENALTY_TOUR) {
        solution =
            turncostcover::apx::ApproximatePenaltyTour(gg, penalties, costs);
      } else {
        assert(false);
      }
      auto end = clock::now();
      log.lower_bound = solution.GetLowerBound();
      log.best_solution = solution.GetCoverageObjectiveValue(costs);
      int nr_covered_fields = 0;
      for (auto v: gg.GetVertices()) {
        if (!solution.IsCovered(v)) log.best_solution += penalties.at(v);
        else nr_covered_fields++;
      }
      log.nr_pixel_covered = nr_covered_fields;
      log.runtime = duration_cast<time_unit>(end - start).count();
    } else {
      run(filename, costs, p, a, use_callbacks, timeout, &log);
    }
  } catch (turncostcover::CouldNotLoadInstanceException &e) {
    std::cerr << e.message << std::endl;
    std::exit(1);
  }

  std::ofstream log_file{output_file, std::ios_base::app};
  if (log_file.is_open()) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    log_file << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S") << "\t" << log.instance
             << "\t" << log.params << "\t" << log.lower_bound << "\t"
             << log.best_solution << "\t"
             << log.runtime << "\t"
             << log.nr_pixel_covered << std::endl;
  } else {
    std::cerr << "Could not log!" << std::endl;
  }
}

