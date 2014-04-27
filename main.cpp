#include "common.hpp"

#include <cstdlib>
#include <initializer_list>
#include <map>

#include "fileinfo.hpp"
#include "fileparser.hpp"
#include "lineparser.hpp"

class LanguageStats {
public:
    LanguageStats(const vector<FileInfo>& v): numFiles(v.size()) {
        extension = v[0].getExtension();
        for (auto& info: v) {
            blankLines += info.getBlankLines();
            commentLines += info.getCommentLines();
            sourceLines += info.getSourceLines();
        }
    }

    LanguageStats(string name = ""):
        numFiles(0),
        blankLines(0),
        commentLines(0),
        sourceLines(0),
        extension(name) { }

    int getNumFiles() const { return numFiles; }
    int getBlankLines() const { return blankLines; }
    int getSourceLines() const { return sourceLines; }
    int getCommentLines() const { return commentLines; }
    string getExtension() const { return extension; }

    void addFileInfo(const FileInfo& info) {
        if (extension == "" ) {
            extension = info.getExtension();
        }
        numFiles++;
        blankLines += info.getBlankLines();
        commentLines += info.getCommentLines();
        sourceLines += info.getSourceLines();
    }

private:
    int numFiles;
    int blankLines;
    int commentLines;
    int sourceLines;
    string extension;
};


void usage() {
    cout << "Usage: cloc <directory>.\n";
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
    vector<string> extensions { ".cpp", ".hpp", ".c", ".h", ".py", ".scala", ".java", ".scala", ".clj", ".lisp", ".cs" };
    if (argc <= 1 ||  argc > 2) {
        usage();
    }

    file::path root_path(argv[1]);
    vector<FileInfo> fileInfos;

    int fileCounter = 0;
    int sourceFileCounter = 0;

    for (file::recursive_directory_iterator iter(root_path), end; iter != end; ++iter) {
        file::path path = *iter;
        auto ext = path.extension();
        fileCounter++;
        // file extension is recognized
        if (std::find(extensions.begin(), extensions.end(), ext) != extensions.end()) {
            FileParser parser(path);
            fileInfos.push_back(parser.parseFile());
            sourceFileCounter++;
        }
    }

    std::map<string, LanguageStats> stats;

    int sumFiles = 0;
    int sumBlankLines = 0;
    int sumCommentLines = 0;
    int sumSourceLines = 0;

    for (auto& ext: extensions) {
        stats[ext] = LanguageStats(ext);
    }

    for (auto& info: fileInfos) {
        auto& v = stats.at(info.getExtension());
        v.addFileInfo(info);
    }
    cout << "Overall number of files: " << fileCounter << endl;
    cout << "Overall number of source files: " << sourceFileCounter << endl << endl;

    cout << "Language\tFiles\tBlank\tComment\tCode" << endl;
    cout << "-----------------------------------------------------" << endl;
    for (auto& pair: stats) {
        auto info = pair.second;
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


    return 0;
}

