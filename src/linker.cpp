#include <mcpelauncher/linker.h>

#include "../bionic/linker/linker_soinfo.h"

void solist_init();
soinfo* soinfo_from_handle(void* handle);

namespace linker::libdl {
    std::unordered_map<std::string, void *> get_dl_symbols();
}

void linker::init() {
    solist_init();
    linker::load_library("libdl.so", linker::libdl::get_dl_symbols());
}

void *linker::load_library(const char *name, const std::unordered_map<std::string, void *> &symbols) {
    return soinfo::load_library(name, symbols)->to_handle();
}

size_t linker::get_library_base(void *handle) {
    return soinfo_from_handle(handle)->base;
}

size_t linker::get_library_size(void *handle) {
    return soinfo_from_handle(handle)->size;
}

extern "C" void __loader_assert(const char* file, int line, const char* msg) {
    fprintf(stderr, "linker assert failed at %s:%i: %s\n", file, line, msg);
    abort();
}