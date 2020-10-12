#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <iostream>
#include <string>
#include <locale>
#include <set>
#include <vector>
namespace {

    std::locale def_loc("en_US.UTF-8");
    int parse(std::string *s) {
        size_t i = 0;
        int num = 0;
        bool sign = false;
        while((*s)[i] == ' ') {
            ++i;
        }
        if ((*s)[i] == '-') {
            sign = true;
            ++i;
        }
        while (i < (*s).size()) {
            if ((*s)[i] <= '9' && (*s)[i] >= '0') {
                num = num * 10 + ((*s)[i] - '0');
            } else {
                return 0;
            }
            ++i;
        }
        return sign ? -num : num;
    }
    bool numeric_comparator(std::string s1, std::string s2) {
        int num2 = parse(&s2);
        int num1 = parse(&s1);
        if (num1 == num2) {
            return def_loc(s1, s2);
        }
        return (num2 > num1);
    }
    bool ignore_case_comparator(std::string s1, std::string s2) {
        transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
        transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
        return def_loc(s1, s2);
    }
    class Vector
    {
        using Impl = std::vector<std::string>;
        Impl m_data;
    public:
        Vector(const std::size_t size_hint)
        {
            m_data.reserve(size_hint / 100);
        }
        template <typename Comparator>
        void sort(Comparator comparator)
        { std::sort(m_data.begin(), m_data.end(), comparator); }


        using const_iterator = Impl::const_iterator;
        using value_type = Impl::value_type;

        const_iterator begin() const
        { return m_data.begin(); }
        const_iterator end() const
        { return m_data.end(); }

        void add(const std::string & line)
        {
            m_data.emplace_back(line);
        }
    };


    using Lines = Vector;

    template <class C>
    void print_out(std::ostream & strm, const C & c)
    {
        std::ostream_iterator<typename C::value_type> out(strm, "\n");
        std::copy(c.begin(), c.end(), out);
    }

    void sort_stream(std::istream & input, const bool ignore_case, const bool numeric, const std::size_t size_hint = 1024)
    {
        Lines lines(size_hint);

        std::string line;
        while (std::getline(input, line)) {
            lines.add(line);
        }

        if (numeric) {
            lines.sort(numeric_comparator);
        } else if (ignore_case) {
            lines.sort(ignore_case_comparator);
        } else {
            lines.sort(def_loc);
        }

        print_out(std::cout, lines);
    }

    std::size_t calculate_size(std::istream & input)
    {
        input.seekg(0, std::ios_base::end);
        const auto end_pos = input.tellg();
        input.seekg(0);
        return end_pos;
    }

}

int main(int argc, char ** argv)
{
    bool ignore_case = false;
    bool numeric = false;
    const char * input_name = nullptr;
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (argv[i][1] != '-') {
                const size_t len = std::strlen(argv[i]);
                for (size_t j = 1; j < len; ++j) {
                    switch (argv[i][j]) {
                        case 'f':
                            ignore_case = true;
                            break;
                        case 'n':
                            numeric = true;
                            break;
                    }
                }
            }
            else {
                if (std::strcmp(argv[i], "--ignore-case") == 0) {
                    ignore_case = true;
                }
                else if (std::strcmp(argv[i], "--numeric-sort") == 0) {
                    numeric = true;
                }
            }
        }
        else {
            input_name = argv[i];
        }
    }

    if (input_name != nullptr) {
        std::ifstream f(input_name);
        sort_stream(f, ignore_case, numeric, calculate_size(f));
    }
    else {
        sort_stream(std::cin, ignore_case, numeric);
    }
}
