#include "lexer.hpp"
#include "parser.hpp"
#include <cstdio>

int main(int argc, char** argv) {
    auto& l = lexer::instance();
    if(argc == 2)
	l.for_file(argv[1]);

    while(l.token() != tokens::eof)
	if(parser::parse_function_definition())
	    printf("parsed function\n");
	else {
	    fprintf(stderr, "\n%s\n", lexer::instance().identifier().data());
	    l.consume();
	}
    return 0;
}
