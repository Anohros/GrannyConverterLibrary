#include "gcl/utilities/stringutility.h"

#include <locale>

namespace GCL::Utilities {

void toUpper(std::string& input) {
    std::locale locale;

    for (auto& chr : input) {
        chr = toupper(chr, locale);
    }
}

void toLower(std::string& input) {
    std::locale locale;

    for (auto& chr : input) {
        chr = tolower(chr, locale);
    }
}

void toUpperFirst(std::string& input) {
    std::locale locale;
    input[0] = toupper(input[0], locale);
}

void toLowerFirst(std::string& input) {
    std::locale locale;
    input[0] = tolower(input[0], locale);
}

}  // namespace GCL::Utilities
