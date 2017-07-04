#include "stl_headers.hpp"

#include <iterator>
#include <algorithm>

//#####################################################################################################################
void addStlHeaders(std::vector <std::string>& list)
{
    std::vector <std::string> stl =
    {
        "cstdlib",
        "csignal",
        "csetjmp",
        "cstdarg",
        "typeinfo",
        "typeindex",
        "type_traits",
        "bitset",
        "functional",
        "utility",
        "ctime",
        "chrono",
        "cstddef",
        "initializer_list",
        "tuple",
        "any",
        "optional",
        "variant",

        "new",
        "memory",
        "scoped_allocators",
        "memory_resources",

        "climits",
        "cfloat",
        "cstdint",
        "cinttypes",
        "limits",

        "exception",
        "stdexcept",
        "cassert",
        "system_error",
        "cerrno",

        "cctype",
        "cwctype",
        "cstring",
        "cwchar",
        "cuchar",
        "string",
        "string_view",

        "array",
        "vector",
        "deque",
        "list",
        "forward_list",
        "set",
        "map",
        "unordered_map",
        "unorederd_set",
        "stack",
        "queue",

        "algorithm",
        "execution",

        "iterator",
        "cmath",
        "complex",
        "valarray",
        "random",
        "numeric",
        "ratio",
        "cfenv",

        "iosfwd",
        "ios",
        "istream",
        "ostream",
        "iostream",
        "fstream",
        "sstream",
        "strstream",
        "iomanip",
        "streambuf",
        "cstdio",

        "locale",
        "clocale",
        "codecvt",

        "regex",

        "atomic",
        "thread",
        "mutex",
        "shared_mutex",
        "future",
        "condition_variable",

        "filesystem"
    };

    std::copy(std::begin(stl), std::end(stl), std::back_inserter(list));
}
//---------------------------------------------------------------------------------------------------------------------
//#####################################################################################################################
