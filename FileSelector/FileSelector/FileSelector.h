#pragma once

#include <string>
#include <map>
#include <filesystem>
#include <optional>

// Opens a config (text) file containing just a single line as a path to some file or folder
std::optional<std::string> getFolderFromConfig(const std::string config);


// ------------------------------------------------------------------------------------------------------------------
// by setting the path to a directory and the file extension,
// I can only have one file as input and one as output.

#include <filesystem>
#include <string>
#include <iostream>

class FilePath {
public:
    bool setFolder(const std::string& folder);

    void setFilename(const std::string& filename);

    void setExtension(const std::string& extension);

    std::string getFilepath() const;

    bool fileExists();

    // Funzioni accessorie per ottenere le singole componenti
    const std::string& getFolder() const;
    const std::string& getFilename() const;
    const std::string& getExtension() const;

private:
    std::string m_folder, m_filename, m_extension;
    bool win_separator;
};

// ------------------------------------------------------------------------------------------------------------------

// by setting the path to a directory and the file extension, I can choose from the files present with that extension


class FileSelector
{
public:

    FileSelector() = default;
    virtual ~FileSelector() = default;

    void setExtension(const std::string& ext);
    bool setFolder(const std::string& folder);
    std::optional<std::string> getSelectedFile();
    void printValidFiles();

protected:

    void enumerateFiles();
    void infoFiles();
    size_t countValidFiles() const;
    size_t countFiles() const;
    std::string getSelected(size_t sel);

    std::string m_folder {};
    std::string m_extension {};
    std::map<size_t, std::filesystem::path> m_files;
    size_t files_in_folder { 0 };
    size_t valid_files { 1 };

    enum class dir_content { unknown, is_empty, no_valid_files, only_one_valid_file, some_valid_files };

    dir_content content = dir_content::is_empty;

private:

public:
    bool folderOk();
    bool no_extension { false };
    bool win_separator { false };

};





