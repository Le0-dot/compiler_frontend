#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"
#include "operator_table.hpp"

int main(int argc, char** argv) {
    auto l = lexer{};
    if(argc == 2)
	l.for_file(argv[1]);

    auto t = operator_table{};
    t.add("=", 1);
    t.add("+", 3);
    t.add("-", 3);
    t.add("*", 4);
    t.add("/", 4);

    auto p = parser(std::move(l), std::move(t));
    if(p.parse_function())
	std::cout << "parsed a function\n";
    return 0;
}
