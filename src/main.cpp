#include <cstdio>
#include <llvm/Support/raw_ostream.h>

#include "lexer.hpp"
#include "parser.hpp"
#include "operator_table.hpp"
#include "code_generator.hpp"

int main(int argc, char** argv) {
    auto l = lexer{};
    std::string module_name = "test_module";
    if(argc == 2) {
	l.for_file(argv[1]);
	module_name = argv[1];
    }

    auto t = operator_table{};
    t.add("=", 1);
    t.add("+", 3);
    t.add("-", 3);
    t.add("*", 4);
    t.add("/", 4);

    auto p = parser(std::move(l), std::move(t));
    auto cg = code_generator(module_name);
    if(auto fe = p.parse_function()) {
	fprintf(stdout, "parsed a function\n");
	if(auto *fir = fe->accept(&cg)) {
	    fprintf(stderr, "read function definition\n");
	    fir->print(llvm::errs());
	}
    }
    fprintf(stderr, "\n");
    return 0;
}
