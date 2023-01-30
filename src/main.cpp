#include "lexer/lexer.hpp"

#include <iostream>

int main(int argc, char** argv) {
    auto& l = lexer::instance();
    if(argc == 2)
	l.for_file(argv[1]);

    for(auto tok = l.token(); tok != tokens::eof; l.consume(), tok = l.token())
	std::cout << l.identifier() << ' ' << (int)tok << (tok == tokens::eol ? "\n" : " | ");
    return 0;
}
