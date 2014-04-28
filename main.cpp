#include "common.hpp"

#include <cstdlib>
#include <map>
#include <chrono>

#include "fileinfo.hpp"
#include "fileparser.hpp"
#include "languagestats.hpp"

using namespace std::chrono;

void usage() {
    cout << "Usage: cloc <directory>.\n";
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
    auto extensions = FileParserFactory::getExtensions();
    // TODO use boost::program_options to process command line args

    if (argc <= 1 ||  argc > 2) {
        usage();
    }

    file::path root_path(argv[1]);
    std::map<string, LanguageStats> stats;
    int fileCounter = 0;
    int sourceFileCounter = 0;

    for (auto& ext: extensions) {
        stats[ext] = LanguageStats(ext);
    }

    auto t1 = high_resolution_clock::now();

    for (file::recursive_directory_iterator iter(root_path), end; iter != end; ++iter) {
        file::path path = *iter;
        auto ext = path.extension().generic_string();
        fileCounter++;
        // file extension is recognized
        if (std::find(extensions.begin(), extensions.end(), ext) != extensions.end()) {
            auto parser = FileParserFactory::makeFileParser(path);
            auto info = parser->parseFile(path);
            stats[ext].addFileInfo(info);
            sourceFileCounter++;
        }
    }

    auto t2 = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(t2 - t1).count();
    auto seconds = duration / 1e9;
    cout << "Elapsed time: " << seconds << " seconds, "
         << sourceFileCounter / seconds << " files processed per second." << endl;

    int sumFiles = 0;
    int sumBlankLines = 0;
    int sumCommentLines = 0;
    int sumSourceLines = 0;

    cout << "Overall number of files: " << fileCounter << endl;
    cout << "Overall number of source files: " << sourceFileCounter << endl << endl;
    cout << "Language\tFiles\tBlank\tComment\tCode" << endl;
    cout << "-----------------------------------------------------" << endl;

    for (auto& pair: stats) {
        auto& info = pair.second;
        if (info.getNumFiles() > 0) {
            cout << info.getExtension() << "\t" << info.getNumFiles() << "\t"
                 << info.getBlankLines() << "\t" << info.getCommentLines() << "\t"
                 << info.getSourceLines() << endl;

            sumBlankLines += info.getBlankLines();
            sumFiles += info.getNumFiles();
            sumCommentLines += info.getCommentLines();
            sumSourceLines += info.getSourceLines();
        }
    }

    cout << "-----------------------------------------------------" << endl;
    cout << "Sum:\t" << sumFiles << "\t" << sumBlankLines << "\t"
         << sumCommentLines << "\t" << sumSourceLines << endl;
    auto overallSum = sumBlankLines + sumCommentLines + sumSourceLines;
    if (overallSum > (int) 1e6) {
        cout << "Overall lines of code: " << overallSum / 1e6 << " MLOC" << endl;
    } else {
        cout << "Overall lines of code: " << overallSum << " LOC" << endl;
    }

    return 0;
}

