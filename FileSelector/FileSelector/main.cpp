#include <iostream>
#include <filesystem>
#include "FileSelector.h"

using namespace std;

int main()
{

    std::cout << " getFolderFromConfig() function test \n";
    auto result = getFolderFromConfig("Config.txt");
    std::string folder{};

    if(result.has_value())
        folder = result.value();

    std::cout << folder << std::endl;

    std::filesystem::path pFolder(folder);

    std::cout << " directory exists:  " << std::filesystem::exists(pFolder) << std::endl
              << " is directory:      " << std::filesystem::is_directory(pFolder) << std::endl;


    std::cout << "------------------------------------------------------------\n";

    std::cout << " FilePath class test \n";

    std::string filein = "LGF_originale.xlsx";
    std::string fileout = "ListinoGF.xlsx";

    FilePath fpath;

    std::string folder2 = "D:\\Users\\se_fe\\Documents\\GAS\\PreparazioneListini";
    fpath.setFolder(folder2);
    fpath.setFilename(filein);
    fpath.setExtension(".xlsx");

    std::cout << fpath.getFilepath() << std::endl;

    std::cout << "------------------------------------------------------------\n";

    std::cout << " FileSelector class test \n";


    FileSelector selector;

    std::string folder3 = "D:\\Users\\se_fe\\Documents\\GAS\\PreparazioneListini";
    selector.setFolder(folder3);
    selector.setExtension(".xlsx");

    auto filepath = selector.getSelectedFile();

    if (filepath.has_value())
        std::cout << filepath.value() << std::endl;
    else
        std::cout << " input selection error!" << std::endl;

    return 0;
}
