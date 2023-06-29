#include "Engine.h"

int main() {

    // Init Engine:

    Engine program;

    // Game Loop:
    while (program.running()) {
        // Update
        program.update();

        // Render
        program.render();
    }

	return 0;
} 