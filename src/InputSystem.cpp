#include "InputSystem.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>

namespace BlackHoleSim {

InputSystem::InputSystem() : m_window(nullptr), m_initialized(false) {
    // Initialize key and mouse states
    std::fill(m_keyStates, m_keyStates + GLFW_KEY_LAST + 1, false);
    std::fill(m_prevKeyStates, m_prevKeyStates + GLFW_KEY_LAST + 1, false);
    std::fill(m_mouseButtonStates, m_mouseButtonStates + GLFW_MOUSE_BUTTON_LAST + 1, false);
    std::fill(m_prevMouseButtonStates, m_prevMouseButtonStates + GLFW_MOUSE_BUTTON_LAST + 1, false);
    
    m_mousePosition = {0.0, 0.0};
    m_mouseDelta = {0.0, 0.0};
    m_scrollDelta = {0.0, 0.0};
    m_mouseSensitivity = 1.0f;
    
    // Initialize default key bindings
    InitializeDefaultBindings();
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
    
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        InputSystem* inputSystem = static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
        if (inputSystem) {
            inputSystem->MouseMoveCallback(xpos, ypos);
        }
    });
    
    glfwSetScrollCallback(window, GLFWScrollCallback);
    
    // Get initial cursor position
    glfwGetCursorPos(window, &m_mousePosition[0], &m_mousePosition[1]);
    m_mouseDelta = {0.0, 0.0};
    
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
    
    m_mouseDelta = {0.0, 0.0};
    
    // Reset scroll values (they are set by callbacks)
    m_scrollDelta = {0.0, 0.0};
    
    // Poll GLFW events
    glfwPollEvents();
    
    // Process continuous input (like camera movement)
    ProcessKeyInput();
    ProcessMouseInput();
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
    x = m_mousePosition[0];
    y = m_mousePosition[1];
}

void InputSystem::GetMouseDelta(double& deltaX, double& deltaY) const {
    deltaX = m_mouseDelta[0];
    deltaY = m_mouseDelta[1];
}

void InputSystem::GetScrollOffset(double& offsetX, double& offsetY) const {
    offsetX = m_scrollDelta[0];
    offsetY = m_scrollDelta[1];
}

void InputSystem::SetCursorMode(int mode) {
    if (m_window) {
        glfwSetInputMode(m_window, GLFW_CURSOR, mode);
    }
}

// Callback registration methods removed - use Set*Callback methods from header instead

void InputSystem::KeyCallback(int key, int scancode, int action, int mods) {
    if (key >= 0 && key <= GLFW_KEY_LAST) {
        if (action == GLFW_PRESS) {
            m_keyStates[key] = true;
        } else if (action == GLFW_RELEASE) {
            m_keyStates[key] = false;
        }
    }
    
    // Check for key bindings and trigger actions
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        auto it = m_keyBindings.find(key);
        if (it != m_keyBindings.end()) {
            const KeyBinding& binding = it->second;
            
            // Check modifier requirements
            auto modStates = GetModifierStates();
            bool shiftPressed = modStates[0];
            bool ctrlPressed = modStates[1];
            bool altPressed = modStates[2];
            
            if (binding.requireShift == shiftPressed &&
                binding.requireCtrl == ctrlPressed &&
                binding.requireAlt == altPressed) {
                TriggerAction(binding.action, 1.0f);
            }
        }
    }
}

// HandleMouseButton method removed - logic moved to static MouseButtonCallback

void InputSystem::MouseMoveCallback(double xpos, double ypos) {
    double deltaX = xpos - m_mousePosition[0];
    double deltaY = ypos - m_mousePosition[1];
    
    m_mouseDelta[0] = deltaX * m_mouseSensitivity;
    m_mouseDelta[1] = deltaY * m_mouseSensitivity;
    m_mousePosition[0] = xpos;
    m_mousePosition[1] = ypos;
    
    // Call registered callback
    if (m_mouseCallback) {
        m_mouseCallback(xpos, ypos, deltaX, deltaY);
    }
}

// Static callback functions for GLFW
void InputSystem::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (s_instance) {
        s_instance->KeyCallback(key, scancode, action, mods);
    }
}

void InputSystem::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (s_instance) {
        if (button >= 0 && button <= GLFW_MOUSE_BUTTON_LAST) {
            if (action == GLFW_PRESS) {
                s_instance->m_mouseButtonStates[button] = true;
            } else if (action == GLFW_RELEASE) {
                s_instance->m_mouseButtonStates[button] = false;
            }
        }
        
        // Check for mouse button bindings and trigger actions
        if (action == GLFW_PRESS) {
            MouseButton mouseBtn = static_cast<MouseButton>(button);
            auto it = s_instance->m_mouseBindings.find(mouseBtn);
            if (it != s_instance->m_mouseBindings.end()) {
                s_instance->TriggerAction(it->second, 1.0f);
            }
        }
    }
}

void InputSystem::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (s_instance) {
        double deltaX = xpos - s_instance->m_mousePosition[0];
        double deltaY = ypos - s_instance->m_mousePosition[1];
        
        s_instance->m_mouseDelta[0] = deltaX;
        s_instance->m_mouseDelta[1] = deltaY;
        s_instance->m_mousePosition[0] = xpos;
        s_instance->m_mousePosition[1] = ypos;
        
        // Call registered callback
        if (s_instance->m_mouseCallback) {
            s_instance->m_mouseCallback(xpos, ypos, deltaX, deltaY);
        }
    }
}

void InputSystem::GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (s_instance) {
        s_instance->m_scrollDelta[0] = xoffset;
        s_instance->m_scrollDelta[1] = yoffset;
        
        // Trigger registered callback
        if (s_instance->m_scrollCallback) {
            s_instance->m_scrollCallback(xoffset, yoffset);
        }
    }
}

// Missing method implementations
void InputSystem::BindKey(int key, Action action, bool requireShift, bool requireCtrl, bool requireAlt) {
    KeyBinding binding;
    binding.action = action;
    binding.requireShift = requireShift;
    binding.requireCtrl = requireCtrl;
    binding.requireAlt = requireAlt;
    m_keyBindings[key] = binding;
}

void InputSystem::BindMouseButton(MouseButton button, Action action) {
    m_mouseBindings[button] = action;
}

void InputSystem::UnbindKey(int key) {
    m_keyBindings.erase(key);
}

void InputSystem::InitializeDefaultBindings() {
    // Simulation controls
    BindKey(GLFW_KEY_SPACE, Action::TOGGLE_PAUSE);
    BindKey(GLFW_KEY_R, Action::RESET_SIMULATION);
    BindKey(GLFW_KEY_S, Action::STEP_SIMULATION);
    BindKey(GLFW_KEY_EQUAL, Action::INCREASE_TIME_STEP);
    BindKey(GLFW_KEY_MINUS, Action::DECREASE_TIME_STEP);
    
    // Camera controls
    BindKey(GLFW_KEY_W, Action::CAMERA_FORWARD);
    BindKey(GLFW_KEY_S, Action::CAMERA_BACKWARD);
    BindKey(GLFW_KEY_A, Action::CAMERA_LEFT);
    BindKey(GLFW_KEY_D, Action::CAMERA_RIGHT);
    BindKey(GLFW_KEY_Q, Action::CAMERA_UP);
    BindKey(GLFW_KEY_E, Action::CAMERA_DOWN);
    BindKey(GLFW_KEY_C, Action::CAMERA_RESET);
    BindKey(GLFW_KEY_TAB, Action::TOGGLE_CAMERA_MODE);
    
    // Rendering toggles
    BindKey(GLFW_KEY_G, Action::TOGGLE_GRID);
    BindKey(GLFW_KEY_P, Action::TOGGLE_PARTICLES);
    BindKey(GLFW_KEY_T, Action::TOGGLE_TRAILS);
    BindKey(GLFW_KEY_L, Action::TOGGLE_LIGHT_RAYS);
    BindKey(GLFW_KEY_1, Action::TOGGLE_ACCRETION_DISK);
    BindKey(GLFW_KEY_2, Action::TOGGLE_BLACK_HOLE);
    BindKey(GLFW_KEY_3, Action::TOGGLE_PHOTON_SPHERE);
    BindKey(GLFW_KEY_U, Action::TOGGLE_UI);
    BindKey(GLFW_KEY_F11, Action::TOGGLE_FULLSCREEN);
    
    // Object spawning
    BindKey(GLFW_KEY_N, Action::SPAWN_PARTICLE);
    BindKey(GLFW_KEY_M, Action::SPAWN_LIGHT_RAY);
    BindKey(GLFW_KEY_DELETE, Action::DELETE_SELECTED);
    
    // Utility
    BindKey(GLFW_KEY_F12, Action::TAKE_SCREENSHOT);
    BindKey(GLFW_KEY_H, Action::SHOW_HELP);
    BindKey(GLFW_KEY_ESCAPE, Action::EXIT_APPLICATION);
    
    // Mouse bindings
    BindMouseButton(MouseButton::LEFT, Action::SPAWN_PARTICLE);
    BindMouseButton(MouseButton::RIGHT, Action::SPAWN_LIGHT_RAY);
}

void InputSystem::ProcessKeyInput() {
    // This method is called from Update() to process continuous key states
    // For actions that need continuous input (like camera movement)
    for (const auto& binding : m_keyBindings) {
        int key = binding.first;
        const KeyBinding& keyBinding = binding.second;
        
        if (IsKeyHeld(key)) {
            // Check modifier requirements
            auto modStates = GetModifierStates();
            bool shiftPressed = modStates[0];
            bool ctrlPressed = modStates[1];
            bool altPressed = modStates[2];
            
            if (keyBinding.requireShift == shiftPressed &&
                keyBinding.requireCtrl == ctrlPressed &&
                keyBinding.requireAlt == altPressed) {
                
                // Only trigger continuous actions for movement keys
                switch (keyBinding.action) {
                    case Action::CAMERA_FORWARD:
                    case Action::CAMERA_BACKWARD:
                    case Action::CAMERA_LEFT:
                    case Action::CAMERA_RIGHT:
                    case Action::CAMERA_UP:
                    case Action::CAMERA_DOWN:
                        TriggerAction(keyBinding.action, 1.0f);
                        break;
                    default:
                        // Other actions are handled by KeyCallback on press
                        break;
                }
            }
        }
    }
}

void InputSystem::ProcessMouseInput() {
    // Process mouse button states for continuous actions
    for (const auto& binding : m_mouseBindings) {
        MouseButton button = binding.first;
        Action action = binding.second;
        
        if (IsMouseButtonHeld(static_cast<int>(button))) {
            // Continuous mouse actions can be added here if needed
        }
    }
}

void InputSystem::TriggerAction(Action action, float value) {
    if (m_inputCallback) {
        m_inputCallback(action, value);
    }
}

std::array<bool, 3> InputSystem::GetModifierStates() const {
    std::array<bool, 3> modStates = {false, false, false};
    
    if (m_window) {
        modStates[0] = (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ||
                       (glfwGetKey(m_window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
        modStates[1] = (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ||
                       (glfwGetKey(m_window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);
        modStates[2] = (glfwGetKey(m_window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) ||
                       (glfwGetKey(m_window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS);
    }
    
    return modStates;
}

} // namespace BlackHoleSim