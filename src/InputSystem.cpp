#include "InputSystem.hpp"
#include <iostream>
#include <algorithm>

InputSystem::InputSystem() : m_window(nullptr), m_initialized(false) {
    // Initialize key and mouse states
    std::fill(m_keyStates, m_keyStates + GLFW_KEY_LAST + 1, false);
    std::fill(m_prevKeyStates, m_prevKeyStates + GLFW_KEY_LAST + 1, false);
    std::fill(m_mouseButtonStates, m_mouseButtonStates + GLFW_MOUSE_BUTTON_LAST + 1, false);
    std::fill(m_prevMouseButtonStates, m_prevMouseButtonStates + GLFW_MOUSE_BUTTON_LAST + 1, false);
    
    m_mouseX = 0.0;
    m_mouseY = 0.0;
    m_prevMouseX = 0.0;
    m_prevMouseY = 0.0;
    m_scrollX = 0.0;
    m_scrollY = 0.0;
}

InputSystem::~InputSystem() {
    Shutdown();
}

bool InputSystem::Initialize(GLFWwindow* window) {
    if (m_initialized) {
        std::cerr << "InputSystem already initialized!" << std::endl;
        return false;
    }
    
    if (!window) {
        std::cerr << "Invalid window pointer!" << std::endl;
        return false;
    }
    
    m_window = window;
    
    // Set GLFW callbacks
    glfwSetWindowUserPointer(window, this);
    
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        InputSystem* inputSystem = static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
        if (inputSystem) {
            inputSystem->KeyCallback(key, scancode, action, mods);
        }
    });
    
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
        InputSystem* inputSystem = static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
        if (inputSystem) {
            inputSystem->MouseButtonCallback(button, action, mods);
        }
    });
    
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        InputSystem* inputSystem = static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
        if (inputSystem) {
            inputSystem->MouseMoveCallback(xpos, ypos);
        }
    });
    
    glfwSetScrollCallback(window, GLFWScrollCallback);
    
    // Get initial cursor position
    glfwGetCursorPos(window, &m_mouseX, &m_mouseY);
    m_prevMouseX = m_mouseX;
    m_prevMouseY = m_mouseY;
    
    m_initialized = true;
    std::cout << "InputSystem initialized successfully." << std::endl;
    return true;
}

void InputSystem::Shutdown() {
    if (m_initialized && m_window) {
        // Reset callbacks
        glfwSetKeyCallback(m_window, nullptr);
        glfwSetMouseButtonCallback(m_window, nullptr);
        glfwSetCursorPosCallback(m_window, nullptr);
        glfwSetScrollCallback(m_window, nullptr);
        glfwSetWindowUserPointer(m_window, nullptr);
        
        m_window = nullptr;
        m_initialized = false;
        std::cout << "InputSystem shutdown." << std::endl;
    }
}

void InputSystem::Update() {
    if (!m_initialized) return;
    
    // Store previous states
    std::copy(m_keyStates, m_keyStates + GLFW_KEY_LAST + 1, m_prevKeyStates);
    std::copy(m_mouseButtonStates, m_mouseButtonStates + GLFW_MOUSE_BUTTON_LAST + 1, m_prevMouseButtonStates);
    
    m_prevMouseX = m_mouseX;
    m_prevMouseY = m_mouseY;
    
    // Reset scroll values (they are set by callbacks)
    m_scrollX = 0.0;
    m_scrollY = 0.0;
    
    // Poll GLFW events
    glfwPollEvents();
}

bool InputSystem::IsKeyPressed(int key) const {
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return m_keyStates[key] && !m_prevKeyStates[key];
}

bool InputSystem::IsKeyHeld(int key) const {
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return m_keyStates[key];
}

bool InputSystem::IsKeyReleased(int key) const {
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return !m_keyStates[key] && m_prevKeyStates[key];
}

bool InputSystem::IsMouseButtonPressed(int button) const {
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return false;
    return m_mouseButtonStates[button] && !m_prevMouseButtonStates[button];
}

bool InputSystem::IsMouseButtonHeld(int button) const {
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return false;
    return m_mouseButtonStates[button];
}

bool InputSystem::IsMouseButtonReleased(int button) const {
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return false;
    return !m_mouseButtonStates[button] && m_prevMouseButtonStates[button];
}

void InputSystem::GetMousePosition(double& x, double& y) const {
    x = m_mouseX;
    y = m_mouseY;
}

void InputSystem::GetMouseDelta(double& deltaX, double& deltaY) const {
    deltaX = m_mouseX - m_prevMouseX;
    deltaY = m_mouseY - m_prevMouseY;
}

void InputSystem::GetScrollOffset(double& offsetX, double& offsetY) const {
    offsetX = m_scrollX;
    offsetY = m_scrollY;
}

void InputSystem::SetCursorMode(int mode) {
    if (m_window) {
        glfwSetInputMode(m_window, GLFW_CURSOR, mode);
    }
}

void InputSystem::RegisterKeyCallback(KeyCallback callback) {
    m_keyCallbacks.push_back(callback);
}

void InputSystem::RegisterMouseButtonCallback(MouseButtonCallback callback) {
    m_mouseButtonCallbacks.push_back(callback);
}

void InputSystem::RegisterMouseMoveCallback(MouseMoveCallback callback) {
    m_mouseMoveCallbacks.push_back(callback);
}

void InputSystem::RegisterScrollCallback(ScrollCallback callback) {
    m_scrollCallbacks.push_back(callback);
}

void InputSystem::KeyCallback(int key, int scancode, int action, int mods) {
    if (key >= 0 && key <= GLFW_KEY_LAST) {
        if (action == GLFW_PRESS) {
            m_keyStates[key] = true;
        } else if (action == GLFW_RELEASE) {
            m_keyStates[key] = false;
        }
    }
    
    // Call registered callbacks
    for (auto& callback : m_keyCallbacks) {
        callback(key, scancode, action, mods);
    }
}

void InputSystem::MouseButtonCallback(int button, int action, int mods) {
    if (button >= 0 && button <= GLFW_MOUSE_BUTTON_LAST) {
        if (action == GLFW_PRESS) {
            m_mouseButtonStates[button] = true;
        } else if (action == GLFW_RELEASE) {
            m_mouseButtonStates[button] = false;
        }
    }
    
    // Call registered callbacks
    for (auto& callback : m_mouseButtonCallbacks) {
        callback(button, action, mods);
    }
}

void InputSystem::MouseMoveCallback(double xpos, double ypos) {
    m_mouseX = xpos;
    m_mouseY = ypos;
    
    // Call registered callbacks
    for (auto& callback : m_mouseMoveCallbacks) {
        callback(xpos, ypos);
    }
}

void InputSystem::GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (s_instance) {
        s_instance->m_scrollX = xoffset;
        s_instance->m_scrollY = yoffset;
        
        // Trigger registered callbacks
        for (auto& callback : s_instance->m_scrollCallbacks) {
            callback(xoffset, yoffset);
        }
    }
}