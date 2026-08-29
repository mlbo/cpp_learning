#include "layers.h"
#include "layers.h"

int main() {
    const inference::ReLU layer;
    return layer.name() == "ReLU" ? 0 : 1;
}
