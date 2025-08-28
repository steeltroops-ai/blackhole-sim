#pragma once

#include <array>
#include <unordered_map>
#include <functional>
#include <vector>
#include <string>

// Forward declaration for GLFW
struct GLFWwindow;

namespace BlackHoleSim {

/**
 * @brief Input system for handling keyboard, mouse, and gamepad input
 * 
 * Provides a unified interface for input handling with support for
 * key bindings, mouse controls, and input callbacks.
 */
class InputSystem {
public:
    /**
     * @brief Key state enumeration
     */
    enum class KeyState {
        RELEASED = 0,
        PRESSED = 1,
        HELD = 2
    };

    /**
     * @brief Mouse button enumeration
     */
    enum class MouseButton {
        LEFT = 0,
        RIGHT = 1,
        MIDDLE = 2
    };

    /**
     * @brief Input action enumeration
     */
    enum class Action {
        // Simulation control
        TOGGLE_PAUSE,
        RESET_SIMULATION,
        STEP_SIMULATION,
        INCREASE_TIME_STEP,
        DECREASE_TIME_STEP,
        
        // Camera control
        CAMERA_FORWARD,
        CAMERA_BACKWARD,
        CAMERA_LEFT,
        CAMERA_RIGHT,
        CAMERA_UP,
        CAMERA_DOWN,
        CAMERA_RESET,
        TOGGLE_CAMERA_MODE,
        
        // Rendering toggles
        TOGGLE_GRID,
        TOGGLE_PARTICLES,
        TOGGLE_TRAILS,
        TOGGLE_LIGHT_RAYS,
        TOGGLE_ACCRETION_DISK,
        TOGGLE_BLACK_HOLE,
        TOGGLE_PHOTON_SPHERE,
        TOGGLE_UI,
        TOGGLE_FULLSCREEN,
        
        // Object spawning
        SPAWN_PARTICLE,
        SPAWN_LIGHT_RAY,
        DELETE_SELECTED,
        
        // Utility
        TAKE_SCREENSHOT,
        SHOW_HELP,
        EXIT_APPLICATION,
        
        // Custom actions
        CUSTOM_1,
        CUSTOM_2,
        CUSTOM_3,
        CUSTOM_4,
        CUSTOM_5
    };

    /**
     * @brief Input callback function type
     */
    using InputCallback = std::function<void(Action action, float value)>;

    /**
     * @brief Mouse callback function type
     */
    using MouseCallback = std::function<void(double x, double y, double deltaX, double deltaY)>;

    /**
     * @brief Scroll callback function type
     */
    using ScrollCallback = std::function<void(double xOffset, double yOffset)>;
    
    /**
     * @brief Key binding structure
     */
    struct KeyBinding {
        Action action;
        bool requireShift;
        bool requireCtrl;
        bool requireAlt;
    };

    /**
     * @brief Construct input system
     */
    InputSystem();

    /**
     * @brief Destructor
     */
    ~InputSystem();

    /**
     * @brief Initialize input system with GLFW window
     * @param window GLFW window handle
     * @return True if initialization successful
     */
    bool Initialize(GLFWwindow* window);

    /**
     * @brief Shutdown input system
     */
    void Shutdown();

    /**
     * @brief Update input state (call once per frame)
     * @param deltaTime Time since last update
     */
    void Update(double deltaTime);

    /**
     * @brief Check if key is currently pressed
     * @param key GLFW key code
     * @return True if key is pressed
     */
    bool IsKeyPressed(int key) const;

    /**
     * @brief Check if key was just pressed this frame
     * @param key GLFW key code
     * @return True if key was just pressed
     */
    bool IsKeyJustPressed(int key) const;

    /**
     * @brief Check if key was just released this frame
     * @param key GLFW key code
     * @return True if key was just released
     */
    bool IsKeyJustReleased(int key) const;

    /**
     * @brief Check if mouse button is currently pressed
     * @param button Mouse button
     * @return True if button is pressed
     */
    bool IsMouseButtonPressed(MouseButton button) const;

    /**
     * @brief Check if mouse button was just pressed this frame
     * @param button Mouse button
     * @return True if button was just pressed
     */
    bool IsMouseButtonJustPressed(MouseButton button) const;

    /**
     * @brief Get current mouse position
     * @return Mouse position [x, y] in screen coordinates
     */
    std::array<double, 2> GetMousePosition() const { return m_mousePosition; }

    /**
     * @brief Get mouse movement delta since last frame
     * @return Mouse delta [dx, dy]
     */
    std::array<double, 2> GetMouseDelta() const { return m_mouseDelta; }

    /**
     * @brief Get scroll wheel delta since last frame
     * @return Scroll delta [dx, dy]
     */
    std::array<double, 2> GetScrollDelta() const { return m_scrollDelta; }

    /**
     * @brief Bind key to action
     * @param key GLFW key code
     * @param action Action to bind
     * @param requireShift Whether Shift key is required
     * @param requireCtrl Whether Ctrl key is required
     * @param requireAlt Whether Alt key is required
     */
    void BindKey(int key, Action action, bool requireShift = false, 
                 bool requireCtrl = false, bool requireAlt = false);

    /**
     * @brief Bind mouse button to action
     * @param button Mouse button
     * @param action Action to bind
     */
    void BindMouseButton(MouseButton button, Action action);

    /**
     * @brief Unbind key from action
     * @param key GLFW key code
     */
    void UnbindKey(int key);

    /**
     * @brief Set input callback for actions
     * @param callback Callback function
     */
    void SetInputCallback(InputCallback callback) { m_inputCallback = callback; }

    /**
     * @brief Set mouse movement callback
     * @param callback Callback function
     */
    void SetMouseCallback(MouseCallback callback) { m_mouseCallback = callback; }

    /**
     * @brief Set scroll callback
     * @param callback Callback function
     */
    void SetScrollCallback(ScrollCallback callback) { m_scrollCallback = callback; }

    /**
     * @brief Enable/disable mouse cursor
     * @param enabled True to show cursor, false to hide
     */
    void SetCursorEnabled(bool enabled);

    /**
     * @brief Check if cursor is enabled
     * @return True if cursor is visible
     */
    bool IsCursorEnabled() const { return m_cursorEnabled; }

    /**
     * @brief Set mouse sensitivity
     * @param sensitivity Mouse sensitivity multiplier
     */
    void SetMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }

    /**
     * @brief Get mouse sensitivity
     * @return Current mouse sensitivity
     */
    float GetMouseSensitivity() const { return m_mouseSensitivity; }

    /**
     * @brief Load key bindings from file
     * @param filename Configuration file path
     * @return True if successful
     */
    bool LoadKeyBindings(const std::string& filename);

    /**
     * @brief Save key bindings to file
     * @param filename Configuration file path
     * @return True if successful
     */
    bool SaveKeyBindings(const std::string& filename) const;

    /**
     * @brief Reset to default key bindings
     */
    void ResetToDefaults();

    /**
     * @brief Get action name as string
     * @param action Action to convert
     * @return Action name string
     */
    static std::string GetActionName(Action action);

    /**
     * @brief Get key name as string
     * @param key GLFW key code
     * @return Key name string
     */
    static std::string GetKeyName(int key);

    /**
     * @brief Get all current key bindings
     * @return Map of key codes to key bindings
     */
    const std::unordered_map<int, KeyBinding>& GetKeyBindings() const { return m_keyBindings; }

    /**
     * @brief Check if any key is currently pressed
     * @return True if any key is pressed
     */
    bool IsAnyKeyPressed() const;

    /**
     * @brief Get list of currently pressed keys
     * @return Vector of pressed key codes
     */
    std::vector<int> GetPressedKeys() const;

private:
    GLFWwindow* m_window;                       ///< GLFW window handle
    bool m_initialized;                         ///< Initialization state
    
    // Input state
    std::unordered_map<int, KeyState> m_keyStates;          ///< Current key states
    std::unordered_map<int, KeyState> m_previousKeyStates;  ///< Previous frame key states
    std::array<bool, 3> m_mouseButtonStates;                ///< Current mouse button states
    std::array<bool, 3> m_previousMouseButtonStates;        ///< Previous mouse button states
    
    // Mouse state
    std::array<double, 2> m_mousePosition;      ///< Current mouse position
    std::array<double, 2> m_previousMousePosition; ///< Previous mouse position
    std::array<double, 2> m_mouseDelta;         ///< Mouse movement delta
    std::array<double, 2> m_scrollDelta;        ///< Scroll wheel delta
    bool m_cursorEnabled;                       ///< Whether cursor is visible
    float m_mouseSensitivity;                   ///< Mouse sensitivity multiplier
    
    // Key bindings
    
    std::unordered_map<int, KeyBinding> m_keyBindings;      ///< Key to action bindings
    std::unordered_map<MouseButton, Action> m_mouseBindings; ///< Mouse button to action bindings
    
    // Callbacks
    InputCallback m_inputCallback;              ///< Input action callback
    MouseCallback m_mouseCallback;              ///< Mouse movement callback
    ScrollCallback m_scrollCallback;            ///< Scroll callback
    
    // Timing
    double m_lastUpdateTime;                    ///< Last update time
    
    /**
     * @brief Initialize default key bindings
     */
    void InitializeDefaultBindings();
    
    /**
     * @brief Process key input and trigger actions
     */
    void ProcessKeyInput();
    
    /**
     * @brief Process mouse input and trigger actions
     */
    void ProcessMouseInput();
    
    /**
     * @brief Update key states
     */
    void UpdateKeyStates();
    
    /**
     * @brief Update mouse states
     */
    void UpdateMouseStates();
    
    /**
     * @brief Check modifier keys
     * @return [shift, ctrl, alt] states
     */
    std::array<bool, 3> GetModifierStates() const;
    
    /**
     * @brief Trigger action callback
     * @param action Action to trigger
     * @param value Action value (0.0-1.0)
     */
    void TriggerAction(Action action, float value = 1.0f);
    
    // Static GLFW callback functions
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    
    // Static instance pointer for callbacks
    static InputSystem* s_instance;
};

} // namespace BlackHoleSim