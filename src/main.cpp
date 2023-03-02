#include <cstdio>

#include "lexer.hpp"
#include "parser.hpp"
#include "semantic_analyzer.hpp"
#include "code_generator.hpp"

int main(int argc, char** argv) {
    lexer l;
    std::string module_name = "test_module";
    if(argc == 2) {
	l = lexer{argv[1]};
	module_name = argv[1];
    }

    auto t = operator_table{};
    t["="] = 0;
    t["+"] = 2;
    t["-"] = 2;
    t["*"] = 3;
    t["/"] = 3;

    auto p = parser(std::move(l), std::move(t));
    auto sa = semantic_analyzer{};
    auto cg = code_generator(module_name);
    if(auto fe = p.parse_function()) {
	fprintf(stdout, "parsed a function\n");
	if(!fe->accept(&sa)) {
	    return -1;
	}
	fprintf(stderr, "finished semantic analysis\n");
	if(auto *fir = fe->accept(&cg)) {
	    fprintf(stderr, "read function definition\n");
	    fir->print(llvm::errs());
	}
    }
    fprintf(stderr, "\n");
    return 0;
}
