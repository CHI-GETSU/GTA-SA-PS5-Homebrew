#include "install/worker.hpp"
#include <cassert>
#include <iostream>
int main() {
    using store::install::worker_path;
    assert(worker_path("/data/gtasa-homebrew/staging/job"));
    assert(!worker_path("/data/gtasa-homebrew"));
    assert(!worker_path("/data/gtasa-homebrew/../sahin/job"));
    assert(!worker_path("/data/sahin/staging/job"));
    assert(!worker_path("/data/prospero-win/prefixes/gta-sa-ps2-r4"));
    std::cout<<"PASS file worker paths are confined to GTA app storage, excluding the shared prefix and Blur storage\n";
}
