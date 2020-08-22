#include "ray-tracing/sandbox.h" 
//#include "lexer/test.h"

int main(int argc, char** argv) { 
    int result = sandbox_render_run(argc, argv);
//    result = lexer_test();
    return result;
}