#include "FileSelector.h"
#include <iostream>
#include <fstream>
#include <string>
#include <format>


std::optional<std::string> getFolderFromConfig(const std::string config)
{
    std::string folder {};
    std::string config_file = ".\\" + config;

    if(std::filesystem::exists(config_file) && (std::filesystem::is_regular_file(config_file)) )
    {
        std::ifstream dir(config_file, std::ios::in);
        std::getline(dir, folder);
    }
    else
        std::cout << " file " << config << " non trovato!" << std::endl;

    if(std::filesystem::exists(folder))
        return folder;
    else
    return std::nullopt;
}


// class FilePath ------------------------------------------------------

bool FilePath::setFolder(const std::string& folder)
{
        // Controlla se la directory esiste e imposta il separatore corretto
        std::filesystem::path path(folder);
        if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
            std::cerr << "La directory specificata non esiste: " << folder << std::endl;
            return false;
        }

        m_folder = folder;
        win_separator = folder.find('\\') != std::string::npos;
        if (m_folder.back() != (win_separator ? '\\' : '/')) {
            m_folder += (win_separator ? '\\' : '/');
        }
        return true;
}

void FilePath::setFilename(const std::string& filename)
{
        // Rimuove l'eventuale estensione dal nome del file
        size_t pos = filename.find_last_of('.');
        if (pos != std::string::npos) {
            m_extension = filename.substr(pos);
            m_filename = filename.substr(0, pos);
        } else {
            m_filename = filename;
        }
}

void FilePath::setExtension(const std::string& extension)
{
        if (extension.empty() || extension[0] != '.') {
            std::cerr << "L'estensione deve iniziare con un punto: " << extension << std::endl;
            return;
        }
        m_extension = extension;
}

std::string FilePath::getFilepath() const
{
    return m_folder + m_filename + m_extension;
}

const std::string& FilePath::getFolder() const { return m_folder; }

const std::string& FilePath::getFilename() const { return m_filename; }

const std::string& FilePath::getExtension() const { return m_extension; }

bool FilePath::fileExists()
{
    return std::filesystem::exists(getFilepath()) && std::filesystem::is_regular_file(getFilepath());
}

// class FileSelector --------------------------------------


void FileSelector::setExtension(const std::string& ext)
{
    if(!ext.empty())
    {
        if(ext.front()=='.')
            m_extension = ext;
        else
        {
            std::string extension = ".";
            extension += ext;
            m_extension = extension;
        }
    }
}

bool FileSelector::setFolder(const std::string& folder)
{
        // Controlla se la directory esiste e imposta il separatore corretto
        std::filesystem::path path(folder);
        if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
            std::cerr << "La directory specificata non esiste: " << folder << std::endl;
            return false;
        }

        m_folder = folder;
        win_separator = folder.find('\\') != std::string::npos;
        if (m_folder.back() != (win_separator ? '\\' : '/')) {
            m_folder += (win_separator ? '\\' : '/');
        }
        return true;
}

std::string FileSelector::getSelected(size_t sel)
{
    return m_files[sel].string();
}

std::optional<std::string> FileSelector::getSelectedFile()
{
    std::string result = "";

    enumerateFiles();
    infoFiles();

        if (countFiles() == 0 && countValidFiles() == 0)
            content = dir_content::is_empty;
        else if (countFiles() > 0 && countValidFiles() == 0)
            content = dir_content::no_valid_files;
        else if (countFiles() >= 0 && countValidFiles() == 1)
            content = dir_content::only_one_valid_file;
        else if (countFiles() >= 0 && countValidFiles() > 1)
            content = dir_content::some_valid_files;

        switch (FileSelector::content)
        {
            case dir_content::is_empty: {
                std::cout << " la direcory selezionata non contiene file" << std::endl;
                break;  }

            case dir_content::no_valid_files: {
                std::cout << " la direcory selezionata non contiene file con estensione "<< m_extension << std::endl;
                char scelta {'N'};

                std::cout << " si vuole impostare un'altra estensione del file? (Y/N) ";

                std::cin >> scelta;

                if (scelta == 'Y' || scelta == 'y' || scelta == 'S' || scelta == 's')
                {
                    std::string extension = "";
                    std::cout << " inserire l'estensione: ";
                    std::cin >> extension;
                    setExtension(extension);
                    getSelectedFile();
                }
                break;  }

            case dir_content::only_one_valid_file: {
                std::cout << " la direcory selezionata contiene un solo file valido: " << std::endl;
                // PrintFilesInFolder();

                std::cout << " si vuole selezionare il file? (Y/N) ";

                char scelta {'N'};
                std::cin >> scelta;
                if (scelta == 'Y' || scelta == 'y' || scelta == 'S' || scelta == 's')
                    result = getSelected(1);
                std::cout << std::endl;
                break;  }

            default: {
                std::cout << " la directory selezionata contiene " << countValidFiles() << " file validi." << std::endl << std::endl;
                    printValidFiles();

                    std::cout << " scegliere uno dei file:   ";
                    auto id {0};

                    std::cin >> id;

                    result = getSelected(id);

                    break;   }
        }

        if (std::filesystem::exists(result))
            return result;
        else
            return std::nullopt;
}

bool FileSelector::folderOk()
{
    bool res { false };
    res = (std::filesystem::exists(m_folder) && (std::filesystem::is_directory(m_folder)));
    return res;
}

void FileSelector::enumerateFiles()
{
    files_in_folder = 0;
    valid_files = 1;
    m_files.clear(); 
    if(std::filesystem::is_empty(std::filesystem::path(m_folder)))
        std::cout << " la directory selezionata è vuota" << std::endl;
    else
    {
        for (auto const& file : std::filesystem::directory_iterator{ m_folder })
        {
            if (std::filesystem::is_regular_file(file) && (std::filesystem::path(file).extension() == m_extension))
            {
                m_files.emplace(std::make_pair(valid_files, file));
                ++valid_files;
            }
            ++files_in_folder;
        }
    }
}

size_t FileSelector::countValidFiles() const
{
    return m_files.size();
}

size_t FileSelector::countFiles() const
{
    return files_in_folder;
}

void FileSelector::infoFiles()
{
        std::cout << std::format(" file nella cartella: {:3}", files_in_folder) << std::endl
                  << std::format(" file validi:         {:3}", m_files.size() ) << std::endl;
}

void FileSelector::printValidFiles()
{
    auto get_stem = [&](const std::filesystem::path p) -> std::string { return p.stem().string(); };

    for(auto & file: m_files)
    {
        // std::cout << f.first << "  " << get_stem(f.second) << std::endl;
        std::cout << std::format(" {:2}{} {} ",  file.first, ':', get_stem(file.second))  << std::endl;
    }
    std::cout << std::endl;
}


