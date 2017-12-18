#include <print.hpp>
#include <cstdlib>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

void Check(const std::string &path)
{
	std::string text;
	while(std::cin >> text) {
		std::ofstream out(path, std::ios_base::app);
		print(text, out);
		out << std::endl;
	}
}


int main(int argc, const char* argv[]) {

    std::string filename;
    std::string name;
    std::string conf(std::getenv("HOME"));
    conf += "/.config/demo.cfg";

    po::options_description desc("General options");
    desc.add_options()
            ("output", po::value<std::string>(), "File to print to")
            ("variable", po::value<std::string>(&filename))
			      ("name", po::value<std::string>(&name), "Get name from config file");

    std::ifstream config(conf);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::store(po::parse_environment(desc,
          [](const std::string &env_var)
          {
          return env_var == "DEMO_OUTPUT" ? "variable" : "";
          }),
          vm);
    po::store(po::parse_config_file(config, desc), vm);
    po::notify(vm);


    if (vm.count("output"))
    {
      Check(vm["output"].as<std::string>());
    } else
    if (!filename.empty()) {
      Check(filename);
    } else
    if (!name.empty()) {
      Check(name);
    } else {
      Check("default.log");
    }
    config.close();

    return 0;
}

