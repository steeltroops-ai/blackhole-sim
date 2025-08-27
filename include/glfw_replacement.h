// GLFW Replacement Header - Windows API Implementation
#ifndef GLFW_REPLACEMENT_H
#define GLFW_REPLACEMENT_H

#ifdef __cplusplus
extern "C" {
#endif

// GLFW constants replacement
#define GLFW_PRESS 1
#define GLFW_RELEASE 0
#define GLFW_REPEAT 2

// Key codes (using Windows VK codes)
#define GLFW_KEY_SPACE 32
#define GLFW_KEY_ESCAPE 27
#define GLFW_KEY_A 'A'
#define GLFW_KEY_B 'B'
#define GLFW_KEY_C 'C'
#define GLFW_KEY_D 'D'
#define GLFW_KEY_E 'E'
#define GLFW_KEY_G 'G'
#define GLFW_KEY_H 'H'
#define GLFW_KEY_L 'L'
#define GLFW_KEY_M 'M'
#define GLFW_KEY_N 'N'
#define GLFW_KEY_P 'P'
#define GLFW_KEY_Q 'Q'
#define GLFW_KEY_R 'R'
#define GLFW_KEY_S 'S'
#define GLFW_KEY_T 'T'
#define GLFW_KEY_U 'U'
#define GLFW_KEY_W 'W'
#define GLFW_KEY_1 '1'
#define GLFW_KEY_2 '2'
#define GLFW_KEY_3 '3'
#define GLFW_KEY_EQUAL 187
#define GLFW_KEY_MINUS 189
#define GLFW_KEY_TAB 9
#define GLFW_KEY_DELETE 46
#define GLFW_KEY_F11 122
#define GLFW_KEY_F12 123
#define GLFW_KEY_LEFT_SHIFT 160
#define GLFW_KEY_RIGHT_SHIFT 161
#define GLFW_KEY_LEFT_CONTROL 162
#define GLFW_KEY_RIGHT_CONTROL 163
#define GLFW_KEY_LEFT_ALT 164
#define GLFW_KEY_RIGHT_ALT 165
#define GLFW_KEY_LAST 348

// Window hints (ignored in our implementation)
#define GLFW_SAMPLES 0x0002100D

// Mouse buttons
#define GLFW_MOUSE_BUTTON_LEFT 0
#define GLFW_MOUSE_BUTTON_RIGHT 1
#define GLFW_MOUSE_BUTTON_MIDDLE 2
#define GLFW_MOUSE_BUTTON_LAST 7

// Callback function types
typedef void (*GLFWkeyfun)(GLFWwindow* window, int key, int scancode, int action, int mods);
typedef void (*GLFWmousebuttonfun)(GLFWwindow* window, int button, int action, int mods);
typedef void (*GLFWcursorposfun)(GLFWwindow* window, double xpos, double ypos);
typedef void (*GLFWscrollfun)(GLFWwindow* window, double xoffset, double yoffset);

// Window handle type
typedef void* GLFWwindow;

// Function declarations
int glfwInit(void);
void glfwTerminate(void);
GLFWwindow* glfwCreateWindow(int width, int height, const char* title, void* monitor, void* share);
void glfwDestroyWindow(GLFWwindow* window);
int glfwWindowShouldClose(GLFWwindow* window);
void glfwSetWindowShouldClose(GLFWwindow* window, int value);
void glfwSwapBuffers(GLFWwindow* window);
void glfwPollEvents(void);
int glfwGetKey(GLFWwindow* window, int key);
int glfwGetMouseButton(GLFWwindow* window, int button);
void glfwGetCursorPos(GLFWwindow* window, double* xpos, double* ypos);
void glfwSetWindowUserPointer(GLFWwindow* window, void* pointer);
void* glfwGetWindowUserPointer(GLFWwindow* window);
void glfwGetWindowSize(GLFWwindow* window, int* width, int* height);
void glfwMakeContextCurrent(GLFWwindow* window);
void glfwWindowHint(int hint, int value);
void glfwSwapInterval(int interval);
GLFWkeyfun glfwSetKeyCallback(GLFWwindow* window, GLFWkeyfun callback);
GLFWmousebuttonfun glfwSetMouseButtonCallback(GLFWwindow* window, GLFWmousebuttonfun callback);
GLFWcursorposfun glfwSetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun callback);
GLFWscrollfun glfwSetScrollCallback(GLFWwindow* window, GLFWscrollfun callback);

#ifdef __cplusplus
}
#endif

#endif // GLFW_REPLACEMENT_H
