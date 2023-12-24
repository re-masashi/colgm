#include "colgm.h"
#include "err.h"
#include "lexer.h"

#include <unordered_map>
#include <thread>
#include <cstdlib>

const u32 VM_RAW_AST   = 1;
const u32 VM_AST       = 1<<1;
const u32 VM_CODE      = 1<<2;
const u32 VM_TIME      = 1<<3;
const u32 VM_EXEC      = 1<<4;
const u32 VM_DETAIL    = 1<<5;
const u32 VM_DEBUG     = 1<<6;
const u32 VM_SYMINFO   = 1<<7;
const u32 VM_PROFILE   = 1<<8;
const u32 VM_PROF_ALL  = 1<<9;
const u32 VM_REF_FILE  = 1<<10;

std::ostream& help(std::ostream& out) {
    out
    << "\n"
    << "     ,--#-,\n"
    << "<3  / \\____\\  <3\n"
    << "    |_|__A_|\n"
#ifdef _WIN32
    << "use command <chcp 65001> to use unicode.\n"
#endif
    << "\ncolgm <option>\n"
    << "option:\n"
    << "   -h,   --help     | get help.\n"
    << "   -v,   --version  | get version.\n"
    << "   -r,   --repl     | use repl interpreter(experimental).\n"
    << "\ncolgm [option] <file> [argv]\n"
    << "option:\n"
    << "   -a,   --ast      | view ast after link/optimize process.\n"
    << "         --raw-ast  | view ast without after-processing.\n"
    << "   -c,   --code     | view generated bytecode.\n"
    << "   -s,   --symbol   | show analysed symbol info.\n"
    << "   -e,   --exec     | execute directly.\n"
    << "   -t,   --time     | show execute time.\n"
    << "   -d,   --detail   | get detail info.\n"
    << "   -f,   --ref-file | get referenced files.\n"
    << "   -dbg, --debug    | debug mode.\n"
    << "         --prof     | show profiling result, available in debug mode.\n"
    << "         --prof-all | show profiling result of all files,"
    << "available under debug mode.\n"
    << "file:\n"
    << "   <filename>       | execute file.\n"
    << "argv:\n"
    << "   <args>           | cmd arguments used in program.\n"
    << "\n";
    return out;
}

std::ostream& logo(std::ostream& out) {
    out
    << "\n"
    << "       __                _\n"
    << "    /\\ \\ \\__ _ ___  __ _| |\n"
    << "   /  \\/ / _` / __|/ _` | |\n"
    << "  / /\\  / (_| \\__ \\ (_| | |\n"
    << "  \\_\\ \\/ \\__,_|___/\\__,_|_|\n"
    << "\n"
    << "ver  : " << __colgm_ver__
    << " " << colgm::get_platform() << " " << colgm::get_arch()
    << " (" << __DATE__ << " " << __TIME__ << ")\n"
    << "std  : c++ " << __cplusplus << "\n"
    << "core : " << std::thread::hardware_concurrency() << " core(s)\n"
    << "\n"
    << "presented by fgprc members - http://fgprc.org.cn\n"
    << "\n"
    << "input <colgm -h> to get help .\n\n";
    return out;
}

std::ostream& version(std::ostream& out) {
    out << "colgm compiler version " << __colgm_ver__;
    out << " " << colgm::get_platform() << " " << colgm::get_arch();
    out << " (" << __DATE__ << " " << __TIME__ << ")\n";
    return out;
}

[[noreturn]]
void err() {
    std::cerr
    << "invalid argument(s).\n"
    << "use <colgm -h> to get help.\n";
    std::exit(1);
}

void execute(
    const std::string& file,
    const std::vector<std::string>& argv,
    const u32 cmd) {

    using clk = std::chrono::high_resolution_clock;
    const auto den = clk::duration::period::den;

    colgm::lexer lex;

    // lexer scans file to get tokens
    lex.scan(file).chkerr();
}

i32 main(i32 argc, const char* argv[]) {
    // output version info
    if (argc<=1) {
        std::clog << logo;
        return 0;
    }

    // run directly or show help
    if (argc==2) {
        std::string s(argv[1]);
        if (s=="-h" || s=="--help") {
            std::clog << help;
        } else if (s=="-v" || s=="--version") {
            std::clog << version;
        }else if (s[0]!='-') {
            execute(s, {}, VM_EXEC);
        } else {
            err();
        }
        return 0;
    }

    // execute with arguments
    const std::unordered_map<std::string, u32> cmdlst = {
        {"--raw-ast", VM_RAW_AST},
        {"--ast", VM_AST},
        {"-a", VM_AST},
        {"--code", VM_CODE},
        {"-c", VM_CODE},
        {"--symbol", VM_SYMINFO},
        {"-s", VM_SYMINFO},
        {"--exec", VM_EXEC},
        {"-e", VM_EXEC},
        {"--time", VM_TIME|VM_EXEC},
        {"-t", VM_TIME|VM_EXEC},
        {"--detail", VM_DETAIL|VM_EXEC},
        {"-d", VM_DETAIL|VM_EXEC},
        {"--debug", VM_DEBUG},
        {"-dbg", VM_DEBUG},
        {"--prof", VM_PROFILE},
        {"--prof-all", VM_PROF_ALL},
        {"-f", VM_REF_FILE},
        {"--ref-file", VM_REF_FILE}
    };
    u32 cmd = 0;
    std::string filename = "";
    std::vector<std::string> vm_argv;
    for(i32 i = 1; i<argc; ++i) {
        if (cmdlst.count(argv[i])) {
            cmd |= cmdlst.at(argv[i]);
        } else if (!filename.length()) {
            filename = argv[i];
        } else {
            vm_argv.push_back(argv[i]);
        }
    }
    if (!filename.length()) {
        err();
    }
    execute(filename, vm_argv, cmd? cmd:VM_EXEC);
    return 0;
}