//--------------------------------------------------
// Implementation code for StringUtils
//
// @author: Wild Boar
//--------------------------------------------------

#include "StringUtils.h"
using namespace NVLib;

//--------------------------------------------------
// Split
//--------------------------------------------------

/**
 * @brief Split a string into its relevant parts
 * @param value The value that we are splitting
 * @param delimiter The delimiter we are working with
 * @param output The resultant tags
 */
void StringUtils::Split(const string & value, char delimiter, vector<string> & output)
{
    size_t pos = 0; auto s = string(value);
    while ((pos = s.find(delimiter)) != string::npos) 
    {
        auto token = s.substr(0, pos);
        output.push_back(token);
        s.erase(0, pos + 1);
    }

    if (s.length() != 0) output.push_back(s);
}

//--------------------------------------------------
// Trim
//--------------------------------------------------

/**
 * @brief Remove the white space from the ends of a string
 * @param value The string that we are seperating
 * @param output The output string
 */
void StringUtils::Trim(const string & value, string & output)
{
    const std::string& whitespace = " \t\r\n";
    
    const auto strBegin = value.find_first_not_of(whitespace);
    if (strBegin == std::string::npos) return;

    const auto strEnd = value.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    output = value.substr(strBegin, strRange);
}

//--------------------------------------------------
// Replace
//--------------------------------------------------

/**
 * @brief Add the functionality to replace an element within a string with another
 * @param value The value we are updating
 * @param token The token that is being replaced
 * @param newToken The new value for the token
 * @return The number of replacements that occurred
 */
int StringUtils::Replace(string& value, const string& token, const string& newToken) 
{
    if (value.empty()) return 0;

    size_t pos = 0; int counter = 0;

    while ((pos = value.find(token, pos)) != std::string::npos) 
    {
        value.replace(pos, token.length(), newToken);
        pos += newToken.length();
        counter++;
    }

    return counter;
}

//--------------------------------------------------
// ZeroBufferInt
//--------------------------------------------------

/**
 * @brief Add the functionality to zero buffer a given int value and return it as a string
 * @param value The value that we are buffering
 * @param bufferSize The size of the buffer that we are applying
 */
string StringUtils::ZeroBufferInt(int value, int bufferSize) 
{
    auto converter = stringstream();
    converter << setw(bufferSize) << setfill('0') << value;
    return converter.str();
}

//----------------------------------------------------------------------------------
// BufferString
//----------------------------------------------------------------------------------

/**
 * @brief Add the functionality to buffer the associated string
 * @param value The value that we are buffering
 * @param length The length of the string
 * @param token The token that we are adding
 * @return string The resultant buffered string
 */
string StringUtils::BufferString(const string& value, int length, const string& token) 
{
    auto result = stringstream();

    for (auto i = 0; i < length; i++) 
    {
        if (i < value.length()) result << value[i];
        else result << token;
    }

    return result.str();
}

//----------------------------------------------------------------------------------
// GetTimeString
//----------------------------------------------------------------------------------

/**
 * @brief Retrieves a given time string
 * @return Retrieve the given time string
 */
string StringUtils::GetTimeString()
{
    // get current time
    auto now = chrono::system_clock::now();

    // get number of milliseconds for the current second
    // (remainder after division into seconds)
    auto ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;

    // convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = chrono::system_clock::to_time_t(now);

    // convert broken time
   std::ostringstream oss;

     #ifdef __unix__   
     	time_t rawtime;
     	struct tm * timeinfo;
	    timeinfo = localtime(&rawtime);
        oss << std::put_time(timeinfo, "%H_%M_%S_");
        oss << std::setfill('0') << std::setw(3) << ms.count();
    #else
        auto bt = std::tm();
        localtime_s(&bt, &timer);
        oss << std::put_time(&bt, "%H_%M_%S_");
        oss << std::setfill('0') << std::setw(3) << ms.count();
    #endif

    return oss.str();
}

//----------------------------------------------------------------------------------
// GetDateTimeString
//----------------------------------------------------------------------------------

/**
 * @brief Get the associated data time string
 * @return The resultant date time string
 */
string StringUtils::GetDateTimeString()
{
    // get current time
    auto now = chrono::system_clock::now();
    auto currentTime = chrono::system_clock::to_time_t(now);

    // Perform the conversion
    return DateTimeUtils::DateTime2String(currentTime, "%Y-%m-%d %H:%M:%S");
}

//----------------------------------------------------------------------------------
// GetDateString
//----------------------------------------------------------------------------------

/**
 * @brief Retrieve the date information as a data string 
 * @return string The string representing the date string
 */
string StringUtils::GetDateString() 
{
    // get current time
    auto now = chrono::system_clock::now();

    // convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = chrono::system_clock::to_time_t(now);

    // convert broken time
    std::ostringstream oss;

     #ifdef __unix__   
     	time_t rawtime;
     	struct tm * timeinfo;
        char buffer[80];

        time(&rawtime);
	    timeinfo = localtime(&rawtime);
        
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);

        oss << buffer;
    #else
        auto bt = std::tm();
        localtime_s(&bt, &timer);
        oss << std::put_time(&bt, "%Y-%m-%d");
    #endif

    return oss.str();
}

//----------------------------------------------------------------------------------
// String2Int
//----------------------------------------------------------------------------------

/**
 * @brief Convert a string into an integer
 * @param value The value that is being converted
 * @return The resultant integer
 */
int StringUtils::String2Int(const string& value)
{
    auto converter = stringstream(value);
    int result; converter >> result;
    if (converter.fail()) throw runtime_error("Integer conversion failed: " + value);
    return result;
}

//----------------------------------------------------------------------------------
// String2Double
//----------------------------------------------------------------------------------

/**
 * @brief Logic to convert a string into a double
 * @param value The value that we are converting
 * @return The resultant double that we have converted
 */
double StringUtils::String2Double(const string& value) 
{
    auto converter = stringstream(value);
    double result; converter >> result;
    return result;
}

//----------------------------------------------------------------------------------
// Int2String
//----------------------------------------------------------------------------------

/**
 * @brief The logic to convert an integer into a string
 * @param value The integer that we are converting
 * @return The resultant string
 */
string StringUtils::Int2String(int value) 
{
    auto converter = stringstream();
    converter << value;
    return converter.str();
}

//----------------------------------------------------------------------------------
// Double2String
//----------------------------------------------------------------------------------

/**
 * @brief The logic to convert a double into a string
 * @param value The value that we are converting
 * @return The resultant string
 */
string StringUtils::Double2String(double value) 
{
    auto converter = stringstream();
    converter << value;
    return converter.str();
}

//----------------------------------------------------------------------------------
// ToUpper
//----------------------------------------------------------------------------------

/**
 * @brief Convert a string to upper case
 * @param value The string that we are converting
 * @return An uppercase version of the string
 */
string StringUtils::ToUpper(const string& value)
{
    auto myLocale = locale();
    auto result = stringstream();
    for (auto character : value) result << toupper(character, myLocale);
    return result.str();
}

//----------------------------------------------------------------------------------
// StartsWith
//----------------------------------------------------------------------------------

/**
 * @brief Checks to see if a particular string starts with a given token
 * @param value The actual string that we are checking
 * @param token The token that we are checking for
 * @return True if the string starts with the given token
 */
bool StringUtils::StartsWith(const string& value, const string& token) 
{
    return value.rfind(token, 0) == 0;
}

//----------------------------------------------------------------------------------
// EndsWith
//----------------------------------------------------------------------------------

/**
 * @brief Checks to see if the given string ends with the given value
 * @param value The string that we are checking
 * @param token The token that we are checking for
 * @return true There is a match
 * @return false there is no match
 */
bool StringUtils::EndsWith(const string& value, const string& token) 
{
    if (value == string() || token == string()) return false;
    if (token.length() > value.length()) return false;
    auto matchPart = value.substr(value.length() - token.length());
    return matchPart == token;
}

//----------------------------------------------------------------------------------
// Contains
//----------------------------------------------------------------------------------

/**
 * @brief Determines whether the given string contains the given token
 * @param value The value that we are looking for
 * @param token The token that we are looking for
 * @return true The Token is contained within the string
 * @return false The token is not contained within the string
 */
bool StringUtils::Contains(const string& value, const string& token)
{
    if (value == string() || token == string()) return false;
    if (token.length() > value.length()) return false;
    auto index = value.find(token);
    return index != string::npos;
}

//----------------------------------------------------------------------------------
// String2Bool
//----------------------------------------------------------------------------------

/**
 * @brief Converts a string into a boolean
 * @param value The value that we are converting
 * @return The resultant boolean
 */
bool StringUtils::String2Bool(const string& value)
{
    auto upperValue = ToUpper(value); Trim(upperValue, upperValue);

    if (upperValue != "0" && upperValue != "1" && upperValue != "TRUE" && upperValue != "FALSE") throw runtime_error("Boolean conversion failed: " + value);

    return upperValue == "1" || upperValue == string("TRUE");
}

//----------------------------------------------------------------------------------
// Bool2String
//----------------------------------------------------------------------------------

/**
 * @brief Converts a boolean into a string
 * @param value The value that we are converting
 * @return The string value
 */
string StringUtils::Bool2String(bool value) 
{
    return value ? "true" : "false";
}

//----------------------------------------------------------------------------------
// Clone
//----------------------------------------------------------------------------------

/**
 * @brief Clone an existing string
 * @param value The value of the string
 * @return The resultant cloned string
*/
string StringUtils::Clone(const string& value) 
{
    return string(value);
}

//----------------------------------------------------------------------------------
// ToLower
//----------------------------------------------------------------------------------

/**
 * @brief Converts a string to lowercase
 * @param value The value that we are converting
 * @return The resultant string
*/
string StringUtils::ToLower(const string& value) 
{
    auto data = string(value);
    std::transform(data.begin(), data.end(), data.begin(), [](unsigned char c) { return std::tolower(c); });
    return data;
}

//----------------------------------------------------------------------------------
// Combine
//----------------------------------------------------------------------------------

/**
 * @brief Combine two strings
 * @param firstValue The first value
 * @param secondValue The second value
 * @return string The combined string
 */
string StringUtils::Combine(const string& firstValue, const string& secondValue) 
{
    auto result = stringstream();
    result << firstValue << secondValue;
    return result.str();
}

//----------------------------------------------------------------------------------
// GetBracketText
//----------------------------------------------------------------------------------

/**
 * @brief Get the Bracket Text object
 * @param value Retrieve bracket test
 * @param startBracket The start bracket
 * @param endBracket The end bracket
 * @return string The string that we are getting
 */
string StringUtils::GetBracketText(const string& value, char startBracket, char endBracket) 
{
    auto result = stringstream(); bool inside = false; 

    for (auto& current : value) 
    {
        if (!inside) 
        {
            if (current == startBracket) inside = true;
        }
        else 
        {
            if (current == endBracket) break;
            result << current;
        }
    }

    return result.str();
}

//----------------------------------------------------------------------------------
// FindLocations
//----------------------------------------------------------------------------------

/**
 * @brief Find the given set of locations
 * @param value The string we are searching in
 * @param token The token that we are searching for
 * @param locations The locations that we are searching for
 * @return int The number of locations found
 */
int StringUtils::FindLocations(const string& value, const string& token, vector<int>& locations) 
{
    locations.clear();

    auto position = 0;
    while(true) 
    {
        auto substring = value.substr(position);
        auto tokenLocation = substring.find(token);
        if (tokenLocation == string::npos) break;
        locations.push_back((int)tokenLocation + position);
        position += tokenLocation + 1;
        if (position >= value.length() - 1) break; // this is probably overcautious!
    }

    return locations.size();
}

//----------------------------------------------------------------------------------
// Stream2Text
//----------------------------------------------------------------------------------

/**
 * @brief Read all the text in a given stream
 * @param stream The stream that we are reading
 * @return string The resultant text
 */
string StringUtils::Stream2Text(istream& stream) 
{
    auto result = stringstream();

    auto line = string();
    while (getline(stream, line)) 
    {
        result << line << endl;
    }

    return result.str();
}

//----------------------------------------------------------------------------------
// AssembleLines
//----------------------------------------------------------------------------------

/**
 * @brief Assemble a set of lines into a single string
 * @param lines The set of lines that we are were assembling
 * @return string The resultant string
 */
string StringUtils::AssembleLines(vector<string>& lines) 
{
    auto result = stringstream();
    for (auto& line : lines) result << line << endl;
    return result.str();
}

//----------------------------------------------------------------------------------
// BuildFieldName
//----------------------------------------------------------------------------------

/**
 * @brief Assemble a field name with the proper ` escaping
 * @param field The field that we are build the name from
 * @return The resultant string clause
*/
string StringUtils::BuildFieldName(const string& field)
{
    auto parts = vector<string>(); StringUtils::FieldSplit(field, parts);

    auto result = stringstream();
    if (parts.size() == 1) result << "`" << parts[0] << "`";
    else if (parts.size() == 2) result << "`" << parts[0] << "`.`" << parts[1] << "`";
    else throw string("Unknown field format");

    return result.str();
}

//--------------------------------------------------
// FieldSplit
//--------------------------------------------------

/**
 * @brief Defines the functionality to split the string appropriately
 * @param value The value that we are splitting
 * @param parts The resultant parts
 */
void StringUtils::FieldSplit(const string & value, vector<string>&parts)
{
    auto current = stringstream();

    for (auto character : value)
    {
        if (character == '`') continue; // ignore all ` characters

        if (character == '.')
        {
            if (current.tellp() > 0) { parts.push_back(current.str()); current = stringstream(); }
        }
        else current << character;
    }

    if (current.tellp() > 0) { parts.push_back(current.str()); current = stringstream(); }
}

//--------------------------------------------------
// Indent
//--------------------------------------------------

/**
 * @brief Renders a given indent
 * @param tabCount The number of tables
 * @return string The string containing the given number of tabs
 */
string StringUtils::Indent(int tabCount) 
{
    auto result = stringstream();
    for (auto i = 0; i < tabCount; i++) result << "\t";
    return result.str();
}

//--------------------------------------------------
// Numeric Check
//--------------------------------------------------

/**
 * @brief Check to see if a given string is numeric 
 * @param value The value that we are checking
 * @return True indicates the value is numeric, otherwise the system returns false
 */
bool StringUtils::IsNumeric(const string& value) 
{
    auto it = value.begin();
    while (it != value.end() && std::isdigit(*it)) ++it;
    return !value.empty() && it == value.end();
}