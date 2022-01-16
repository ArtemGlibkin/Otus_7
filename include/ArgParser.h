#include <boost/program_options.hpp>
#include "FileMonitorParameters.h"

class ConsoleArgumentParser
{
	boost::program_options::options_description desc{ "Options" };
    boost::program_options::variables_map vm;

public:
	ConsoleArgumentParser()
	{
        desc.add_options()
            ("-D, directory", boost::program_options::value<Directories>(), "Input directory")
            ("-R, reject-directory", boost::program_options::value<Directories>(), "Rejected directory")
            ("-S, scan-level", boost::program_options::value<int>()->default_value(0), "Scan Level")
            ("-F, file-mask", boost::program_options::value<std::vector<std::string>>(), "File mask")
            ("-B, block-size", boost::program_options::value<int>()->default_value(1), "Block size")
            ("-A, hash-algorithm", boost::program_options::value<std::string>()->default_value("md5"), "Hash algorithm")
            ("-M, minimal-file-size", boost::program_options::value<int>()->default_value(1), "Minimal file size");
	}

    void parse(int argc, const char* argv[])
    {
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        if (vm.find("-D") == vm.end())
            throw std::runtime_error("ScanDirectories is not defined");
    }

    FileMonitorParameters getParameters()
    {
        FileMonitorParameters ret;
        if (vm.find("-D") != vm.end())
            ret.includeDirectories = vm.at("-D").as<Directories>();
        if (vm.find("-R") != vm.end())
            ret.excludeDirectories = vm.at("-R").as<Directories>();
        if (vm.find("-F") != vm.end())
            ret.filesMasks = vm.at("-F").as<FilesMasks>();
        if (vm.find("-A") != vm.end())
            ret.algorithm = getAlgorithm(vm.at("-A").as<std::string>());

        ret.nestedDirectories = vm.at("-S").as<int>() == 0 ? false : true;
        ret.blockSize = vm.at("-B").as<int>();
        ret.minimalFileSize = vm.at("-M").as<int>();

        return ret;
    }
private:

    HashAlgorithms getAlgorithm(const std::string& str)
    {
        if (str == std::string("sha1"))
            return HashAlgorithms::SHA1;
        else if (str == std::string("md5"))
            return HashAlgorithms::MD5;
        else
            throw "Unknown hash algorithm";
    }

};