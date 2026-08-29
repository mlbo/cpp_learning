#include "inference_engine.h"
#include "inference_engine.h"

int main() {
    const inference::EngineConfig config;
    return config.num_threads > 0 ? 0 : 1;
}
