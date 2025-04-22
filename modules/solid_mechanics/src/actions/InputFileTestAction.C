// This test action should help create an input file

#include "InputFileTestAction.h"
#include "MooseApp.h"
#include "ActionFactory.h"
#include "MooseObjectAction.h"
#include "Factory.h"
#include "FEProblem.h"

registerMooseAction("SolidMechanicsApp", InputFileTestAction, "add_material");

InputParameters
InputFileTestAction::validParams()
{
  InputParameters params = Action::validParams();
  params.addClassDescription("MWE to add stress_free_temperature to input file");
  params.addRequiredParam<FileName>("csv_file", "Name of csv file that has variable values");
  params.addRequiredParam<FunctionName>("thermal_expansion_function", "thermal expansion function");
  params.addCoupledVar("stress_free_temperature",
                       "The stress free temperature for the thermal expansion data");
  return params;
}

InputFileTestAction::InputFileTestAction(const InputParameters & params) : Action(params) {}

void
InputFileTestAction::act()
{
  Moose::out << "Creating Input File Action" << std::endl;
  // get values from csv
  auto & filename = getParam<FileName>("csv_file");
  std::ifstream file(filename);
  auto line_num = 0;
  std::vector<VariableName> stress_temps;
  for (std::string line; std::getline(file, line);)
  {
    if (line_num > 0)
      stress_temps.push_back(line);
    std::cout << line << std::endl;
    line_num++;
  }
  auto csv_params = _factory.getValidParams("ComputeMeanThermalExpansionFunctionEigenstrain");
  csv_params.set<std::vector<VariableName>>("stress_free_temperature") = stress_temps;
  // csv_params.set<std::vector<VariableName>>("stress_free_temperature") =
  // getParam<std::vector<VariableName>>("stress_free_temperature");
  csv_params.set<std::string>("eigenstrain_name") = "eigen_name";
  csv_params.set<FunctionName>("thermal_expansion_function") =
      getParam<FunctionName>("thermal_expansion_function");
  csv_params.set<double>("thermal_expansion_function_reference_temperature") = 1.2;
  csv_params.set<std::vector<VariableName>>("temperature") =
      getParam<std::vector<VariableName>>("stress_free_temperature");
  _problem->addMaterial("ComputeMeanThermalExpansionFunctionEigenstrain", "mat1", csv_params);
}
