#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
using std::cout;
using std::vector;
using std::string;

// Returns true, if parsing went ok
bool parse(int ac, char* av[], po::variables_map& vm);


int main(int ac, char* av[]){
	
	po::variables_map args;
	if (parse(ac, av, args)){
		vector<string>::const_iterator iter = 
			args["input-file"].as< vector<string> >().begin();
//	cout << args["input-file"].as< vector<string> >();
	}
	return 0;
}


bool parse(int ac, char* av[], po::variables_map& vm){
	
	// Declare regular options
	po::options_description desc("Allowed options");
	desc.add_options()
    		("help,h", "produce help message")
    		("version,v", "print version information")
		("levels", po::value<string>(), "CP2K output file containing energy levels")
		("cubelist", po::value<string>(), "file with list of wavefunction cubes you wish to extrapolate")
		("hartree", po::value<string>(), "cube file of hartree potential from CP2K")
		("start", po::value<double>(), "distance between extrapolation plane and outermost atom in a.u.")
		("width", po::value<double>(), "length of extrapolation in a.u.")
	;
	
	// Register positional options
	po::positional_options_description p;
	p	.add("levels", 1)
		.add("cubelist", 1)
		.add("hartree", 1)
		.add("start", 1)
		.add("width", 1);

	// Parse
	po::store(po::command_line_parser(ac,av).
			options(desc).positional(p).run(), vm);
	po::notify(vm);    

	if (		vm.count("help") ||
			!vm.count("levels") ||
			!vm.count("hartree") ||
			!vm.count("start") ||
			!vm.count("width")	) {
		cout << "Usage: wfextr.x [options]\n";
		cout << desc << "\n";
	} else if (vm.count("version")) {
		cout << "V1.0, Nov 22nd 2011\n";
	} else {
		return true;
	}

	return false;
}
