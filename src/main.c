#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <bgfx/c99/bgfx.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

static bool s_showStats = false;

static void glfw_errorCallback(int error, const char *description) {
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

static void glfw_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_F1 && action == GLFW_RELEASE) {
        s_showStats = !s_showStats;
    }
}

int main(int argc, char **argv) {
    // Create a GLFW window without an OpenGL context.
    glfwSetErrorCallback(glfw_errorCallback);
    if (!glfwInit()) {
        return 1;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window = glfwCreateWindow(1024, 768, "helloworld", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwSetKeyCallback(window, glfw_keyCallback);

    // Call bgfx_render_frame before bgfx_init to signal to bgfx not to create a render thread.
    // Most graphics APIs must be used on the same thread that created the window.
    bgfx_render_frame(1);

    // Initialize bgfx using the native window handle and window resolution.
    bgfx_init_t init;
    bgfx_init_ctor(&init);
    init.platformData.nwh = glfwGetCocoaWindow(window);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    init.resolution.width = (uint32_t)width;
    init.resolution.height = (uint32_t)height;
    init.resolution.reset = BGFX_RESET_VSYNC;

    if (!bgfx_init(&init)) {
        return 1;
    }

    // Set view 0 to the same dimensions as the window and to clear the color buffer.
    const bgfx_view_id_t kClearView = 0;
    bgfx_set_view_clear(kClearView, BGFX_CLEAR_COLOR, 0x303030ff, 1.0f, 0);
    bgfx_set_view_rect(kClearView, 0, 0, width, height);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
        bgfx_touch(kClearView);

        // Use debug font to print information about this example.
        bgfx_dbg_text_clear(0, false);
        bgfx_dbg_text_printf(0, 0, 0x0f, "Press F1 to toggle stats.");
        bgfx_dbg_text_printf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
        bgfx_dbg_text_printf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
        bgfx_dbg_text_printf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");

        const bgfx_stats_t* stats = bgfx_get_stats();
        bgfx_dbg_text_printf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);

        // Enable stats or debug text.
        bgfx_set_debug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);

        // Advance to next frame. Process submitted rendering primitives.
        bgfx_frame(false);
    }

    bgfx_shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}