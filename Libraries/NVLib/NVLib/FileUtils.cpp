//--------------------------------------------------
// Implementation code for FileUtils
//
// @author: Wild Boar
//--------------------------------------------------

#include "FileUtils.h"
using namespace NVLib;

//--------------------------------------------------
// PathCombine
//--------------------------------------------------

/**
 * Defines the logic to combine paths
 * @param baseFolder The base folder that we are combining
 * @param fileName The name of the file that we are combining with
 */
string FileUtils::PathCombine(const string& baseFolder, const string& fileName) 
{
	auto folder = PrepareFolder(baseFolder);
	auto file = PrepareFile(fileName);

	if (folder == string()) return file;

	auto path = file == string() ? folder : folder + "/" + file;

	StringUtils::Replace(path, "\\", "/");

	return path;
}

/**
 * @brief Prepare the folder for combination
 * @param folder The folder that we are preparing
 * @return The prepared folder
*/
string FileUtils::PrepareFolder(const string& folder) 
{
	if (folder == string()) return string();
	auto lastIndex = folder.size() - 1;
	if (folder[lastIndex] == '/') return folder.substr(0, folder.size() - 1);
	if (folder[lastIndex] == '\\') return folder.substr(0, folder.size() - 1);
	return folder;
}

/**
 * @brief Prepare the file name for combinition 
 * @param file The file name that we are reparing
 * @return The prepared
*/
string FileUtils::PrepareFile(const string& file) 
{
	if (file == string()) return string();
	auto workingFile = file;
	if (workingFile[0] == '.') workingFile = workingFile.substr(1);
	if (workingFile[0] == '/') return workingFile.substr(1);
	if (workingFile[0] == '\\') return workingFile.substr(1);
	return workingFile;
}

//--------------------------------------------------
// BuildFileName
//--------------------------------------------------

/**
 * @brief Construct a filename from parts
 * @param baseName The base name of the file
 * @param index The index of the file
 * @param extension The extension associated with the file
 * @return The resultant filename
*/
string FileUtils::BuildFileName(const string& baseName, int index, const string& extension) 
{
	auto generator = stringstream();
	auto hasUnderscore = baseName[baseName.size() - 1] == '_';
	auto hasExtensionDot = extension[0] == '.';

	auto actualBase = hasUnderscore ? baseName : baseName + '_';
	auto actualExtension = hasExtensionDot ? extension : '.' + extension;

	generator << actualBase << setw(4) << setfill('0') << index << actualExtension;

	return generator.str();
}

//--------------------------------------------------
// GetExtension
//--------------------------------------------------

/**
 * @brief Get the extension associated with the given file 
 * @param fileName The name of the file we are getting the extension for
 * @return The resultant file extension
*/
string FileUtils::GetExtension(const string& fileName) 
{
	auto i = fileName.rfind('.', fileName.length());
	if (i != string::npos) return(fileName.substr(i + 1, fileName.length() - i));	
	else return("");
}

//--------------------------------------------------
// GetFileName
//--------------------------------------------------

/**
 * @brief Retrieve the name of the given file
 * @param path The path that we are getting
 * @return The resultant file name
*/
string FileUtils::GetFileName(const string& path) 
{
	auto pathClone = string(path);

	StringUtils::Replace(pathClone, "\\", "/");

	auto i = pathClone.rfind("/", pathClone.length());
	if (i == string::npos) return path;
	else return pathClone.substr(i + 1, pathClone.length() - i);
}

//--------------------------------------------------
// GetNameWithoutExtension
//--------------------------------------------------

/**
 * @brief Retrieve the name of the file without the given extension
 * @param fileName The name of the file we are getting
 * @return The name of the file without the extension
*/
string FileUtils::GetNameWithoutExtension(const string& fileName) 
{
	auto i = fileName.rfind('.', fileName.length());
	if (i != string::npos) return(fileName.substr(0, i));
	else return fileName;
}

//--------------------------------------------------
// BuildPath
//--------------------------------------------------

/**
 * @brief Creates the path to a file given the folder, template and the index of the file
 * @param folder The folder that the file lives in
 * @param templateName The template of the file name 
 * @param index The index of the file
 * @return The resultant string
*/
string FileUtils::BuildPath(const string& folder, const string& templateName, int index) 
{
	auto numberString = StringUtils::ZeroBufferInt(index, 4);
	auto fileName = string(templateName); StringUtils::Replace(fileName, "[id]", numberString);
	return PathCombine(folder, fileName);
}

//--------------------------------------------------
// IsFolder
//--------------------------------------------------

/**
 * @brief Checks to see if a path is a folder or not
 * @param path The path that we are checking
 * @return true This path is a folder
 * @return false This path is a file
 */
bool FileUtils::IsFolder(const string& path) 
{
	return filesystem::is_directory(path);
}

//--------------------------------------------------
// Exists
//--------------------------------------------------

/**
 * @brief Check to see if a given "disk element" exists
 * @param fileName The name of the file that we are checking for
 * @return True if the file exists, else false
*/
bool FileUtils::Exists(const string& fileName) 
{
	return filesystem::exists(fileName);
}

//--------------------------------------------------
// AddFolder(s)
//--------------------------------------------------

/**
 * @brief Add a new folder to the system
 * @param path The path to the folder that we are adding
*/
void FileUtils::AddFolder(const string& path) 
{
	if (path == string()) return;
	filesystem::create_directory(path);
}

/**
 * @brief Add a set of folders to the system 
 * @param path The path that we are creating
 */
void FileUtils::AddFolders(const string& path) 
{
	if (path == string()) return;
	filesystem::create_directories(path);
}

//--------------------------------------------------
// Remove
//--------------------------------------------------

/**
 * @brief Remove a "disk element" from the system
 * @param path The path to the "element" that we are removing
*/
void FileUtils::Remove(const string& path) 
{
	filesystem::remove(path);
}

/**
 * @brief A stronger remove that kills everything
 * @param path The path that we are removing
 */
void FileUtils::RemoveAll(const string& path) 
{
	filesystem::remove_all(path);
}

//--------------------------------------------------
// GetFileList
//--------------------------------------------------

/**
 * @brief Retrieve a list of files that are in a given folder
 * @param path The path to the list of files
 * @param fileNames The names of the files that we are retrieving
*/
void FileUtils::GetFileList(const string& path, vector<string>& fileNames) 
{
	fileNames.clear();

	for (const auto & file : filesystem::directory_iterator(path)) 
	{
		auto fileName = file.path().string();
		StringUtils::Replace(fileName, "\\", "/"); // Standardize path delimitors
		fileNames.push_back(fileName);
	}

	sort(fileNames.begin(), fileNames.end()); // standardize order
}

//--------------------------------------------------
// WriteFile
//--------------------------------------------------

/**
 * @brief Writes the content of a string to a text file
 * @param fileName The name of the file that is being written to
 * @param data The data that is being written
 */
void FileUtils::WriteFile(const string& fileName, const string& data) 
{
	auto writer = ofstream(fileName);
	if (!writer.is_open()) throw runtime_error("Unable to open file: " + fileName);

	writer << data;

	writer.close();
}

//--------------------------------------------------
// ReadFile
//--------------------------------------------------

/**
 * @brief Defines the logic to read the content from a text file
 * @param fileName The name of the file that we are reading
 * @return The resultant file content
 */
string FileUtils::ReadFile(const string& fileName) 
{
	auto reader = ifstream(fileName);
	if (!reader.is_open()) throw runtime_error("Unable to load: " + fileName);

	auto result = string(istreambuf_iterator<char>(reader), istreambuf_iterator<char>());

	reader.close();

	return result;
}

//--------------------------------------------------
// Copy
//--------------------------------------------------

/**
 * @brief Perform a file copy
 * @param source The source folder
 * @param destination The destination folder
 */
void FileUtils::CopyFile(const string& source, const string& destination) 
{
	filesystem::copy(source, destination, filesystem::copy_options::overwrite_existing);
}

//--------------------------------------------------
// Move
//--------------------------------------------------

/**
 * @brief Perform a file move
 * @param source The source folder
 * @param distination The destination folder
 */
void FileUtils::MoveFile(const string& source, const string& destination) 
{
	filesystem::copy(source, destination, filesystem::copy_options::overwrite_existing);
	filesystem::remove(source);
}

//--------------------------------------------------
// GetFolderElementCount
//--------------------------------------------------

/**
 * @brief Retrieve the number of elements in the given folder
 * @param folder The folder that we are checking
 * @return int The number of elements that were found
 */
int FileUtils::GetFileCount(const string& folder) 
{
	using std::filesystem::directory_iterator;
    using fp = bool (*)( const std::filesystem::path&);
    return std::count_if(directory_iterator(folder), directory_iterator{}, (fp)std::filesystem::is_regular_file);
}

//--------------------------------------------------
// GetFolderElementCount
//--------------------------------------------------

/**
 * @brief Generate a unique string using the uuid idea
 * @return string The string that we have generated
 */
string FileUtils::MakeUniqueName() 
{
	uuid_t uuid; uuid_generate(uuid); auto name = stringstream(); 
	
	for (const auto& element : uuid) 
	{
		int value = element;
		auto stream = stringstream(); stream << std::hex << std::uppercase << value;
		auto hexNumber = stream.str();	
		name << hexNumber;
	}

	return name.str();
}

/**
 * @brief Generate a temporary folder
 * @param basePath The base path to the folder
 * @return string The path to the generated folder
 */
string FileUtils::MakeTempFolder(const string& basePath) 
{
	auto tempName = MakeUniqueName();
	auto path = PathCombine(basePath, tempName);
	AddFolders(path);
	return path;
}

/**
 * @brief Generate a temporary file (that can then be appended to)
 * @param folder The folder that we want the file to exist in
 * @param extension The extension associated with the file
 * @return string The path to the file
 */
string FileUtils::MakeTempFile(const string& folder, const string& extension) 
{
	auto tempName = MakeUniqueName();
	auto ext = string(extension); if (ext[0] != '.') ext = "." + ext;
	auto fileName = tempName + ext;
	auto path = PathCombine(folder, fileName);
	WriteFile(path, string());
	return path;
}

//--------------------------------------------------
// File Hashing
//--------------------------------------------------

/**
 * @brief Retrieve the hash code for a provide file
 * @param path The path to the file that we are hashing
 * @return string The resultant hashcode
 */
string FileUtils::GetFileHash(const string& path) 
{
	auto content = NVLib::FileUtils::ReadFile(path);
	return MessageDigest(content, EVP_md5(), MD5_DIGEST_LENGTH);
}

/**
 * @brief Helper method for the given hashing
 * @param s The string that we are evaluating
 * @param evp_md The structure
 * @param digest_length The length
 * @return string The resultant string
 */
string FileUtils::MessageDigest(const std::string& s, const EVP_MD* evp_md, size_t digest_length) 
{
    vector<unsigned char> md(digest_length, 0);
    EVP_MD_CTX* ctx = EVP_MD_CTX_create();
    EVP_MD_CTX_init(ctx);
    EVP_DigestInit_ex(ctx, evp_md, NULL);
    EVP_DigestUpdate(ctx, s.data(), s.size());
    EVP_DigestFinal_ex(ctx, md.data(), NULL);
    EVP_MD_CTX_destroy(ctx);

    stringstream ss;
    for (auto c : md) 
	{
        ss << setfill('0') << setw(2) << hex << (unsigned int)c;
    }

    return ss.str();
}